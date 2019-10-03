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

// Creates a scene based on a point cloud layer.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property Camera originCamera: originCamera
    property LocationDataSource locationDataSource: null
    property double translationFactor: 18000.0

    SceneWithElevation {
        id:scene
        PointCloudLayer {
            id: layer
            PortalItem {
                itemId: "fc3f4a4919394808830cd11df4631a54"
            }
        }
    }

    Camera {
        id: originCamera
        Point {
            y: 39.7712
            x: -74.1197
            z: 1.0
        }
        heading: 0.0
        pitch: 90.0
        roll: 0.0
    }
}
