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
import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit 100.15

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
