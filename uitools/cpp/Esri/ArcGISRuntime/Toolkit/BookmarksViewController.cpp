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
#include "BookmarksViewController.h"

// Toolkit headers
#include "BookmarkListItem.h"
#include "Internal/DisconnectOnSignal.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"

// Qt headers
#include <QtGlobal>

// ArcGISRuntime headers
#include <Bookmark.h>
#include <Map.h>
#include <Scene.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    void setupBookmarks(BookmarkListModel* sourceModel, GenericListModel* targetModel)
    {
      QObject::connect(sourceModel, &BookmarkListModel::rowsInserted, targetModel,
                       [sourceModel, targetModel](const QModelIndex& parent, int first, int last)
                       {
                         if (parent.isValid())
                           return;

                         if (!targetModel->insertRows(first, last - first + 1))
                           return;

                         for (auto i = first; i <= last; ++i)
                         {
                           QModelIndex j = targetModel->index(i);
                           auto targetItem = targetModel->element<BookmarkListItem>(j);
                           if (targetItem)
                             targetItem->setBookmark(sourceModel->at(i));
                         }
                       });

      QObject::connect(sourceModel, &BookmarkListModel::rowsRemoved, targetModel,
                       [targetModel](const QModelIndex& parent, int first, int last)
                       {
                         if (parent.isValid())
                           return;

                         targetModel->removeRows(first, last - first + 1);
                       });

      QObject::connect(sourceModel, &BookmarkListModel::rowsMoved, targetModel,
                       [targetModel](const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
                       {
                         if (parent.isValid() || destination.isValid())
                           return;

                         targetModel->moveRows(QModelIndex{}, start, end - start + 1, QModelIndex{}, row);
                       });

      QList<QObject*> targetItems;
      for (int i = 0; i < sourceModel->size(); ++i)
      {
        targetItems << new BookmarkListItem(sourceModel->at(i), targetModel);
      }
      targetModel->append(targetItems);
    }

    /*!
     \internal
     \brief Manages the connection between Controller \a self and GeoView \a geoView.
     Attempts to call functor `f` if/when the Bookmark within the geoModel is loaded.
     This may also cause the geoModel itself to load.
     Will continue to call `f` every time a mapChanged/sceneChanged signal is triggered on
     the GeoView.
     */
    template <typename GeoViewToolkit, typename Func>
    void connectToGeoView(GeoViewToolkit* geoView, BookmarksViewController* self, Func&& f)
    {
      static_assert(
          std::is_same<GeoViewToolkit, MapViewToolkit>::value ||
              std::is_same<GeoViewToolkit, SceneViewToolkit>::value,
          "Must be connected to a SceneView or MapView");

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

        // Destroy the connection `c` if the map/scene changes, or the geoView changes. This means
        // the connection is only relevant for as long as the model/view is relavant to the BookmarksViewController.
        disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
        disconnectOnSignal(self, &BookmarksViewController::geoViewChanged, self, c);
      };

      // Hooks up to any geoModels that appear when the mapView/sceneView changed signal is called.
      QObject::connect(geoView, getGeoModelChangedSignal(geoView), self, connectToGeoModel);
      connectToGeoModel();
    }
  }

  /*!
  \class Esri::ArcGISRuntime::Toolkit::BookmarksViewController
  \ingroup ArcGISQtToolkitUiCppControllers
  \inmodule EsriArcGISRuntimeToolkit

  \brief In MVC architecture, this is the controller for the corresponding
  \c Bookmark view.
  This class handles the management of the BookmarkListItem objects, and
  displays the current bookmark.
 */

  /*!
  \brief Constructor.
  \list
    \li \a parent owning parent object.
  \endlist
 */
  BookmarksViewController::BookmarksViewController(QObject* parent) :
    QObject(parent),
    m_bookmarks(new GenericListModel(&BookmarkListItem::staticMetaObject, this))
  {
    m_bookmarks->setDisplayPropertyName("name");
  }

  /*!
  \brief Destructor
 */
  BookmarksViewController::~BookmarksViewController() = default;

  /*!
  \brief Returns the \c GeoView as a \c QObject.
 */
  QObject* BookmarksViewController::geoView() const
  {
    return m_geoView;
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
  void BookmarksViewController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);

      if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        auto map = mapView->map();

        if (map && map->bookmarks())
          disconnect(map->bookmarks(), nullptr, m_bookmarks, nullptr);
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        auto scene = sceneView->arcGISScene();

        if (scene && scene->bookmarks())
          disconnect(scene->bookmarks(), nullptr, m_bookmarks, nullptr);
      }

      m_bookmarks->clear();
    }

    m_geoView = geoView;

    // Important that this emit happens before the below connections,
    // as this emit will destroy the connections set up below.
    emit geoViewChanged();

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connect(mapView, &MapViewToolkit::mapChanged, this, [this]()
              {
                m_bookmarks->clear();
              });

      // `connectToGeoView` guarantees the map and/or scene exists as it is only invoked once the geomodel is loaded.
      connectToGeoView(mapView, this, [this, mapView]
                       {
                         setupBookmarks(mapView->map()->bookmarks(), m_bookmarks);
                       });
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      connect(sceneView, &SceneViewToolkit::sceneChanged, this, [this]()
              {
                m_bookmarks->clear();
              });

      connectToGeoView(sceneView, this, [this, sceneView]
                       {
                         setupBookmarks(sceneView->arcGISScene()->bookmarks(), m_bookmarks);
                       });
    }
  }

  /*!
  \brief Returns the known list of available bookmarks which
  can be textually displayed.

  Internally, this is a \c GenericListModel with an \c elementType of
  \c Bookmark.
 */
  GenericListModel* BookmarksViewController::bookmarks() const
  {
    return m_bookmarks;
  }

  /*!
  \brief Updates the \c GeoView camera to point to the current bookmark's
  location on the map.
 */
  void BookmarksViewController::zoomToBookmarkExtent(BookmarkListItem* bookmark)
  {
    if (!bookmark)
      return;

    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      sceneView->setBookmark(bookmark->bookmark());
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      mapView->setBookmark(bookmark->bookmark());
    }
  }

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::BookmarksViewController::geoViewChanged()
  \brief Emitted when the geoView has changed.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::BookmarksViewController::geoView
  \brief The geoView the controller is utilizing for interactions.
  \sa Esri::ArcGISRuntime::Toolkit::BookmarksViewController::geoView() const
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
