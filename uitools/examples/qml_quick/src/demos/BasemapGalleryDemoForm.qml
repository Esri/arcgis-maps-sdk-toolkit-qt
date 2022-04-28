// Copyright 2021 ESRI
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

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit.CppApi 100.15

DemoPage {
    sceneViewContents: Component {
        SceneView {
            Scene {
                id: scene
            }
            BasemapGallery {
                id: gallery
                geoModel: scene
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 10
                }
            }

            Connections {
                // On startup choose an appropriate basemap from the gallery
                // to be initially selected.
                target: gallery.controller.portal
                function onFetchDeveloperBasemapsStatusChanged() {
                    const basemaps = gallery.controller.portal.developerBasemaps;
                    if (basemaps.count > 0) {
                        scene.basemap = basemaps.get(0);
                    }
                }
            }
        }
    }

    mapViewContents: Component {
//! [Set up BasemapGallery QML]
        MapView {
            Map {
                id: map
            }
            BasemapGallery {
                id: gallery
                geoModel: map
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 10
                }
            }

            Connections {
                // On startup choose an appropriate basemap from the gallery
                // to be initially selected.
                target: gallery.controller.portal
                function onFetchDeveloperBasemapsStatusChanged() {
                    const basemaps = gallery.controller.portal.developerBasemaps;
                    if (basemaps.count > 0) {
                        map.basemap = basemaps.get(0);
                    }
                }
            }
        }
//! [Set up BasemapGallery QML]
    }
}
