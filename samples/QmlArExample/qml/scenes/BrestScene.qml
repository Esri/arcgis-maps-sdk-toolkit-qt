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

// Creates a scene centered on Brest (France)
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property alias originCamera: originCamera
    property var locationDataSource: null
    property double translationFactor: 500.0

    SceneWithElevation {
        id:scene
        ArcGISSceneLayer {
            url: "https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Brest/SceneServer/layers/0"
        }
    }

    Camera {
        id: originCamera
        Point {
            y: 48.3808
            x: -4.49492
            z: 48.2511
        }
        heading: 0.0
        pitch: 90.0
        roll: 0.0
    }
}
