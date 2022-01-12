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
#include "Internal/GeoViews.h"
#include "Internal/DoOnLoad.h"


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

    if (m_geoView)
    {
      if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        connect(mapView, &MapViewToolkit::mapChanged, this,
                [this, mapView]
                {
                  auto map = mapView->map();
                  doOnLoad(map, this, [this, map]
                           {
                             auto floorManager = map->floorManager();
                             doOnLoad(floorManager, this, [this]
                                      {
                                        populateSites();
                                      });
                           });
                });
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        connect(sceneView, &SceneViewToolkit::sceneChanged, this,
                [this, sceneView]
                {
                  auto scene = sceneView->arcGISScene();
                  doOnLoad(scene, this, [this, scene]()
                           {
                             auto floorManager = scene->floorManager();
                             doOnLoad(floorManager, this, [this]
                                      {
                                        populateSites();
                                      });
                           });
                });
      }
    }
    emit geoViewChanged();
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
