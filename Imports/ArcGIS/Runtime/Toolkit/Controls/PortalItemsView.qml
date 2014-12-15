/*******************************************************************************
 * Copyright 2012-2015 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import ArcGIS.Runtime 10.25

Item {
    id: portalItemsView
    width: 600
    height: 500

    property string query
    property alias delegate: itemsGridView.delegate
    property alias highlight: itemsGridView.highlight
    property alias highlightFollowsCurrentItem: itemsGridView.highlightFollowsCurrentItem
    property alias gridView: itemsGridView
    property int searchPageSize: 100
    property Portal portal
    property bool infoButtonVisible: true
    property int infoButtonSize: 25
    property int referenceWidth: 200 * displayScaleFactor
    property int cellMargin: 10
    property int cells: calcCells(width)
    property int cellWidth: width / cells
    property int titleMargin: 45 * displayScaleFactor
    property int cellHeight: cellWidth
    property real titlePointSize: 12
    property alias currentIndex: itemsGridView.currentIndex
    property PortalItemInfo currentItem: itemsGridView.currentIndex >= 0 ? itemsGridView.model[itemsGridView.currentIndex] : null
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    signal clicked(PortalItemInfo itemInfo)
    signal doubleClicked(PortalItemInfo itemInfo)

    function refresh() {
        console.log("Search Parameters=", JSON.stringify(portalSearchParameters.json));
        portalSearch.searchItems(portalSearchParameters);
    }

    onQueryChanged: {
        refresh();
    }

    function calcCells(w) {
        var rw =  (referenceWidth + cellMargin) * displayScaleFactor;
        var c = Math.max(1, Math.round(w / referenceWidth));
        var cw = w / c;
        if (cw > rw)
            c++;
        cw = w / c;
        if (c > 1 && cw < (rw * 0.75))
            c--;
        return c;
    }

    PortalSearchItems {
        id: portalSearch
        portal: portalItemsView.portal
    }

    PortalSearchParameters {
        id: portalSearchParameters
        start: 1
        limit: searchPageSize
        query: portalItemsView.query
    }

    GridView {
        id: itemsGridView
        anchors.fill: parent
        model: portalSearch.results
        delegate: defaultDelegate
        cellWidth: portalItemsView.cellWidth
        cellHeight: portalItemsView.cellHeight
        clip: true
        highlightFollowsCurrentItem: false //true
        highlight: defaultHighlight
    }

    Component {
        id: defaultHighlight

        Item {
            width: itemsGridView.cellWidth
            height: itemsGridView.cellHeight
            visible: itemsGridView.highlightFollowsCurrentItem

            RectangularGlow {
                anchors {
                    fill: parent
                    margins: cellMargin
                }
                glowRadius: cellMargin
                cornerRadius: cellMargin
                spread: 0.5
                color: "#0079c1"
            }

            x: defaultHighlight.GridView.view ? defaultHighlight.GridView.view.currentItem.x : 0
            y: defaultHighlight.GridView.view ? defaultHighlight.GridView.view.currentItem.y : 0

            Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }
    }

    Component {
        id: defaultDelegate

        Item {
            width: itemsGridView.cellWidth
            height: itemsGridView.cellHeight

            Item {
                anchors {
                    fill: parent
                    margins: portalItemsView.cellMargin
                }

                RectangularGlow {
                    color: "lightgray"
                    anchors.fill: itemRect
                    glowRadius: cellMargin / 3
                }

                Rectangle {
                    id: itemRect
                    anchors.fill: parent
                    border {
                        width: 1
                        color: "#4c4c4c"
                    }
                    color: "#f7f8f8"
                    radius: 3

                    Image {
                        id: thumbnailImage
                        height: (width * 133) / 200
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                            margins: 4
                        }
                        source: thumbnailUrl
                        fillMode: Image.PreserveAspectFit

                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border {
                                color: "darkgray"
                            }
                        }
                    }

                    Rectangle {
                        height: titleText.paintedHeight - (parent.height - thumbnailImage.height)
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: thumbnailImage.bottom
                        }
                        color:"#60FFFFFF"
                    }

                    Text {
                        id: titleText
                        text: title;
                        font {
                            pointSize: titlePointSize
                        }
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignBottom
                        color: "#323232"
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                            top: thumbnailImage.bottom
                            margins: 2
                        }
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            if (itemsGridView.highlightFollowsCurrentItem)
                                itemsGridView.currentIndex = index;
                            else
                                portalItemsView.clicked(itemsGridView.model[index]);
                        }

                        onDoubleClicked: {
                            portalItemsView.doubleClicked(itemsGridView.model[index]);
                        }
                    }

                    Item {
                        anchors {
                            top: thumbnailImage.bottom
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                        }

                        ImageButton {
                            id: infoButton
                            visible: portalItemsView.infoButtonVisible
                            source: "images/info.png"
                            height: portalItemsView.infoButtonSize * displayScaleFactor
                            width: height
                            anchors {
                                right: parent.right
                                margins: 2
                                verticalCenter: parent.top
                            }

                            onClicked: {
                                var itemInfo = itemsGridView.model[index];
                                infoDialog.title = itemInfo.title;
                                infoDialog.text = itemInfo.title;
                                infoDialog.informativeText = itemInfo.description;
                                infoDialog.detailedText =
                                        qsTr("Type") + " : " + itemType + "\r\n" +
                                        qsTr("Owner") + " : " + owner + "\r\n" +
                                        qsTr("Created") + " : " + created + "\r\n" +
                                        qsTr("Last Modified") + " : " + modified;
                                infoDialog.visible = true;
                            }
                        }
                    }
                }
            }
        }
    }

    MessageDialog {
        id: infoDialog
        icon: StandardIcon.Information
    }

    Connections {
        target: portal

        onSignInComplete: {
            refresh();
        }
    }
}
