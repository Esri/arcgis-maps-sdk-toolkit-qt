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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKSVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKSVIEWCONTROLLER_H

// Toolkit headers
#include "BookmarkListItem.h"
#include "Internal/GenericListModel.h"

// ArcGISRuntime headers
#include <Bookmark.h>
#include <GeoView.h>
// Qt headers
#include <QObject>

namespace Esri {
  namespace ArcGISRuntime {
    namespace Toolkit {

      class BookmarksViewController : public QObject
      {
        Q_OBJECT
        Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
        Q_PROPERTY(QAbstractListModel* bookmarks READ bookmarks CONSTANT)
      public:
        Q_INVOKABLE BookmarksViewController(QObject* parent = nullptr);

        ~BookmarksViewController() override;

        QObject* geoView() const;

        void setGeoView(QObject* mapView);

        GenericListModel* bookmarks() const;

        Q_INVOKABLE void zoomToBookmarkExtent(Esri::ArcGISRuntime::Toolkit::BookmarkListItem* bookmark);

      signals:
        void geoViewChanged();

      private:
        QObject* m_geoView = nullptr;
        GenericListModel* m_bookmarks = nullptr;
      };

    } // Toolkit
  } // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKSVIEWCONTROLLER_H
