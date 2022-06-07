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
import Esri.ArcGISRuntime.Toolkit.Controller 100.15

import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/*!
 \qmltype BookmarksView
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.15
 \brief The user interface for the BookmarksView.

 The BookmarksView displays a collection of viewpoints represented by bookmarks from either Webmap/Webscene
 or are programmatically defined. When the user selects a bookmark from the provided list,
 the viewpoint in the geoView is set to the new bookmark's view extent.
 \image docs/bookmarksView.gif
 Example code in the QML API (C++ API might differ):
 \snippet qml_quick/src/demos/BookmarksViewDemoForm.qml Set up Bookmark QML
*/

Pane {
    id: bookmarksView

    /*!
      \qmlproperty BookmarksViewController controller
      \brief The controller handles binding logic between the BookmarksView and
      the \c BookmarkListItem.

      The QML controller is documented \l{BookmarksViewController}{here} and the CPP controller is documented \l{Esri::ArcGISRuntime::Toolkit::BookmarksViewController}{here}.
    */
    property var controller: BookmarksViewController { }

    /*!
       \qmlproperty GeoView geoView
       \brief The \c GeoView for this tool. Currenlty, it can be a \c Map; but could be extended to use a \c Scene as well.
     */
    property var geoView;

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    wheelEnabled: true
    hoverEnabled: true

    contentItem: ListView{
        id: listView
        model: controller.bookmarks
        clip: true

        ScrollBar.vertical: ScrollBar {
            active: true
            snapMode: ScrollBar.SnapOnRelease
        }

        delegate: ItemDelegate {
            // `name` is a role within GenericListModel's BookmarkListItem specification.
            // `text` can take `name` because the `model` builds the roles from the
            // properties of BookmarkListItem that is set in BookmarksViewController.
            text: name
            width: listView.width
            indicator: null
            onPressed: controller.zoomToBookmarkExtent(modelData)
        }
    }

    implicitHeight: 200
    implicitWidth: 300
}
