// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.

import QtQuick 2.6
import Esri.ArcGISRuntime 100.6

// Creates a graphic with sympbol for the tabletop test scene.

Graphic {
    property alias x: location.x
    property alias y: location.y
    property alias color: symbol.color
    property real size: 0.1
    Point {
        id: location
        x: 0.0
        y: 0.0
    }
    symbol: SimpleMarkerSceneSymbol {
        id: symbol
        style: "SimpleMarkerSceneSymbolStyleSphere"
        color: "green"
        width: size
        height: size
        depth: size
    }
}
