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
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13


DemoPage {
    anchors.fill: parent
    // set focus to enable keyboard navigation
    focus: true
    property var featureLayer : null
    sceneViewContents: Component {
        SceneView {
            id:sceneView
            Scene {
                BasemapTopographic {}

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

            onIdentifyLayerStatusChanged: {
                if (identifyLayerStatus !== Enums.TaskStatusCompleted)
                    return;
                if (featureLayer.layerType === Enums.LayerTypeFeatureLayer) {
                    // remove the selection if click was not on a feature
                    featureLayer.clearSelection();
                    const geoElements = sceneView.identifyLayerResult.geoElements;
                    // visually select the features
                    for (let i = 0; i < geoElements.length; i++) {
                        featureLayer.selectFeature(geoElements[i]);
                    }
                }

                if(sceneView.identifyLayerResult.geoElements.length === 0){
                    return;
                }
                var popup = ArcGISRuntimeEnvironment.createObject("Popup", {initGeoElement : sceneView.identifyLayerResult.geoElements[0]});
                popup.popupDefinition.title = sceneView.identifyLayerResult.layerContent.name;
                const popupManager = ArcGISRuntimeEnvironment.createObject("PopupManager", { popup: popup});
                popupView.popupManager = popupManager;
                popupView.visible = true;
                }

                PopupView {
                    id:popupView
                    anchors {
                         left: parent.left
                         top: parent.top
                         bottom: parent.bottom
                    }
                    visible: true
                }
            }
        }

    mapViewContents: Component {
    MapView {
        id:mapView
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

        onIdentifyLayerStatusChanged: {
            if (identifyLayerStatus !== Enums.TaskStatusCompleted)
                return;
            //select a feature

            if (featureLayer.layerType === Enums.LayerTypeFeatureLayer) {
                featureLayer.clearSelection();
                const geoElements = mapView.identifyLayerResult.geoElements;

                for (let i = 0; i < geoElements.length; i++) {
                    featureLayer.selectFeature(geoElements[i]);
                }
            }

            if(mapView.identifyLayerResult.geoElements.length === 0){
                return;
            }

            var popup = ArcGISRuntimeEnvironment.createObject("Popup", {initGeoElement : mapView.identifyLayerResult.geoElements[0]});
            popup.popupDefinition.title = mapView.identifyLayerResult.layerContent.name;
            const popupManager = ArcGISRuntimeEnvironment.createObject("PopupManager", { popup: popup});
            popupView.popupManager = popupManager;
            popupView.visible = true;
            }

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
}
