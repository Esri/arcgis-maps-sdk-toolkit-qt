// COPYRIGHT 2025 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com
/// \file FloorFilterController.cpp

/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
#include "FloorFilterController.h"

#include <QFuture>

// ArcGISRuntime headers
#include <Envelope.h>
#include <EnvelopeBuilder.h>
#include <FloorFacility.h>
#include <FloorLevel.h>
#include <FloorManager.h>
#include <FloorSite.h>
#include <GeometryEngine.h>
#include <Layer.h>
#include <Map.h>
#include <Scene.h>
#include <Viewpoint.h>

// Toolkit headers
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "DisconnectOnSignal.h"
#include "DoOnLoad.h"
#include "GeoViews.h"

// stl headers
#include <cmath>

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {

    /*!
      \internal
      \brief default scale for our "observing" viewpoint to find
      facility intersections if no viewpoint scale is available.
     */
    constexpr double DEFAULT_TARGET_SCALE = 4300.0;

    /*!
      \internal
      \brief Constant padding value for the extent when zooming
      to the geometry of a FloorFacility.
     */
    const float ZOOM_PADDING = 1.5;

    /*!
      \internal
      \brief Given a ListModel and id, finds the element in the ListModel that matches the given id.
     */
    template <typename T>
    T* findElement(const GenericListModel* model, const QString& id)
    {
      const auto rows = model->rowCount();
      for (int i = 0; i < rows; ++i)
      {
        auto index = model->index(i);
        auto item = model->element<T>(index);
        if (item->modelId() == id)
        {
          return item;
        }
      }
      return nullptr;
    }

    /*!
      \internal
      \brief Returns the FloorManager from the GeoView's model.
      Can return null if map is not loaded.
     */
    FloorManager* getFloorManager(QObject* geoView)
    {
      if (auto mapView = qobject_cast<MapViewToolkit*>(geoView))
      {
        if (auto map = mapView->map())
        {
          return map->floorManager();
        }
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(geoView))
      {
        if (auto scene = sceneView->arcGISScene())
        {
          return scene->floorManager();
        }
      }
      return nullptr;
    }

    /*!
      \internal
      \brief Manages the connection between Controller \a self and GeoView \a geoView.
      Attempts to call functor `f` if/when the FloorFilter within the geoModel is loaded.
      This may also cause the geoModel itself to load.
      Will continue to call `f` every time a map/sceneChanged signal is triggered on
      the GeoView.
     */
    template <typename GeoView, typename Func>
    void connectToGeoView(GeoView* geoView, FloorFilterController* self, Func&& f)
    {
      static_assert(
          std::is_same<GeoView, MapViewToolkit>::value ||
              std::is_same<GeoView, SceneViewToolkit>::value,
          "Must be connected to a SceneView or MapView");

      auto connectToGeoModel = [self, geoView, f = std::forward<Func>(f)]
      {
        auto model = getGeoModel(geoView);
        if (!model)
        {
          return;
        }

        // Here we attempt to calls `f` if/when both the GeoModel and FloorManager are loaded.
        // This may happen immediately or asyncnronously.This can be interrupted if GeoView or
        // GeoModel changes in the interim.
        auto c = doOnLoaded(model, self, [self, model, geoView, f = std::move(f)]()
                          {
                            auto floorManager = model->floorManager();
                            if (!floorManager)
                              return;

                            auto c2 = doOnLoaded(floorManager, self, [f = std::move(f)]
                                               {
                                                 f();
                                               });
                            // Destroy the connection `c` if the map/scene changes, or the geoView changes.
                            // This means the connection is only relevant for as long as the model/view is relavant to
                            // the FloorFilterController.
                            disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c2);
                            disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c2);
                          });
        // Destroy the connection `c` if the map/scene changes, or the geoView changes. This means
        // the connection is only relevant for as long as the model/view is relavant to the FloorFilterController.
        disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
        disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c);
      };

      // Hooks up to any geoModels that appear when the mapView/sceneView changed signal is called.
      QObject::connect(geoView, getGeoModelChangedSignal(geoView), self, connectToGeoModel);
      connectToGeoModel();

      // Hook up to any viewpoint changes on the GeoView.
      auto c2 = QObject::connect(geoView, &std::remove_pointer<decltype(geoView)>::type::viewpointChanged,
                                 self, &FloorFilterController::tryUpdateSelection);
      disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c2);
    }
  }

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilterController
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  FloorFilterController::FloorFilterController(QObject* parent) :
    QObject(parent),
    m_levels(new GenericListModel(&FloorFilterLevelItem::staticMetaObject, this)),
    m_facilities(new GenericListModel(&FloorFilterFacilityItem::staticMetaObject, this)),
    m_sites(new GenericListModel(&FloorFilterSiteItem::staticMetaObject, this))
  {
    m_sites->setDisplayPropertyName("name");
    m_facilities->setDisplayPropertyName("name");
    m_facilities->setTooltipPropertyName("parentSiteName");
    m_levels->setDisplayPropertyName("longName");

    connect(this, &FloorFilterController::selectedFacilityIdChanged, this, &FloorFilterController::populateLevelsForSelectedFacility);

    connect(this, &FloorFilterController::selectedSiteIdChanged, this, &FloorFilterController::populateFacilitiesForSelectedSite);
    connect(this, &FloorFilterController::isSelectedSiteRespectedChanged, this, &FloorFilterController::populateFacilitiesForSelectedSite);

    connect(this, &FloorFilterController::selectedLevelIdChanged, this,
            [this](QString /*oldId*/, QString newId)
            {
              auto newLevelItem = level(newId);
              auto newLevel = newLevelItem ? newLevelItem->floorLevel() : nullptr;
              auto floorManager = getFloorManager(m_geoView);
              if (floorManager)
              {
                const auto levels = floorManager->levels();
                for (const auto level : levels)
                {
                  if (level)
                  {
                    switch (m_updatelevelMode)
                    case UpdateLevelsMode::AllLevelsMatchingVerticalOrder:
                    {
                      level->setVisible(newLevel ? level->verticalOrder() == newLevel->verticalOrder() : false);
                    }
                  }
                }
              }
            });
  }

  FloorFilterController::~FloorFilterController()
  {
  }

  QObject* FloorFilterController::geoView() const
  {
    return m_geoView;
  }

  GenericListModel* FloorFilterController::levels() const
  {
    return m_levels;
  }

  GenericListModel* FloorFilterController::sites() const
  {
    return m_sites;
  }

  GenericListModel* FloorFilterController::facilities() const
  {
    return m_facilities;
  }

  void FloorFilterController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
    {
      return;
    }

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);
    }

    m_geoView = geoView;

    // Important that this emit happens before the below connections,
    // as this emit will destroy the connections set up below.
    emit geoViewChanged();

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connectToGeoView(mapView, this, [this]
                       {
                         populateSites();
                       });
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      connectToGeoView(sceneView, this, [this]
                       {
                         populateSites();
                       });
    }
  }

  QString FloorFilterController::selectedFacilityId() const
  {
    return m_selectedFacilityId;
  }

  void FloorFilterController::setSelectedFacilityId(QString selectedFacilityId)
  {
    if (m_selectedFacilityId == selectedFacilityId)
    {
      return;
    }

    QString oldId = m_selectedFacilityId;
    m_selectedFacilityId = std::move(selectedFacilityId);
    emit selectedFacilityIdChanged(std::move(oldId), m_selectedFacilityId);
  }

  QString FloorFilterController::selectedLevelId() const
  {
    return m_selectedLevelId;
  }

  void FloorFilterController::setSelectedLevelId(QString selectedLevelId)
  {
    if (m_selectedLevelId == selectedLevelId)
    {
      return;
    }

    QString oldId = m_selectedLevelId;
    m_selectedLevelId = std::move(selectedLevelId);
    emit selectedLevelIdChanged(std::move(oldId), m_selectedLevelId);
  }

  QString FloorFilterController::selectedSiteId() const
  {
    return m_selectedSiteId;
  }

  void FloorFilterController::setSelectedSiteId(QString selectedSiteId)
  {
    if (m_selectedSiteId == selectedSiteId)
    {
      return;
    }

    QString oldId = m_selectedSiteId;
    m_selectedSiteId = std::move(selectedSiteId);
    emit selectedSiteIdChanged(std::move(oldId), m_selectedSiteId);
  }

  void FloorFilterController::populateLevelsForSelectedFacility()
  {
    m_levels->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
    {
      return;
    }

    auto allLevels = manager->levels();

    if (allLevels.isEmpty())
    {
      setSelectedLevelId({});
      return;
    }

    // Sort levels by vertical order.
    std::sort(std::begin(allLevels), std::end(allLevels), [](FloorLevel* a, FloorLevel* b)
              {
                return a && b ? a->verticalOrder() < b->verticalOrder() : static_cast<bool>(a);
              });

    QString defaultLevel = allLevels.first()->levelId();
    QList<QObject*> levelItems;
    for (const auto level : std::as_const(allLevels))
    {
      if (level && level->facility()->facilityId() == selectedFacilityId())
      {
        levelItems << new FloorFilterLevelItem(level, m_levels);
        if (level->verticalOrder() == 0)
        {
          // Default level is level with vertical order 0.
          defaultLevel = level->levelId();
        }
      }
    }

    // Reverse order in controller for ascending order. Such that bottom floor level is
    // at bottom of the list.
    m_levels->append(QList<QObject*>(std::crbegin(levelItems), std::crend(levelItems)));
    setSelectedLevelId(defaultLevel);
  }

  void FloorFilterController::populateFacilitiesForSelectedSite()
  {
    auto manager = getFloorManager(m_geoView);
    if (!manager)
    {
      m_facilities->clear();
      return;
    }

    // Do not blow out the facilities list with a new list if we are showing all facilities in the
    // FloorManager and we are in ignore selected-site mode. This is not just a micro-optimization,
    // there are cases where selecting a facility in this mode will select a parent site, and we don't
    // want to regenerate the facilities list when the selected site changes.
    if (manager->facilities().length() == m_facilities->rowCount() && !m_selectedSiteRespected)
    {
      return;
    }

    const auto allFacilites = manager->facilities();
    QList<QObject*> facilityItems;
    for (const auto facility : allFacilites)
    {
      // If we have no sites take everything, otherwise filter by the selected site.
      if (!m_selectedSiteRespected || manager->sites().isEmpty() || facility->site()->siteId() == selectedSiteId())
      {
        facilityItems << new FloorFilterFacilityItem(facility, m_facilities);
      }
    }

    m_facilities->clear();
    m_facilities->append(facilityItems);
    // Select only facility if there is only 1 facility.
    if (facilityItems.size() == 1)
    {
      setSelectedFacilityId(static_cast<FloorFilterFacilityItem*>(facilityItems.at(0))->modelId());
    }
    else
    {
      setSelectedFacilityId("");
    }
  }

  void FloorFilterController::populateSites()
  {
    m_sites->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
    {
      return;
    }

    const auto allSites = manager->sites();

    QList<QObject*> siteItems;
    for (const auto site : allSites)
    {
      siteItems << new FloorFilterSiteItem(site, m_sites);
    }
    m_sites->append(siteItems);

    // Select only site if there is only 1 site.
    if (siteItems.size() == 1)
    {
      setSelectedSiteId(static_cast<FloorFilterSiteItem*>(siteItems.at(0))->modelId());
    }
    else
    {
      setSelectedSiteId("");
      // Even if a site isn't selected, we may still need to populate facilities
      // if the isSelectedSiteRespected flag is false or there are 0 sites
      // in the manager.
      populateFacilitiesForSelectedSite();
    }
  }

  void FloorFilterController::zoomToFacility(FloorFilterFacilityItem* facilityItem)
  {
    if (!facilityItem)
    {
      return;
    }

    const auto f = facilityItem->floorFacility();
    if (f)
    {
      zoomToEnvelope(f->geometry().extent());
    }
  }

  void FloorFilterController::zoomToFacility(const QString& facilityId)
  {
    zoomToFacility(facility(facilityId));
  }

  void FloorFilterController::zoomToSite(FloorFilterSiteItem* siteItem)
  {
    if (!siteItem)
      return;

    const auto s = siteItem->floorSite();
    if (s)
    {
      zoomToEnvelope(s->geometry().extent());
    }
  }

  void FloorFilterController::zoomToSite(const QString& siteId)
  {
    zoomToSite(site(siteId));
  }

  FloorFilterFacilityItem* FloorFilterController::facility(const QString& facilityId) const
  {
    return findElement<FloorFilterFacilityItem>(m_facilities, facilityId);
  }

  FloorFilterSiteItem* FloorFilterController::site(const QString& siteId) const
  {
    return findElement<FloorFilterSiteItem>(m_sites, siteId);
  }

  FloorFilterLevelItem* FloorFilterController::level(const QString& levelId) const
  {
    return findElement<FloorFilterLevelItem>(m_levels, levelId);
  }

  FloorFilterFacilityItem* FloorFilterController::selectedFacility() const
  {
    return facility(selectedFacilityId());
  }

  FloorFilterSiteItem* FloorFilterController::selectedSite() const
  {
    return site(selectedSiteId());
  }

  FloorFilterLevelItem* FloorFilterController::selectedLevel() const
  {
    return level(selectedLevelId());
  }

  bool FloorFilterController::isSelectedSiteRespected() const
  {
    return m_selectedSiteRespected;
  }

  void FloorFilterController::setIsSelectedSiteRespected(bool isSelectedSiteRespected)
  {
    if (isSelectedSiteRespected == m_selectedSiteRespected)
    {
      return;
    }

    m_selectedSiteRespected = isSelectedSiteRespected;
    emit isSelectedSiteRespectedChanged();
  }

  FloorFilterController::UpdateLevelsMode FloorFilterController::updateLevelsMode() const
  {
    return m_updatelevelMode;
  }

  void FloorFilterController::setUpdateLevelsMode(UpdateLevelsMode updateLevelsMode)
  {
    if (updateLevelsMode == m_updatelevelMode)
    {
      return;
    }

    m_updatelevelMode = updateLevelsMode;
    emit updateLevelsModeChanged();
  }

  FloorFilterController::AutomaticSelectionMode FloorFilterController::automaticSelectionMode() const
  {
    return m_automaticSelectionMode;
  }

  void FloorFilterController::setAutomaticSelectionMode(AutomaticSelectionMode automaticSelectionMode)
  {
    if (m_automaticSelectionMode == automaticSelectionMode)
    {
      return;
    }

    m_automaticSelectionMode = automaticSelectionMode;
    emit automaticSelectionModeChanged();
  }

  void FloorFilterController::tryUpdateSelection()
  {
    if (m_automaticSelectionMode == AutomaticSelectionMode::Never)
    {
      return;
    }

    if (m_settingViewpoint)
    {
      return;
    }

    Viewpoint observedViewpoint;
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      observedViewpoint = sceneView->currentViewpoint(ViewpointType::CenterAndScale);
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      observedViewpoint = mapView->currentViewpoint(ViewpointType::CenterAndScale);
    }

    // Expectation: viewpoint is center and scale
    if (observedViewpoint.isEmpty() ||
        std::isnan(observedViewpoint.targetScale()))
    {
      return;
    }

    auto floorManager = getFloorManager(m_geoView);

    // Only take action if viewpoint is within minimum scale. Default minscale is 4300 or less (~zoom level 17 or greater)
    double targetScale = 0.0;
    if (floorManager)
    {
      if (auto siteLayer = floorManager->siteLayer())
      {
        targetScale = siteLayer->minScale();
      }
    }

    if (targetScale == 0.0)
    {
      targetScale = DEFAULT_TARGET_SCALE;
    }

    if (observedViewpoint.targetScale() > targetScale)
    {
      if (m_automaticSelectionMode == AutomaticSelectionMode::Always)
      {
        setSelectedLevelId({});
        setSelectedFacilityId({});
        setSelectedSiteId({});
      }
      // Assumption: if too zoomed out to see sites, also too zoomed out to see facilities
      return;
    }

    // If the centerpoint is within a site's geometry, select that site.
    // This code gracefully skips selection if there are no sites or no matching sites
    auto sites = floorManager ? floorManager->sites() : QList<FloorSite*>{};
    auto siteResult = std::find_if(std::cbegin(sites), std::cend(sites),
                                   [&observedViewpoint](FloorSite* site)
                                   {
                                     if (site == nullptr)
                                       return false;

                                     auto extent = site->geometry().extent();
                                     if (extent.isEmpty())
                                       return false;

                                     return GeometryEngine::intersects(extent, observedViewpoint.targetGeometry());
                                   });

    if (siteResult != std::cend(sites))
    {
      setSelectedSiteId((*siteResult)->siteId());
    }
    else if (m_automaticSelectionMode == AutomaticSelectionMode::Always)
    {
      setSelectedSiteId({});
    }

    // Move on to facility selection. Default to map-authored Facility MinScale. If MinScale not specified or is 0, default to 1500.
    targetScale = 0.0;
    if (floorManager)
    {
      if (auto siteLayer = floorManager->facilityLayer())
      {
        targetScale = siteLayer->minScale();
      }
    }

    if (targetScale == 0)
    {
      targetScale = 1500;
    }

    // If out of scale, stop here
    if (observedViewpoint.targetScale() > targetScale)
    {
      return;
    }

    auto facilities = floorManager ? floorManager->facilities() : QList<FloorFacility*>{};
    auto facilityResult = std::find_if(std::cbegin(facilities), std::cend(facilities),
                                       [&observedViewpoint](FloorFacility* facility)
                                       {
                                         if (facility == nullptr)
                                           return false;

                                         auto extent = facility->geometry().extent();
                                         if (extent.isEmpty())
                                           return false;

                                         return GeometryEngine::intersects(extent, observedViewpoint.targetGeometry());
                                       });

    if (facilityResult != std::cend(facilities))
    {
      setSelectedFacilityId((*facilityResult)->facilityId());
    }
    else if (m_automaticSelectionMode == AutomaticSelectionMode::Always)
    {
      setSelectedFacilityId({});
    }
  }

  void FloorFilterController::zoomToEnvelope(const Envelope& envelope)
  {
    if (envelope.isEmpty() || !m_geoView)
    {
      return;
    }

    EnvelopeBuilder b{envelope};
    b.expandByFactor(ZOOM_PADDING);

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      m_settingViewpoint = true;
      mapView->setViewpointAsync(b.toEnvelope()).then(this, [this](bool success)
      {
        if (success)
        {
          m_settingViewpoint = false;
        }
      });
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      m_settingViewpoint = true;
      sceneView->setViewpointAsync(b.toEnvelope()).then(this, [this](bool success)
      {
        if (success)
        {
          m_settingViewpoint = false;
        }
      });
    }
  }
} // Esri::ArcGISRuntime::Toolkit
