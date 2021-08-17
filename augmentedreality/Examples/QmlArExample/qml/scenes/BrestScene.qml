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

// Creates a scene centered on Brest (France)
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property alias originCamera: originCamera
    property LocationDataSource locationDataSource: null
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
