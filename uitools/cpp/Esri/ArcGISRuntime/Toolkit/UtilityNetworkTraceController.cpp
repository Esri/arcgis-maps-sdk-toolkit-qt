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
#include "UtilityNetworkTraceController.h"

// Toolkit headers
#include "UtilityNetworkListItem.h"
#include "Internal/DisconnectOnSignal.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"

// Qt headers
#include <QtGlobal>

// ArcGISRuntime headers
#include <Map.h>
#include <UtilityNetworkListModel.h>

// Toolkit headers
#include "UtilityNetworkTrace.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    void setupUtilityNetworks(UtilityNetworkListModel* sourceModel, GenericListModel* targetModel)
    {
      QObject::connect(sourceModel, &UtilityNetworkListModel::rowsInserted, targetModel,
                       [sourceModel, targetModel](const QModelIndex& parent, int first, int last)
                       {
                         if (parent.isValid())
                           return;

                         if (!targetModel->insertRows(first, last - first + 1))
                           return;

                         for (auto i = first; i <= last; ++i)
                         {
                           QModelIndex j = targetModel->index(i);
                           auto targetItem = targetModel->element<UtilityNetworkListItem>(j);
                           if (targetItem)
                             targetItem->setUtilityNetwork(sourceModel->at(i));
                         }
                       });

      QObject::connect(sourceModel, &UtilityNetworkListModel::rowsRemoved, targetModel,
                       [targetModel](const QModelIndex& parent, int first, int last)
                       {
                         if (parent.isValid())
                           return;

                         targetModel->removeRows(first, last - first + 1);
                       });

      QObject::connect(sourceModel, &UtilityNetworkListModel::rowsMoved, targetModel,
                       [targetModel](const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
                       {
                         if (parent.isValid() || destination.isValid())
                           return;

                         targetModel->moveRows(QModelIndex{}, start, end - start + 1, QModelIndex{}, row);
                       });

      QList<QObject*> targetItems;
      for (int i = 0; i < sourceModel->size(); ++i)
      {
        targetItems << new UtilityNetworkListItem(sourceModel->at(i), targetModel);
      }
      targetModel->append(targetItems);
    }

    /*!
     \internal
     \brief Manages the connection between Controller \a self and GeoView \a geoView.
     Attempts to call functor `f` if/when the UtilityNetwork within the geoModel is loaded.
     This may also cause the geoModel itself to load.
     Will continue to call `f` every time a mapChanged signal is triggered on
     the GeoView.
     */
    template <typename GeoViewToolkit, typename Func>
    void connectToGeoView(GeoViewToolkit* geoView, UtilityNetworkTraceController* self, Func&& f)
    {
      static_assert(
          std::is_same<GeoViewToolkit, MapViewToolkit>::value,
          "Must be connected to a MapView");

      auto connectToGeoModel = [self, geoView, f = std::forward<Func>(f)]
      {
        auto model = getGeoModel(geoView);
        if (!model)
        {
          return;
        }

        // Here we attempt to calls `f` if/when the GeoModel is loaded.
        // This may happen immediately or asyncnronously. This can be interrupted if GeoView or
        // GeoModel changes in the interim.
        auto c = doOnLoaded(model, self, [f = std::move(f)]()
                            {
                              f();
                            });

        // Destroy the connection `c` if the map changes, or the geoView changes. This means
        // the connection is only relevant for as long as the model/view is relavant to the UtilityNetworkTraceController.
        disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
        disconnectOnSignal(self, &UtilityNetworkTraceController::geoViewChanged, self, c);
      };

      // Hooks up to any geoModels that appear when the mapView changed signal is called.
      QObject::connect(geoView, getGeoModelChangedSignal(geoView), self, connectToGeoModel);
      connectToGeoModel();
    }
  }

  /*!
  \class Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceController
  \ingroup ArcGISQtToolkitUiCppControllers
  \inmodule EsriArcGISRuntimeToolkit

  \brief In MVC architecture, this is the controller for the corresponding
  \c UtilityNetwork trace.
  This class handles the management of the UtilityNetworkListItem objects, and
  displays the current UtilityNetwork.
 */

  /*!
  \brief Constructor.
  \list
    \li \a parent owning parent object.
  \endlist
 */
  UtilityNetworkTraceController::UtilityNetworkTraceController(QObject* parent) :
    QObject(parent),
    m_utilityNetworks(new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this))
  {
    // Let's add a progress indicator here
    qDebug() << "constructing UtilityNetworkTraceController";
    m_utilityNetworks->setDisplayPropertyName("name");

    QUrl featureServiceURL{"https://sampleserver7.arcgisonline.com/portal/sharing/rest"};
    m_portal = new Portal(featureServiceURL);
    m_portal->setCredential(new Credential("viewer01", "I68VGU^nMurF"));

    QObject::connect(m_portal,
                     &Portal::doneLoading,
                     this,
                     [this](Error)
                     {
                       if (!m_geoView)
                         qDebug() << "the geoview is null";
                       if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
                       {
                         QUrl webMapURL{"https://www.arcgis.com/home/item.html?id=471eb0bf37074b1fbb972b1da70fb310"};
                         Map* map = new Map(webMapURL);
                         mapView->setMap(map);

                         QObject::connect(map,
                                          &Map::doneLoading,
                                          this,
                                          [this, map, mapView](Error)
                                          {
                                            qDebug() << "map loaded";
                                            qDebug() << "un load status: " << static_cast<int>(map->utilityNetworks()->at(0)->loadStatus());
                                            qDebug() << "uns: " << map->utilityNetworks()->size();
                                            m_utilityNetwork = map->utilityNetworks()->at(0);
                                            QObject::connect(m_utilityNetwork,
                                                             &UtilityNetwork::doneLoading,
                                                             this,
                                                             [this, mapView](Error)
                                                             {
                                                               qDebug() << "Utility Network loaded";
                                                               // We can disable the progress indicator now
                                                               m_utilityNetworkTrace = new UtilityNetworkTrace(mapView);

                                                             });
                                            m_utilityNetwork->load();
                                          });
                         map->load();
                       }
                     });
    m_portal->load();
  }

  /*!
  \brief Destructor
 */
  UtilityNetworkTraceController::~UtilityNetworkTraceController() = default;

  /*!
  \brief Returns the \c GeoView as a \c QObject.
 */
  QObject* UtilityNetworkTraceController::geoView() const
  {
    return m_geoView;
  }

  /*!
  \brief Set the GeoView object this Controller uses.

  Internally this is cast to a \c MapView using \c qobject_cast,
  which is why the paremeter is of form \c QObject and not \c GeoView.

  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and
        \c{QObject*}.
  \endlist
*/
  void UtilityNetworkTraceController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);

      if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        auto map = mapView->map();

        if (map && map->utilityNetworks())
          disconnect(map->utilityNetworks(), nullptr, m_utilityNetworks, nullptr);
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        // scene does not have utility networks
        return;
      }

      m_utilityNetworks->clear();
    }

    m_geoView = geoView;

    // Important that this emit happens before the below connections,
    // as this emit will destroy the connections set up below.
    emit geoViewChanged();

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connect(mapView, &MapViewToolkit::mapChanged, this, [this]()
              {
                m_utilityNetworks->clear();
              });

      // `connectToGeoView` guarantees the map exists as it is only invoked once the geomodel is loaded.
      connectToGeoView(mapView, this, [this, mapView]
                       {
                         setupUtilityNetworks(mapView->map()->utilityNetworks(), m_utilityNetworks);
                       });
    }
  }

  /*!
  \brief Returns the known list of available utilityNetworks which
  can be textually displayed.

  Internally, this is a \c GenericListModel with an \c elementType of
  \c UtilityNetwork.
 */
  GenericListModel* UtilityNetworkTraceController::utilityNetworks() const
  {
    return m_utilityNetworks;
  }

  Portal* UtilityNetworkTraceController::portal() const
  {
    return m_portal;
  }

  void UtilityNetworkTraceController::setPortal(Portal* portal)
  {
    if (portal != m_portal)
      m_portal = portal;
  }

  /*!
  \brief Updates the \c GeoView camera to point to the current utilityNetwork's
  location on the map.
 */
  /*void UtilityNetworkTraceController::zoomToBookmarkExtent(BookmarkListItem* bookmark)
  {
    if (!bookmark)
      return;

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      mapView->setBookmark(bookmark->bookmark());
    }
  }*/

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceController::geoViewChanged()
  \brief Emitted when the geoView has changed.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceController::geoView
  \brief The geoView the controller is utilizing for interactions.
  \sa Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceController::geoView() const
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
