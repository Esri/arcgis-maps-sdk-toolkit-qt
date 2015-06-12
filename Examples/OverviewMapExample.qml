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
import QtPositioning 5.2
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISRuntime.Toolkit.Controls 1.0

Rectangle {
    width: 500
    height: 400

    Map {
        id: map
        anchors.fill: parent
        wrapAroundEnabled: true
        mapPanningByMagnifierEnabled: true
        magnifierOnPressAndHoldEnabled: true
        zoomByPinchingEnabled: true

        positionDisplay {
            positionSource: PositionSource {
            }
        }

        ArcGISTiledMapServiceLayer {
            url: "http://services.arcgisonline.com/arcgis/rest/services/NatGeo_World_Map/MapServer"
        }


        Envelope {
            id: initialExtent
            xMin: -401141.52444049926
            yMin: 5025857.891979294
            xMax: 4295149.493399481
            yMax: 7518316.510301659
        }

        OverviewMap {
            anchors {
                top: parent.top
                left: parent.left
                margins: 20
            }

            baseLayer: ArcGISTiledMapServiceLayer {
                url: "http://services.arcgisonline.com/arcgis/rest/services/NatGeo_World_Map/MapServer"
            }

            northArrow {
                visible: false
            }
        }

        NorthArrow {
            anchors {
                top: parent.top
                right: parent.right
                margins: 20
            }
            visible: map.mapRotation != 0
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
