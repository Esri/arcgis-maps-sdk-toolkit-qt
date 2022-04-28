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
import Esri.ArcGISRuntime.Toolkit 100.15


DemoPage {
    id: demo
    anchors.fill: parent

    sceneViewContents: Component {
        SceneView {
            id:sceneView
            property var featureLayer : null
            Scene {
                initBasemapStyle: Enums.BasemapStyleArcGISTopographic
                initialViewpoint: ViewpointExtent {
                    Envelope {
                        SpatialReference {
                            wkid:4326
                        }
                        xMax:-122.3511
                        xMin:-122.5277
                        yMax:37.7956
                        yMin:37.7204
                    }
                }

                FeatureLayer {
                    ServiceFeatureTable {
                        id: featureTable
                        url: "https://sampleserver6.arcgisonline.com/arcgis/rest/services/SF311/FeatureServer/0"
                    }
                    renderingMode: Enums.FeatureRenderingModeDynamic
                }
            }
            onMouseClicked: {
                //get the feature layer from the map
                featureLayer = sceneView.scene.operationalLayers.get(0);
                sceneView.identifyLayer(featureLayer, mouse.x, mouse.y , 12, false);
            }

            onIdentifyLayerStatusChanged: onIdentified.bind(sceneView)(featureLayer, popupView)

            PopupView {
                id:popupView
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
                visible: false
            }
        }
    }

    mapViewContents: Component {
//! [Set up Popup View]
        MapView {
            id:mapView
            property var featureLayer : null
            anchors.fill: parent
            Map {
                id:map
                initUrl: "https://runtime.maps.arcgis.com/home/webmap/viewer.html?webmap=e4c6eb667e6c43b896691f10cc2f1580"
            }

            onMouseClicked: {
                //get the feature layer from the map
                featureLayer = mapView.map.operationalLayers.get(0);
                mapView.identifyLayer(featureLayer, mouse.x, mouse.y , 12, false);
            }

            onIdentifyLayerStatusChanged: onIdentified.bind(mapView)(featureLayer, popupView)

            PopupView {
                id:popupView
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
                visible: false
            }
        }
    }

    function onIdentified(featureLayer, popupView) {
        if (this.identifyLayerStatus !== Enums.TaskStatusCompleted)
            return;
        //select a feature

        if (featureLayer.layerType === Enums.LayerTypeFeatureLayer) {
            featureLayer.clearSelection();
            const geoElements = this.identifyLayerResult.geoElements;

            for (let i = 0; i < geoElements.length; i++) {
                featureLayer.selectFeature(geoElements[i]);
            }
        }

        if(this.identifyLayerResult.geoElements.length === 0){
            return;
        }

        var popup = ArcGISRuntimeEnvironment.createObject("Popup", {initGeoElement : this.identifyLayerResult.geoElements[0]});
        popup.popupDefinition.title = this.identifyLayerResult.layerContent.name;
        const popupManager = ArcGISRuntimeEnvironment.createObject("PopupManager", {popup: popup});
        popupView.popupManager = popupManager;
        popupView.visible = true;
    }
//! [Set up Popup View]
}
