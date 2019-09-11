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

// Creates a tabletop test scene.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property alias graphicsOverlay: graphicsOverlay
    property var originCamera: camera
    property var locationDataSource: null
    property double translationFactor: 10.0

    SceneWithElevation {
        id:scene
    }

    GraphicsOverlay {
        id: graphicsOverlay
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.0
            color: "green"
        }

        TabletopTestSceneGraphic {
            x: 0.000001
            y: 0.0
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000002
            y: 0.0
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000003
            y: 0.0
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000004
            y: 0.0
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000005
            y: 0.0
            color: "red"
        }

        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.000001
            color: "blue"
        }
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.000002
            color: "blue"
        }
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.000003
            color: "blue"
        }
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.000004
            color: "blue"
        }
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.000005
            color: "blue"
        }
    }

    Camera {
        id: camera
        location: Point {
            y: 0.0
            x: 0.0
            z: 0.0
        }
        heading: 0.0
        pitch: 90.0
        roll: 0.0
    }
}
