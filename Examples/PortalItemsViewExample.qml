/*******************************************************************************
 * Copyright 2012-2014 Esri
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

import QtQuick 2.2
import QtQuick.Controls 1.1
import ArcGIS.Runtime 10.25
import ArcGIS.Runtime.Toolkit.Controls 1.0

Rectangle {
    width: 600
    height: 400

    Component.onCompleted: {
        portalItemsView.refresh();
    }

    function addLayer(url) {
        var layer = ArcGISRuntime.createObject("ArcGISTiledMapServiceLayer", { "url": url });
        map.addLayer(layer);
    }

    Portal {
        id: portal
        url: "http://arcgis.com"
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        resizing: true

        handleDelegate: Rectangle {
            width: 15
            height: parent.height
            color: "black"

            Rectangle {
                width: 1
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                }
                color: "#80808080"
            }
        }

        Rectangle {
            width: parent.width * 0.25
            color: "#F0F0F0"

            PortalItemsView {
                id: portalItemsView
                anchors {
                    fill: parent
                    margins: 10
                }
                portal: portal
                query: 'owner:esri AND type:"Map Service"'
                highlightFollowsCurrentItem: true

                onDoubleClicked: {
                    addLayer(itemInfo.url);
                }
            }
        }

        Map {
            id: map
            wrapAroundEnabled: true

            NorthArrow {
                anchors {
                    top: parent.top
                    right: parent.right
                    margins: 20
                }
            }

            NavigationToolbar {
                anchors {
                    right: parent.right
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
