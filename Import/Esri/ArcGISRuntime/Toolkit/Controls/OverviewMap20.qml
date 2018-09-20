/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.0
import QtQuick.Controls 1.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import Esri.ArcGISRuntime 100.0

Item {
    id: overview

    property alias glow : overviewGlow
    property alias mapview: overviewmap
    property alias markupLayer : markupLayer

    property var parentMapview: null
    property var overviewLayer: null
    property string fillColor : "#60000000"
    property real zoomRatio: 10
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" || Qt.platform.os === "linux" ? 96 : 72)

    width: 200 * displayScaleFactor
    height: width * 0.665


    function zoomAll() {
        overviewmap.setViewpointRotation(0);
        overviewmap.setViewpointGeometry(parentMapview.map.initialViewpoint.extent);
    }

    function updateView() {
        if( !parentMapview || !parentMapview.map ||!overviewmap.map)
            return;
        if (parentMapview.map.loadStatus !== Enums.LoadStatusLoaded)
            return;
        if (overviewmap.map.loadStatus !== Enums.LoadStatusLoaded)
            return;

        aoiLayer.graphics.clear();
        var e = parentMapview.map.initialViewpoint.extent;
        var v = parentMapview.visibleArea.extent;

        var part1 = ArcGISRuntimeEnvironment.createObject("Part");
        part1.spatialReference = overviewmap.map.spatialReference
        part1.addPointXY(e.xMin, e.yMax);
        part1.addPointXY(e.xMax, e.yMax);
        part1.addPointXY(e.xMax, e.yMin);
        part1.addPointXY(e.xMin, e.yMin);
        part1.addPointXY(e.xMin, e.yMax);

        var part2 = ArcGISRuntimeEnvironment.createObject("Part");
        part2.spatialReference = overviewmap.map.spatialReference
        part2.addPointXY(v.xMax, v.yMin);
        part2.addPointXY(v.xMax, v.yMax);
        part2.addPointXY(v.xMin, v.yMax);
        part2.addPointXY(v.xMin, v.yMin);
        part2.addPointXY(v.xMax, v.yMin);

        var pCollection = ArcGISRuntimeEnvironment.createObject("PartCollection");
        var builder = ArcGISRuntimeEnvironment.createObject("PolygonBuilder");
        pCollection.spatialReference =  overviewmap.map.spatialReference
        builder.spatialReference = overviewmap.map.spatialReference
        pCollection.addPart(part1);
        pCollection.addPart(part2);
        builder.parts = pCollection;

        // assign the geometry of the graphic to be the polygon
        var polygonGraphic = ArcGISRuntimeEnvironment.createObject("Graphic");
        polygonGraphic.geometry = builder.geometry;
        polygonGraphic.symbol = fillSymbol;
        overviewmap.setViewpointCenterAndScale(parentMapview.visibleArea.extent.center,parentMapview.mapScale * zoomRatio);
        aoiLayer.graphics.append(polygonGraphic);
    }

    Connections {
        target: parentMapview

        onVisibleAreaChanged: {
            updateView();
        }
    }

    RectangularGlow {
        id: overviewGlow
        anchors.fill: overviewmap
        glowRadius: 10
        spread: 0.2
        color: "black"
        cornerRadius: 10
    }

    SimpleFillSymbol {
        id: fillSymbol
        style: Enums.SimpleFillSymbolStyleSolid
        color: fillColor
    }

    MapView {
        id: overviewmap
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeDisabled

        GraphicsOverlay {
            id: aoiLayer
        }

        GraphicsOverlay {
            id: markupLayer
        }
    }

    Component.onCompleted: {
        var basemap = ArcGISRuntimeEnvironment.createObject("Basemap");
        basemap.baseLayers.append(overviewLayer);
        overviewmap.map = ArcGISRuntimeEnvironment.createObject("Map", {basemap: basemap});
    }
}
