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
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISArToolkit 1.0

// Creates a tabletop test scene.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property alias graphicsOverlay: graphicsOverlay
    property Camera originCamera: camera
    property LocationDataSource locationDataSource: null
    property double translationFactor: 10.0

    SceneWithElevation {
        id:scene
    }

    GraphicsOverlay {
        id: graphicsOverlay

        // draw a green sphere in coordinate (0,0)
        TabletopTestSceneGraphic {
            x: 0.0
            y: 0.0
            color: "green"
        }

        // draw a line of red sphere from South to North
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

        // draw a line of blue sphere from West to East
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
