
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
//

import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISARView 1.0

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "QmlArSample"

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
//        onTransformationMatrixChanged: {
            // create QmlTransformationMatrix
            // create Camera
            // set to sceneiew
//        }
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
//        spaceEffect: Enums.SpaceEffectTransparent
//        atmosphereEffect: Enums.AtmosphereEffectNone

        // from https://devtopia.esri.com/mort5161/ARSamples/blob/b63c7b62c217d36fa44fb91fd271554061431af5/ArcGISAR.Droid/TestScenes.cs
        Scene {
            initialViewpoint: ViewpointExtent {
                Camera {
                    Point {
                        x: -4.49492
                        y: 48.3808
                        z: 48.2511
                        spatialReference: SpatialReference.createWgs84()
                    }
                    heading: 344.488
                    pitch: 74.1212
                    roll: 0.0
                }
            }
            Surface {
                navigationConstraint: Enums.NavigationConstraintNone
//                backgroundGrid: BackgroundGrid {
//                    visible: false
//                }
                ArcGISTiledElevationSource {
                    url: "https://scene.arcgis.com/arcgis/rest/services/BREST_DTM_1M/ImageServer"
                }
            }
            ArcGISSceneLayer {
                url: "https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Brest/SceneServer/layers/0"
                opacity: 1.0
            }
        }
    }
}
