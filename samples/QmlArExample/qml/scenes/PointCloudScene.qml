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

// Creates a scene based on a point cloud layer.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property var originCamera: null
    property var locationDataSource: null
    property double translationFactor: 2000

    SceneWithElevation {
        id:scene
        PointCloudLayer {
            id: layer
            PortalItem {
                itemId: "fc3f4a4919394808830cd11df4631a54"
            }
            onLoadStatusChanged: {
                if (loadStatus === Enums.LoadStatusLoaded) {
                    var center = layer.fullExtent.center;
                    originCamera = ArcGISRuntimeEnvironment.createObject("Camera", {
                                                                             latitude: center.y,
                                                                             longitude: center.x,
                                                                             altitude: 100,
                                                                             heading: 0,
                                                                             pitch: 90,
                                                                             roll: 0
                                                                         });
                }
            }
        }
    }
}
