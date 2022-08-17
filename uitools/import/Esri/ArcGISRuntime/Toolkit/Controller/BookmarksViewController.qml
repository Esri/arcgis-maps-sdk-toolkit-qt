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
import QtQml 2.15
import Esri.ArcGISRuntime 100.15
import QtQml.Models 2.15

/*!
   \qmltype BookmarkController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.15
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief The controller part of a BookmarksView. This class handles the
   management of the Bookmark objects and zooming onto the bookmarks' view extents.
 */

QtObject {
    id: controller
    /*!
      \qmlproperty GeoView geoView
      \brief The geoView of the controller.
     */
    property GeoView geoView: null;

    /*!
      \qmlproperty ListModel bookmarks
      \brief The bookmarks of Bookmark list objects.

      Internally, the list is a QML ListModel containing elements of type ListElement.
      Each ListElement has a `modelData` and property `name`, which maps to a Bookmark.
     */
    readonly property alias bookmarks: internal.bookmarks

    /*!
       \brief Method that takes a Bookmark \a modelData and  zoom to that bookmark in the `geoView`.
     */
    function zoomToBookmarkExtent(modelData) {
        geoView.setBookmark(modelData);
    }

    /*! \internal */
    property QtObject internal : QtObject {
        id: internal

        // qmlproperty Bookmark rawBookmarks
        // The bookmarks, stored in the geoView.
        // This property can experience changes, which are copied into the internal `bookmarks` ListModel.
        property var rawBookmarks: {
            if (geoView === null)
                return null;

            if (geoView instanceof MapView) {
                return geoView.map ? geoView.map.bookmarks : null;
            }
            else if (geoView instanceof SceneView) {
                return geoView.scene ? geoView.scene.bookmarks : null;
            }
        }

        // qmlproperty ListModel bookmarks
        // The bookmarks of Bookmark ListModel.
        // This is the internal representation, containing the elements.
        property ListModel bookmarks: ListModel {
            //
        }

        // When the `geoView`'s bookmarks change, we want to reflect that change into this
        // internal `bookmarks` ListModel, which is utilised by the view.
        onRawBookmarksChanged: {
            bookmarks.clear();
            if (internal.rawBookmarks) {
                for (let i = 0; i <= internal.rawBookmarks.count; i++) {
                    let bookmark = internal.rawBookmarks.get(i);
                    if (bookmark)
                        bookmarks.insert(i, {modelData: bookmark, name: bookmark.name});
                }
            }
        }

        // We set up connections so that when the `geoView`'s bookmarks change, we want to reflect those changes
        // within the internal `bookmarks` ListModel, which is utilised by the view.
        property Connections rawBookmarkConections : Connections {
            target: internal.rawBookmarks

            function onRowsInserted(parent, first, last) {
                for (let i = first; i <= last; i++) {
                    let bookmark = internal.rawBookmarks.get(i);
                    bookmarks.insert(i, {modelData: bookmark, name: bookmark.name});
                }
            }

            function onRowsRemoved(parent, first, last) {
                bookmarks.remove(first, last-first+1);
            }

            function onRowsMoved(parent, start, end, destination, row) {
                bookmarks.move(start, row, end-start+1);
            }
        }
    }
}
