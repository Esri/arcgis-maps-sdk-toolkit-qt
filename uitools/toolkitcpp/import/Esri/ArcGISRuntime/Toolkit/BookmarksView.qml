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
import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Layouts

/*!
 \qmltype BookmarksView
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.15
 \brief The user interface for the BookmarksView.

 The BookmarksView displays a collection of viewpoints represented by bookmarks from either Webmap/Webscene
 or are programmatically defined. When the user selects a bookmark from the provided list,
 the viewpoint in the geoView is set to the new bookmark's view extent.

 The list is a single tab stop. The Up, Down, Home and End keys move between bookmarks, and Enter or Space
 selects the current one. Assistive technologies see the list as "Bookmarks"; set \c Accessible.name on the
 BookmarksView to change that name.
 \image bookmarksView.gif
 \snippet qml/demos/BookmarksViewDemoForm.qml Set up Bookmark QML
*/

Pane {
    id: bookmarksView

    /*!
      \internal
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

        // The list is a single tab stop. Up/Down, Home and End move the current bookmark;
        // only Enter, Space or a click moves the GeoView.
        activeFocusOnTab: count > 0
        onCountChanged: {
            if (count > 0 && currentIndex < 0)
                currentIndex = 0;
        }
        Keys.onPressed: event => {
                            if (event.key === Qt.Key_Home) {
                                currentIndex = 0;
                                event.accepted = true;
                            } else if (event.key === Qt.Key_End) {
                                currentIndex = count - 1;
                                event.accepted = true;
                            }
                        }

        Accessible.role: Accessible.List
        Accessible.name: bookmarksView.Accessible.name.length > 0 ? bookmarksView.Accessible.name
                                                                  : qsTr("Bookmarks")

        ScrollBar.vertical: ScrollBar {
            active: true
            snapMode: ScrollBar.SnapOnRelease
        }

        Label {
            objectName: "emptyLabel"
            anchors.fill: parent
            visible: listView.count === 0
            text: qsTr("No bookmarks")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        delegate: ItemDelegate {
            id: bookmarkDelegate

            function zoomToBookmark() {
                listView.currentIndex = index;
                controller.zoomToBookmarkExtent(listModelData);
                bookmarksView.Accessible.announce(qsTr("Showing %1").arg(name), Accessible.Polite);
            }

            // `name` is a role within GenericListModel's BookmarkListItem specification.
            // `text` can take `name` because the `model` builds the roles from the
            // properties of BookmarkListItem that is set in BookmarksViewController.
            text: name
            width: listView.width
            height: Math.max(implicitHeight, 24)
            indicator: null
            contentItem: Label {
                text: bookmarkDelegate.text
                font: bookmarkDelegate.font
                wrapMode: Text.Wrap
                verticalAlignment: Text.AlignVCenter
                // The row already carries the bookmark name.
                Accessible.ignored: true
            }

            Accessible.name: name
            Accessible.focusable: true
            Accessible.selectable: true
            Accessible.selected: ListView.isCurrentItem
            onActiveFocusChanged: {
                if (activeFocus)
                    listView.currentIndex = index;
            }
            onClicked: zoomToBookmark()
            Keys.onReturnPressed: zoomToBookmark()
            Keys.onEnterPressed: zoomToBookmark()

            // Keyboard focus indicator.
            Rectangle {
                objectName: "focusRing"
                anchors.fill: parent
                visible: bookmarkDelegate.ListView.isCurrentItem && listView.activeFocus
                color: "transparent"
                border.width: 2
                border.color: bookmarkDelegate.contentItem.color
            }
        }
    }

    implicitHeight: Math.min(listView.count > 0 ? Math.min(200 * bookmarksView.font.pointSize / 12,
                                                           listView.contentHeight + topPadding + bottomPadding)
                                                : 200 * bookmarksView.font.pointSize / 12,
                             parent ? Math.max(0, parent.height - anchors.topMargin - anchors.bottomMargin)
                                    : 200 * bookmarksView.font.pointSize / 12)
    implicitWidth: Math.min(300 * bookmarksView.font.pointSize / 12,
                            parent ? Math.max(0, parent.width - anchors.leftMargin - anchors.rightMargin)
                                   : 300 * bookmarksView.font.pointSize / 12)
}
