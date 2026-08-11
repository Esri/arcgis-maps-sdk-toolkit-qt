
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
#ifndef QRT_DISABLE_DEPRECATED_WARNINGS
#define QRT_DISABLE_DEPRECATED_WARNINGS
#endif
#include "BookmarksViewController.h"

// Toolkit headers
#include "BookmarkListItem.h"
#include "DisconnectOnSignal.h"
#include "DoOnLoad.h"
#include "GeoViews.h"

// Qt headers
#include <QFuture>
#include <QtGlobal>

// ArcGISRuntime headers
#include <Bookmark.h>
#include <BookmarkListModel.h>
#include <Map.h>
#include <Scene.h>

namespace Esri::ArcGISRuntime::Toolkit
{
  static void setupBookmarks(BookmarkListModel* sourceModel, GenericListModel* targetModel)
  {
    QObject::connect(sourceModel, &BookmarkListModel::rowsInserted, targetModel,
                     [sourceModel, targetModel](const QModelIndex& parent, int first, int last)
    {
      if (parent.isValid())
      {
        return;
      }

      if (!targetModel->insertRows(first, last - first + 1))
      {
        return;
      }

      for (auto i = first; i <= last; ++i)
      {
        QModelIndex j = targetModel->index(i);
        auto* targetItem = targetModel->element<BookmarkListItem>(j);
        if (targetItem)
        {
          targetItem->setBookmark(sourceModel->at(i));
        }
      }
    });

    QObject::connect(sourceModel, &BookmarkListModel::rowsRemoved, targetModel, [targetModel](const QModelIndex& parent, int first, int last)
    {
      if (parent.isValid())
      {
        return;
      }

      targetModel->removeRows(first, last - first + 1);
    });

    QObject::connect(sourceModel, &BookmarkListModel::rowsMoved, targetModel,
                     [targetModel](const QModelIndex& parent, int sourceRow, int end, const QModelIndex& destination, int destinationChild)
    {
      if (parent.isValid() || destination.isValid())
      {
        return;
      }

      targetModel->moveRows(QModelIndex{}, sourceRow, end - sourceRow + 1, QModelIndex{}, destinationChild);
    });

    QList<QObject*> targetItems;
    for (auto bookmark : *sourceModel)
    {
      targetItems << new BookmarkListItem(bookmark, targetModel);
    }
    targetModel->append(targetItems);
  }

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::BookmarksViewController
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  BookmarksViewController::BookmarksViewController(QObject* parent) :
    QObject(parent),
    m_bookmarks(new GenericListModel(&BookmarkListItem::staticMetaObject, this))
  {
    m_bookmarks->setDisplayPropertyName("name");
  }

  BookmarksViewController::~BookmarksViewController() = default;

  QObject* BookmarksViewController::geoView() const
  {
    return m_geoView;
  }

  void BookmarksViewController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
    {
      return;
    }

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);

      if (auto* mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        auto* map = mapView->map();

        if (map && map->bookmarks())
        {
          disconnect(map->bookmarks(), nullptr, m_bookmarks, nullptr);
        }
      }
      else if (auto* sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        auto* scene = sceneView->arcGISScene();

        if (scene && scene->bookmarks())
        {
          disconnect(scene->bookmarks(), nullptr, m_bookmarks, nullptr);
        }
      }
      else if (auto* localSceneView = qobject_cast<LocalSceneViewToolkit*>(m_geoView))
      {
        auto* scene = localSceneView->arcGISScene();

        if (scene && scene->bookmarks())
        {
          disconnect(scene->bookmarks(), nullptr, m_bookmarks, nullptr);
        }
      }

      m_bookmarks->clear();
    }

    m_geoView = geoView;

    // Important that this emit happens before the below connections,
    // as this emit will destroy the connections set up below.
    emit geoViewChanged();

    // Manages the connection between Controller \a self and GeoView \a geoView.
    // Attempts to call functor `f` if/when the Bookmark within the geoModel is loaded.
    // This may also cause the geoModel itself to load.
    // Will continue to call `f` every time a mapChanged/sceneChanged signal is triggered on
    // the GeoView.
    auto connectToGeoView = [this](auto* typedGeoView, auto&& f)
    {
      auto connectToGeoModel = [this, typedGeoView, f]()
      {
        auto model = getGeoModel(typedGeoView);
        if (!model)
        {
          return;
        }

        // Call `f` once the GeoModel is loaded.
        auto c = doOnLoaded(model, this, [f]()
        {
          f();
        });

        // Tear down if map/scene or GeoView changes.
        disconnectOnSignal(typedGeoView, getGeoModelChangedSignal(typedGeoView), this, c);
        disconnectOnSignal(this, &BookmarksViewController::geoViewChanged, this, c);
      };

      // Re-run when the map/scene changes.
      QObject::connect(typedGeoView, getGeoModelChangedSignal(typedGeoView), this, connectToGeoModel);
      connectToGeoModel();
    };

    if (auto* mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connect(mapView, &MapViewToolkit::mapChanged, this, [this]()
      {
        m_bookmarks->clear();
      });

      // `connectToGeoView` guarantees the map and/or scene exists as it is only invoked once the geomodel is loaded.
      connectToGeoView(mapView, [this, mapView]
      {
        setupBookmarks(mapView->map()->bookmarks(), m_bookmarks);
      });
    }
    else if (auto* sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      connect(sceneView, &SceneViewToolkit::sceneChanged, this, [this]()
      {
        m_bookmarks->clear();
      });

      connectToGeoView(sceneView, [this, sceneView]
      {
        setupBookmarks(sceneView->arcGISScene()->bookmarks(), m_bookmarks);
      });
    }
    else if (auto* localSceneView = qobject_cast<LocalSceneViewToolkit*>(m_geoView))
    {
      connect(localSceneView, &LocalSceneViewToolkit::sceneChanged, this, [this]()
      {
        m_bookmarks->clear();
      });

      connectToGeoView(localSceneView, [this, localSceneView]
      {
        setupBookmarks(localSceneView->arcGISScene()->bookmarks(), m_bookmarks);
      });
    }
  }

  GenericListModel* BookmarksViewController::bookmarks() const
  {
    return m_bookmarks;
  }

  void BookmarksViewController::zoomToBookmarkExtent(BookmarkListItem* bookmark)
  {
    if (!bookmark)
    {
      return;
    }

    if (auto* sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      auto future = sceneView->setBookmarkAsync(bookmark->bookmark());
      Q_UNUSED(future)
    }
    else if (auto* localSceneView = qobject_cast<LocalSceneViewToolkit*>(m_geoView))
    {
      auto future = localSceneView->setBookmarkAsync(bookmark->bookmark());
      Q_UNUSED(future)
    }
    else if (auto* mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      auto future = mapView->setBookmarkAsync(bookmark->bookmark());
      Q_UNUSED(future)
    }
  }

} // namespace Esri::ArcGISRuntime::Toolkit
