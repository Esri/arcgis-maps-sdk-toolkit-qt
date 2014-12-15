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
import QtQuick.Controls 1.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import ArcGIS.Runtime 10.25

Item {
    id: overview

    property Map map
    property string basemapPath
    property alias glow : overviewGlow
    property alias markupLayer : markupLayer
    property string fillColor : "#60000000"
    property bool initialized : false
    property var baseLayer
    property alias northArrow : northArrow
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    width: 200 * displayScaleFactor
    height: width * 0.665

    Timer {
        interval: 1000
        running: true
        repeat: false;

        onTriggered: {
            initialized = true;
            updateView();
        }
    }

    function zoomAll() {
        overviewMap.mapRotation = 0;
        overviewMap.extent = overviewBasemap.fullExtent;
    }

    function updateView() {

        if(!initialized)
            return;
        if (!map)
            return;
        if (map.status !== Enums.MapStatusReady)
            return;
        if (overviewMap.status !== Enums.MapStatusReady)
            return;
        aoiLayer.removeAllGraphics();
        var e = map.fullExtent;
        var v = map.visibleExtent;
        v.reverseAllPaths();

        var graphic = {
            geometry: {
                "rings" : [
                    [
                        [e.xMin, e.yMax],
                        [e.xMax, e.yMax],
                        [e.xMax, e.yMin],
                        [e.xMin, e.yMin],
                        [e.xMin, e.yMax]
                    ],
                    v.json.rings[0]
                ]
            },

            symbol: {
                type: "esriSFS",
                style: "esriSFSSolid",
                color: fillColor
            }
        };

        overviewMap.zoomToScale(map.mapScale * 15);
        overviewMap.panTo(map.extent.center);
        aoiLayer.addGraphic(graphic);
    }

    Connections {
        target: map

        onExtentChanged: {
            updateView();
        }
    }

    RectangularGlow {
        id: overviewGlow
        anchors.fill: overviewMap
        glowRadius: 10
        spread: 0.2
        color: "black"
        cornerRadius: 10
    }

    Map {
        id: overviewMap
        anchors.fill: parent
        wrapAroundEnabled: true
        esriLogoVisible: false

        ArcGISLocalTiledLayer {
            id: overviewBasemap
            path: basemapPath
        }

        GraphicsLayer {
            id: aoiLayer
        }

        GraphicsLayer {
            id: markupLayer
        }

        NorthArrow {
            id: northArrow
            width: 20
            anchors {
                top: parent.top
                right: parent.right
                margins: 5
            }
        }

        onStatusChanged: {
            if (status === Enums.MapStatusReady)
                fullExtent = overviewBasemap.fullExtent;
        }

        onExtentChanged: {
            updateView();
        }
    }

    Component.onCompleted: {
        if (!map && parent && parent.objectType && parent.objectType === "Map")
            map = parent;
        if (map && baseLayer)
            overviewMap.insertLayer(baseLayer, 0);
    }
}
