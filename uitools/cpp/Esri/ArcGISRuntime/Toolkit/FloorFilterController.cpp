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

// ArcGISRuntime headers
#include <Envelope.h>
#include <EnvelopeBuilder.h>
#include <FloorFacility.h>
#include <FloorLevel.h>
#include <FloorManager.h>
#include <FloorSite.h>
#include <GeometryEngine.h>
#include <Map.h>
#include <Scene.h>

// Toolkit headers
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "Internal/DisconnectOnSignal.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"
#include "Internal/SingleShotConnection.h"

// stl headers
#include <cmath>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    /*!
     \internal
     \brief Constant padding value for the extent when zooming
            to the geometry of a FloorFacility.
     */
    const float ZOOM_PADDING = 1.5;

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
          return;

        // Here we attempt to calls `f` if/when both the GeoModel and FloorManager are loaded.
        // This may happen immediately or asyncnronously.This can be interrupted if GeoView or
        // GeoModel changes in the interim.
        auto c = doOnLoad(model, self, [self, model, geoView, f = std::move(f)]()
                          {
                            auto floorManager = model->floorManager();
                            if (!floorManager)
                              return;

                            auto c = doOnLoad(floorManager, self, [f = std::move(f)]
                                              {
                                                f();
                                              });
                            // Destroy the connection `c` if the map/scene changes, or the geoView changes.
                            // This means the connection is only relevant for as long as the model/view is relavant to
                            // the FloorFilterController.
                            disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
                            disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c);
                          });
        // Destroy the connection `c` if the map/scene changes, or the geoView changes. This means
        // the connection is only relevant for as long as the model/view is relavant to the FloorFilterController.
        disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
        disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c);
      };

      // Hooks up to any geoModels that appear when the map/sceneView changed signal is called.
      QObject::connect(geoView, getGeoModelChangedSignal(geoView), self, connectToGeoModel);
      connectToGeoModel();

      // Hook up to any viewpoint changes on the GeoView.
      auto c = QObject::connect(geoView, &std::remove_pointer<decltype(geoView)>::type::viewpointChanged,
                                self, &FloorFilterController::tryUpdateSelection);
      disconnectOnSignal(self, &FloorFilterController::geoViewChanged, self, c);
    }
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilterController
    \brief The controller part of a FloorFilter. This class handles the
    visibility of levels, and listening to changes to the current
    viewpoint for automatically selecting a level.
   */

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \enum Esri::ArcGISRuntime::Toolkit::FloorFilterController::UpdateLevelsMode
    \brief The mode that defines how levels are made visible/invisible in the geoView.
    \sa Esri::ArcGISRuntime::Toolkit::FloorFilterController::updateLevelsMode
    \value AllLevelsMatchingVerticalOrder When a level is selected, all levels with matching vertical order are made visible, all other levels are invisible.
  */

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \enum Esri::ArcGISRuntime::Toolkit::FloorFilterController::AutomaticSelectionMode
    \brief The mode that defines how facilities are selected by viewpoint navigation.
    \sa Esri::ArcGISRuntime::Toolkit::FloorFilterController::automaticSelectionMode
    \value Never Viewpoint navigation does not affect the currently selected facility.
    \value Always When the geoview's current viewpoint updates, the controller tests to see if the facility interect the viewpoint,
           and selects it if so. If no facility intersects the viewpoint, then the current facility is deselected.
    \value AlwaysNonClearing Variant of \c Always, but if there is no facility to select within the viewpoint then we do not
           deselect the current viewpoint.
  */

  /*!
    \brief Constructs a new controller object with a given \a parent.
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

  /*!
   \brief Destructor.
   */
  FloorFilterController::~FloorFilterController()
  {
  }

  /*!
    \brief Returns the \c GeoView as a \c QObject.
   */
  QObject* FloorFilterController::geoView() const
  {
    return m_geoView;
  }

  /*!
   \brief Returns the levels in the currently selected facility.
   \sa selectedFacilityId
   */
  GenericListModel* FloorFilterController::levels() const
  {
    return m_levels;
  }

  /*!
   \brief Returns the site in the current \c GeoView's FloorManager.
   \sa geoView
   */
  GenericListModel* FloorFilterController::sites() const
  {
    return m_sites;
  }

  /*!
   \brief Returns the facilities in the currently selected site.
   \sa selectedSiteId
   */
  GenericListModel* FloorFilterController::facilities() const
  {
    return m_facilities;
  }

  /*!
  \brief Set the GeoView object this Controller uses.

  Internally this is cast to a \c MapView or \c SceneView using \c qobject_cast,
      which is why the paremeter is of form \c QObject and not \c GeoView.

  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and
        \c{QObject*}.
  \endlist
                                                                             */
  void FloorFilterController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

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

  /*!
   \brief Returns the ID of the currently selected facility.
   */
  QString FloorFilterController::selectedFacilityId() const
  {
    return m_selectedFacilityId;
  }

  /*!
   \brief Sets the ID of the currently selected facility to \a selectedFacilityId.

   This will change the contents of \l levels to all levels within the facility, and
   reset \l selectedLevelId.
   */
  void FloorFilterController::setSelectedFacilityId(QString selectedFacilityId)
  {
    if (m_selectedFacilityId == selectedFacilityId)
      return;
    QString oldId = m_selectedFacilityId;
    m_selectedFacilityId = std::move(selectedFacilityId);
    emit selectedFacilityIdChanged(std::move(oldId), m_selectedFacilityId);
  }

  /*!
   \brief Returns the ID of the currently selected level.
   */
  QString FloorFilterController::selectedLevelId() const
  {
    return m_selectedLevelId;
  }

  /*!
   \brief Sets the ID of the currently selected level to \a selectedLevelId.

   This will change the current visibile level on the GeoView.
   */
  void FloorFilterController::setSelectedLevelId(QString selectedLevelId)
  {
    if (m_selectedLevelId == selectedLevelId)
      return;

    QString oldId = m_selectedLevelId;
    m_selectedLevelId = std::move(selectedLevelId);
    emit selectedLevelIdChanged(std::move(oldId), m_selectedLevelId);
  }

  /*!
   \brief Returns the ID of the currently selected site.
   */
  QString FloorFilterController::selectedSiteId() const
  {
    return m_selectedSiteId;
  }

  /*!
   \brief Sets the ID of the currently selected site to \a selectedSiteId.

   This will change the contents of \l facilities to all facilities within the site, and
   reset \l selectedFacilityId.
   */
  void FloorFilterController::setSelectedSiteId(QString selectedSiteId)
  {
    if (m_selectedSiteId == selectedSiteId)
      return;

    QString oldId = m_selectedSiteId;
    m_selectedSiteId = std::move(selectedSiteId);
    emit selectedSiteIdChanged(std::move(oldId), m_selectedSiteId);
  }

  /*!
   \internal
   \brief Clears the levels list, and repopulates with only those levels
   that match the currently selected facility id. Levels are sorted in terms
   of their vertical order, and a default level is selected, favouring the
   level with verticalOrder == 0.
   */
  void FloorFilterController::populateLevelsForSelectedFacility()
  {
    m_levels->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
      return;

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
    for (const auto level : qAsConst(allLevels))
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

  /*!
   \internal
   \brief Clears the facilities list and repopulates with the facilites that have a matching
   siteID to \l selectedSiteId.

   If only on facility exists we select that facility, otherwise we do not automatically select
   a facility.

   Note that `sites` are optional in the data, and it may be the case that ther are no sites,
   in which case we popualte with all available facilities.

   It may also be the case that `m_selectedSiteRespected` is false, in which case we extract all
   facilities and ignore the currently selected site in terms of filtering.
   */
  void FloorFilterController::populateFacilitiesForSelectedSite()
  {
    auto manager = getFloorManager(m_geoView);
    if (!manager)
    {
      m_facilities->clear();
      return;
    }

    // Do not blow out the facilities list with a new list if we are showing all facilities in the
    // FloorManager and we are in ignore selected-site mode. This is not just a micro-optimziation,
    // there are cases where selecting a facility in this mode will select a parent site, and we don't
    // want to regenerate the facilities list when the selected site changes.
    if (manager->facilities().length() == m_facilities->rowCount() && !m_selectedSiteRespected)
      return;

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

  /*!
   \internal
   \brief Populates the sites list with all sites from the floorManager.

   If only one site exists in the model, then we select it automatically.
   */
  void FloorFilterController::populateSites()
  {
    m_sites->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
      return;

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

  /*!
   \brief On the GeoView, zooms to the facility contained within \a facilityItem.
   */
  void FloorFilterController::zoomToFacility(FloorFilterFacilityItem* facilityItem)
  {
    if (!facilityItem)
      return;

    const auto f = facilityItem->floorFacility();
    if (f)
    {
      zoomToEnvelope(f->geometry().extent());
    }
  }

  /*!
   \brief On the GeoView, zooms to the facility with ID matching \a facilityId.
   */
  void FloorFilterController::zoomToFacility(const QString& facilityId)
  {
    zoomToFacility(facility(facilityId));
  }

  /*!
   \brief On the GeoView, zooms to the site contained within \a siteItem.
   */
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

  /*!
   \brief On the GeoView, zooms to the site with ID matching \a siteId.
   */
  void FloorFilterController::zoomToSite(const QString& siteId)
  {
    zoomToSite(site(siteId));
  }

  /*!
   \brief Returns the item with \c modelId matching \a facilityId by searching \l facilities.
   */
  FloorFilterFacilityItem* FloorFilterController::facility(const QString& facilityId) const
  {
    auto model = m_facilities;
    const auto rows = model->rowCount();
    for (int i = 0; i < rows; ++i)
    {
      auto index = model->index(i);
      auto item = model->element<FloorFilterFacilityItem>(index);
      if (item->modelId() == facilityId)
        return item;
    }
    return nullptr;
  }

  /*!
   \brief Returns the item with \c modelId matching \a siteId by searching \l sites.
   */
  FloorFilterSiteItem* FloorFilterController::site(const QString& siteId) const
  {
    auto model = m_sites;
    const auto rows = model->rowCount();
    for (int i = 0; i < rows; ++i)
    {
      auto index = model->index(i);
      auto item = model->element<FloorFilterSiteItem>(index);
      if (item->modelId() == siteId)
        return item;
    }
    return nullptr;
  }

  /*!
   \brief Returns the item with \c modelId matching \a levelId by searching \l levels.
   */
  FloorFilterLevelItem* FloorFilterController::level(const QString& levelId) const
  {
    auto model = m_levels;
    const auto rows = model->rowCount();
    for (int i = 0; i < rows; ++i)
    {
      auto index = model->index(i);
      auto item = model->element<FloorFilterLevelItem>(index);
      if (item->modelId() == levelId)
        return item;
    }
    return nullptr;
  }

  /*!
   \internal
   */
  FloorFilterFacilityItem* FloorFilterController::selectedFacility() const
  {
    return facility(selectedFacilityId());
  }

  /*!
   \internal
   */
  FloorFilterSiteItem* FloorFilterController::selectedSite() const
  {
    return site(selectedSiteId());
  }

  /*!
   \internal
   */
  FloorFilterLevelItem* FloorFilterController::selectedLevel() const
  {
    return level(selectedLevelId());
  }

  /*!
   \brief Returns whether the selected site is respected.

   When the selected site is respected, the \l facilities list is limited to the facilities
   which match the selected site. If the selected site is not respected, then \l facilities
   will contain all facilities from all sites, regardless of the selected site.
   */
  bool FloorFilterController::isSelectedSiteRespected() const
  {
    return m_selectedSiteRespected;
  }

  /*!
   \brief Sets the \l isSelectedSiteRespected flag to \a isSelectedSiteRespected.

   This will repopulate the \l facilities model.
   */
  void FloorFilterController::setIsSelectedSiteRespected(bool isSelectedSiteRespected)
  {
    if (isSelectedSiteRespected == m_selectedSiteRespected)
      return;

    m_selectedSiteRespected = isSelectedSiteRespected;
    emit isSelectedSiteRespectedChanged();
  }

  /*!
   \brief Returns the current level update mode in use.

   Defaults to \c{FloorFilterController::UpdateLevelsMode::AllLevelsMatchingVerticalOrder}.
   */
  FloorFilterController::UpdateLevelsMode FloorFilterController::updateLevelsMode() const
  {
    return m_updatelevelMode;
  }

  /*!
   \brief Sets the current level update mode in use to \a updateLevelsMode.

   This will not take effect until a selection is made.
   */
  void FloorFilterController::setUpdateLevelsMode(UpdateLevelsMode updateLevelsMode)
  {
    if (updateLevelsMode == m_updatelevelMode)
      return;

    m_updatelevelMode = updateLevelsMode;
    emit updateLevelsModeChanged();
  }

  /*!
    \brief Returns the current level update mode in use.

    Defaults to \c{FloorFilterController::AutomaticSelectionMode::Always}.
   */
  FloorFilterController::AutomaticSelectionMode FloorFilterController::automaticSelectionMode() const
  {
    return m_automaticSelectionMode;
  }

  /*!
   \brief Sets the current automatic selection mode in use to \a automaticSelectionMode.

   This will not take effect until a viewpoint navigation occurs.
   */
  void FloorFilterController::setAutomaticSelectionMode(AutomaticSelectionMode automaticSelectionMode)
  {
    if (m_automaticSelectionMode == automaticSelectionMode)
      return;

    m_automaticSelectionMode = automaticSelectionMode;
    emit automaticSelectionModeChanged();
  }

  /*!
   * \internal
   * \brief Attempts to select a facility based on the current viewpoint geometry.
   */
  void FloorFilterController::tryUpdateSelection()
  {
    if (m_automaticSelectionMode == AutomaticSelectionMode::Never)
      return;

    if (m_settingViewpoint)
      return;

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
        isnan(observedViewpoint.targetScale()))
      return;

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
      targetScale = 4300.0;

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

  /*!
      \internal
      \brief Zooms the given \a geoView to \a envelope, applying `ZOOM_PADDING`.
     */
  void FloorFilterController::zoomToEnvelope(const Envelope& envelope)
  {
    if (envelope.isEmpty() || !m_geoView)
      return;

    EnvelopeBuilder b{envelope};
    b.expandByFactor(ZOOM_PADDING);

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      m_settingViewpoint = true;
      singleShotConnection(mapView, &MapViewToolkit::setViewpointCompleted, this, [this](bool /*success*/)
                           {
                             m_settingViewpoint = false;
                           });
      mapView->setViewpoint(b.toEnvelope());
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      m_settingViewpoint = true;
      singleShotConnection(sceneView, &SceneViewToolkit::setViewpointCompleted, this, [this](bool /*success*/)
                           {
                             m_settingViewpoint = false;
                           });
      sceneView->setViewpoint(b.toEnvelope());
    }
  }

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::geoViewChanged()
  \brief Emitted when the geoView has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedSiteIdChanged(QString oldId, QString newId)
  \brief Emitted when the selectedSiteId has changed from \a oldId to \a newId.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedFacilityIdChanged(QString oldId, QString newId)
  \brief Emitted when the selectedFacilityId has changed from \a oldId to \a newId.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedLevelIdChanged(QString oldId, QString newId)
  \brief Emitted when the selectedLevelId has changed from \a oldId to \a newId.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::isSelectedSiteRespectedChanged()
  \brief Emitted when the isSelectedSiteRespected flag has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::updateLevelsModeChanged()
  \brief Emitted when the updateLevelsMode flag has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterController::automaticSelectionModeChanged()
  \brief Emitted when the automaticSelectionModeChanged flag has changed.
 */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::geoView
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedSiteRespected
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedFacility
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedFacilityId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedLevel
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedLevelId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedSite
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::selectedSiteId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::levels
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::facilities
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::sites
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::updateLevelsMode
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterController::automaticSelectionMode
   */
} // Toolkit
} // ArcGISRuntime
} // Esri
