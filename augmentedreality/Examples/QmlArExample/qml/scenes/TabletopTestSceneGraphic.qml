/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

import QtQuick 2.6
import Esri.ArcGISRuntime 100.13

// Creates a graphic with sympbol for the tabletop test scene.

Graphic {
    property alias x: location.x
    property alias y: location.y
    property alias z: location.z
    property alias color: symbol.color
    property real size: 0.1

    Point {
        id: location
        x: 0.0
        y: 0.0
        z: 0.0
    }

    symbol: SimpleMarkerSceneSymbol {
        id: symbol
        style: "SimpleMarkerSceneSymbolStyleSphere"
        anchorPosition: Enums.SceneSymbolAnchorPositionCenter
        color: "green"
        width: size
        height: size
        depth: size
    }
}
