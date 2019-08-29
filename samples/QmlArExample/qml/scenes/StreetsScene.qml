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
import Esri.ArcGISArToolkit 1.0

// Creates a scene based on the Streets base map.
// Mode: Full-Scale AR

Item {
    property alias scene: scene
    property alias originCamera: originCamera
    property alias locationDataSource: locationDataSource
    property double translationFactor: 1

    SceneWithElevation {
        id:scene
        BasemapStreets {
        }
    }

    Camera {
        id: originCamera
    }

    LocationDataSource {
        id: locationDataSource
    }
}
