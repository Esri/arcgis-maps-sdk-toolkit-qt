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
    connect(this, &FloorFilterController::selectedFacilityIdChanged, this, &FloorFilterController::selectedChanged);
    connect(this, &FloorFilterController::selectedLevelIdChanged, this, &FloorFilterController::selectedChanged);
    connect(this, &FloorFilterController::selectedSiteIdChanged, this, &FloorFilterController::selectedChanged);

    connect(this, &FloorFilterController::selectedFacilityIdChanged, this, &FloorFilterController::populateLevelsForSelectedFacility);
    connect(this, &FloorFilterController::selectedSiteIdChanged, this, &FloorFilterController::populateFacilitiesForSelectedSite);
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
    if (m_selectedLevelId == selectedFacilityId)
      return;

    m_selectedFacilityId = std::move(selectedFacilityId);
    emit selectedFacilityIdChanged();
  }

  QString FloorFilterController::selectedLevelId() const
  {
    return m_selectedLevelId;
  }

  void FloorFilterController::setSelectedLevelId(QString selectedLevelId)
  {
    if (m_selectedLevelId == selectedLevelId)
      return;

    m_selectedLevelId = std::move(selectedLevelId);
    emit selectedLevelIdChanged();
  }

  QString FloorFilterController::selectedSiteId() const
  {
    return m_selectedSiteId;
  }

  void FloorFilterController::setSelectedSiteId(QString selectedSiteId)
  {
    if (m_selectedSiteId == selectedSiteId)
      return;

    m_selectedSiteId = std::move(selectedSiteId);
    emit selectedSiteIdChanged();
  }

  FloorFacility* FloorFilterController::selectedFacility() const
  {
    if (auto floorManager = getFloorManager(m_geoView))
    {
      auto facilities = floorManager->facilities();
      auto it = std::find_if(std::cbegin(facilities),
                             std::cend(facilities),
                             [this](FloorFacility* facility) -> bool
                             {
                               return facility && facility->facilityId() == selectedFacilityId();
                             });
      if (it != std::cend(facilities))
      {
        return *it;
      }
    }
    return nullptr;
  }

  FloorSite* FloorFilterController::selectedSite() const
  {
    if (auto floorManager = getFloorManager(m_geoView))
    {
      auto sites = floorManager->sites();
      auto it = std::find_if(std::cbegin(sites),
                             std::cend(sites),
                             [this](FloorSite* site) -> bool
                             {
                               return site && site->siteId() == selectedSiteId();
                             });
      if (it != std::cend(sites))
      {
        return *it;
      }
    }
    return nullptr;
  }

  FloorLevel* FloorFilterController::selectedLevel() const
  {
    if (auto floorManager = getFloorManager(m_geoView))
    {
      auto levels = floorManager->levels();
      auto it = std::find_if(std::cbegin(levels),
                             std::cend(levels),
                             [this](FloorLevel* level) -> bool
                             {
                               return level && level->levelId() == selectedLevelId();
                             });
      if (it != std::cend(levels))
      {
        return *it;
      }
    }
    return nullptr;
  }

  void FloorFilterController::populateLevelsForSelectedFacility()
  {
    m_levels->clear();
    auto manager = getFloorManager(m_geoView);
    if (!manager)
      return;

    const auto allLevels = manager->levels();

    QList<QObject*> levelItems;
    for (const auto level : allLevels)
    {
      if (level->facility()->facilityId() == selectedFacilityId())
      {
        levelItems << new FloorFilterLevelItem(level, m_levels);
      }
    }
    //' Reverse order for ascending order.
    m_levels->append(QList<QObject*>(std::crbegin(levelItems), std::crend(levelItems)));
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
      if (facility->site()->siteId() == selectedSiteId())
      {
        facilityItems << new FloorFilterFacilityItem(facility, m_facilities);
      }
    }
    m_facilities->append(facilityItems);
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
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
