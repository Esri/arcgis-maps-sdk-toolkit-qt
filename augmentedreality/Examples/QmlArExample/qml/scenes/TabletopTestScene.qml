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
import Esri.ArcGISArToolkit 1.0

// Creates a tabletop test scene.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property alias graphicsOverlay: graphicsOverlay
    property alias originCamera: originCamera
    property LocationDataSource locationDataSource: null
    property double translationFactor: 10.0

    SceneWithElevation {
        id:scene
    }

    GraphicsOverlay {
        id: graphicsOverlay
        sceneProperties: LayerSceneProperties {
            surfacePlacement: Enums.SurfacePlacementAbsolute
        }

        // Draw a line of red sphere from South to North
        TabletopTestSceneGraphic {
            x: 0.000001
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000002
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000003
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000004
            color: "red"
        }
        TabletopTestSceneGraphic {
            x: 0.000005
            color: "red"
        }

        // Draw a line of blue sphere from West to East
        TabletopTestSceneGraphic {
            y: 0.000001
            color: "blue"
        }
        TabletopTestSceneGraphic {
            y: 0.000002
            color: "blue"
        }
        TabletopTestSceneGraphic {
            y: 0.000003
            color: "blue"
        }
        TabletopTestSceneGraphic {
            y: 0.000004
            color: "blue"
        }
        TabletopTestSceneGraphic {
            y: 0.000005
            color: "blue"
        }

        // Draw a line of green sphere from bottom to top
        TabletopTestSceneGraphic {
            color: "green"
        }
        TabletopTestSceneGraphic {
            z: 0.1
            color: "green"
        }
        TabletopTestSceneGraphic {
            z: 0.2
            color: "green"
        }
        TabletopTestSceneGraphic {
            z: 0.3
            color: "green"
        }
        TabletopTestSceneGraphic {
            z: 0.4
            color: "green"
        }
        TabletopTestSceneGraphic {
            z: 0.5
            color: "green"
        }
    }

    Camera {
        id: originCamera
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
