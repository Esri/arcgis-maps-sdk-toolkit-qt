
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

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "QmlArSample"

    // add a sceneView component
    SceneView {
        anchors.fill: parent

        // add a Scene to the SceneView
        Scene {
            // add the BasemapTopographic basemap to the scene
            BasemapTopographic {}

            // add a surface...surface is a default property of scene
            Surface {
                // add an arcgis tiled elevation source...elevation source is a default property of surface
                ArcGISTiledElevationSource {
                    url: "https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"
                }
            }
        }
    }
}
