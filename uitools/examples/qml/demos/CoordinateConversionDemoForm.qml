/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

import QtQuick
import QtQuick.Controls
import Esri.ArcGISRuntime
import Esri.ArcGISRuntime.Toolkit
import DemoApp

DemoPage {
    sceneViewContents: Component {
        SceneView {
            id: view

            //! [Set up Coordinate Conversion QML]
            CoordinateConversion {
                id: gallery
                geoView: parent
                anchors {
                    right: parent.right
                    margins: 10
                    bottom: parent.attributionTop
                }
            }
            //! [Set up Coordinate Conversion QML]

            CoordinateConversionDemo {
                geoView: view
            }
        }
    }

    mapViewContents: Component {
        MapView {
            id: view
            CoordinateConversion {
                geoView: parent
                anchors {
                    right: parent.right
                    margins: 10
                    bottom: parent.attributionTop
                }
            }
            CoordinateConversionDemo {
                geoView: view
            }
        }
    }
}