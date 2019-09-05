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

// Creates a scene centered on the US-Mexico border.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property var originCamera: null
    property var locationDataSource: null
    property double translationFactor: 1000

    SceneWithElevation {
        id:scene
        IntegratedMeshLayer {
            id: layer
            url: "https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/" +
                 "VRICON_SW_US_Sample_Integrated_Mesh_scene_layer/SceneServer"

        }
    }

    Camera {
        id: originCamera
        location: Point {
            y: 32.5337
            x: -116.925
            z: 126.0
        }
        heading: 0.0
        pitch: 90.0
        roll: 0.0
    }
}
