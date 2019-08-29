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

// Creates a scene centered on Yosemite National Park.
// Mode: Tabletop AR

Item {
    property alias scene: scene
    property var originCamera: null
    property var locationDataSource: null
    property double translationFactor: 2000

    SceneWithElevation {
        id:scene
        IntegratedMeshLayer {
            id: layer
            url: "https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/" +
                 "VRICON_Yosemite_Sample_Integrated_Mesh_scene_layer/SceneServer"

            onLoadStatusChanged: {
                if (loadStatus === Enums.LoadStatusLoaded) {
                    // Get the center point of the layer's extent.
                    var center = layer.fullExtent.center;

                    // Find the elevation of the layer at the center point.
                    var surface = scene.baseSurface;
                    surface.onLocationToElevationStatusChanged.connect(function() {
                        if (surface.locationToElevationStatus !== Enums.TaskStatusCompleted)
                            return;

                        // Create the origin camera at the center point and elevation of the data.
                        // This will ensure the data is anchored to the table.
                        originCamera = ArcGISRuntimeEnvironment.createObject("Camera", {
                                                                                 latitude: center.y,
                                                                                 longitude: center.x,
                                                                                 altitude: 1200, // surface.locationToElevationResult
                                                                                 heading: 0,
                                                                                 pitch: 90,
                                                                                 roll: 0
                                                                             });

                    });
                    surface.locationToElevation(center);
                }
            }
        }
    }
}
