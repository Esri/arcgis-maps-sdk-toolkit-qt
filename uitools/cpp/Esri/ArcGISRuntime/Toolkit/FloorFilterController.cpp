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

// Toolkit headers
#include "FloorFilterFacilityItem.h"
#include "FloorFilterLevelItem.h"
#include "FloorFilterSiteItem.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"
#include "Internal/SingleShotConnection.h"

// ArcGISRuntime headers
#include <Envelope.h>
#include <EnvelopeBuilder.h>
#include <FloorFacility.h>
#include <FloorLevel.h>
#include <FloorManager.h>
#include <FloorSite.h>
#include <Map.h>
#include <Scene.h>

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
      \brief Zooms the given \a geoView to \a envelope, applying `ZOOM_PADDING`.
     */
    void zoomToEnvelope(const Envelope& envelope, QObject* geoView)
    {
      if (envelope.isEmpty() || !geoView)
        return;

      EnvelopeBuilder b{envelope};
      b.expandByFactor(ZOOM_PADDING);

      if (auto mapView = qobject_cast<MapViewToolkit*>(geoView))
      {
        mapView->setViewpoint(b.toEnvelope());
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(geoView))
      {
        sceneView->setViewpoint(b.toEnvelope());
      }
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
     \brief When \a signal fires on \a sender, the given \a connection is disconnected.
     This makes the connection's invvocation depdendent on \a signal not firing.
     */
    template <typename Sender, typename Signal>
    QMetaObject::Connection disconnectOnSignal(Sender* sender, Signal&& signal, QObject* self, QMetaObject::Connection connection)
    {
      if (!connection)
        return QMetaObject::Connection{};

      return singleShotConnection(sender, signal, self, [c = std::move(connection)]
                                  {
                                    QObject::disconnect(c);
                                  });
    }

    /*!
     * \internal
     * \brief Manages the connection between Controller \a self and GeoView \a geoView.
     * Attempts to call functor `f` if/when the FloorFilter within the geoModel is loaded.
     * This may also cause the geoModel itself to load.
     * Will continue to call `f` every time a map/sceneChanged signal is triggered on
     * the GeoView.
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
    }
  }

  FloorFilterController::FloorFilterController(QObject* parent) :
    QObject(parent),
    m_levels(new GenericListModel(&FloorFilterLevelItem::staticMetaObject, this)),
    m_facilities(new GenericListModel(&FloorFilterFacilityItem::staticMetaObject, this)),
    m_sites(new GenericListModel(&FloorFilterSiteItem::staticMetaObject, this))
  {
    m_sites->setDisplayPropertyName("name");
    m_facilities->setDisplayPropertyName("name");
    m_levels->setDisplayPropertyName("longName");

    connect(this, &FloorFilterController::selectedFacilityIdChanged, this, &FloorFilterController::selectedChanged);
    connect(this, &FloorFilterController::selectedLevelIdChanged, this, &FloorFilterController::selectedChanged);
    connect(this, &FloorFilterController::selectedSiteIdChanged, this, &FloorFilterController::selectedChanged);

    connect(this, &FloorFilterController::selectedFacilityIdChanged, this, &FloorFilterController::populateLevelsForSelectedFacility);

    connect(this, &FloorFilterController::selectedSiteIdChanged, this, &FloorFilterController::populateFacilitiesForSelectedSite);
    connect(this, &FloorFilterController::isSelectedSiteRespectedChanged, this, &FloorFilterController::populateFacilitiesForSelectedSite);

    connect(this, &FloorFilterController::selectedLevelIdChanged, this,
            [this](QString /*oldId*/, QString newId)
            {
              auto newLevelItem = level(newId);
              auto newLevel = newLevelItem ? newLevelItem-> floorLevel() : nullptr;
              qDebug() << "NEW_LEVEL:" << (newLevel ? newLevel->longName() : "NONE");

              auto floorManager = getFloorManager(m_geoView);
              if (floorManager)
              {
                const auto levels = floorManager->levels();
                for (const auto level : levels)
                {
                  if (level)
                  {
                    level->setVisible(newLevel ? level->verticalOrder() == newLevel->verticalOrder() : false);
                  }
                }
              }
            });
  }

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

  QString FloorFilterController::selectedFacilityId() const
  {
    return m_selectedFacilityId;
  }

  void FloorFilterController::setSelectedFacilityId(QString selectedFacilityId)
  {
    if (m_selectedFacilityId == selectedFacilityId)
      return;

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
      return;

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
      return;

    QString oldId = m_selectedSiteId;
    m_selectedSiteId = std::move(selectedSiteId);
    emit selectedSiteIdChanged(std::move(oldId), m_selectedSiteId);
  }

  void FloorFilterController::populateLevelsForSelectedFacility()
  {
    m_levels->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
      return;

    const auto allLevels = manager->levels();

    QString defaultLevel{};
    QList<QObject*> levelItems;
    for (const auto level : allLevels)
    {
      if (level->facility()->facilityId() == selectedFacilityId())
      {
        levelItems << new FloorFilterLevelItem(level, m_levels);
        if (level->verticalOrder() == 0)
        {
          // Default level is level with vertical order 0.
          defaultLevel = level->levelId();
        }
      }
    }

    // Otherwise, if there is no appropriate vertical order, we use the bottom
    // level as the default level.
    if (defaultLevel.isEmpty() && levelItems.size() > 0)
    {
      defaultLevel = static_cast<FloorFilterLevelItem*>(levelItems.last())->modelId();
    }

    // Reverse order in controller for ascending order. Such that bottom floor level is 
    // at bottom of the list.
    m_levels->append(QList<QObject*>(std::crbegin(levelItems), std::crend(levelItems)));

    setSelectedLevelId(defaultLevel);
  }

  void FloorFilterController::populateFacilitiesForSelectedSite()
  {
    m_facilities->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
      return;

    const auto allFacilites = manager->facilities();

    QList<QObject*> facilityItems;
    for (const auto facility : allFacilites)
    {
      // If we have no sites take everything, otherwise filter by the selected site.
      if (!m_selectedSiteResepected || manager->sites().isEmpty() || facility->site()->siteId() == selectedSiteId())
      {
        facilityItems << new FloorFilterFacilityItem(facility, m_facilities);
      }
    }
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
    }
  }

  void FloorFilterController::zoomToFacility(FloorFilterFacilityItem* facilityItem)
  {
    if (!facilityItem)
      return;

    const auto f = facilityItem->floorFacility();
    if (f)
      zoomToEnvelope(f->geometry().extent(), m_geoView);
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
      zoomToEnvelope(s->geometry().extent(), m_geoView);
  }

  void FloorFilterController::zoomToSite(const QString& siteId)
  {
    zoomToSite(site(siteId));
  }

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
    return m_selectedSiteResepected;
  }

  void FloorFilterController::setIsSelectedSiteRespected(bool isSelectedSiteRespected)
  {
    if (isSelectedSiteRespected == m_selectedSiteResepected)
      return;

    m_selectedSiteResepected = isSelectedSiteRespected;
    emit isSelectedSiteRespectedChanged();
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
