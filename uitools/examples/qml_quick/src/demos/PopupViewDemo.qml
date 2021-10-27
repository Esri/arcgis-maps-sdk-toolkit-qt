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
import DemoPage 1.0

DemoPage {
    anchors.fill: parent
    // set focus to enable keyboard navigation
    focus: true
    property var featureLayer : null

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
            const popups = mapView.identifyLayerResult.popups;

            if(popups.length === 0){
                console.log("no popups related to the selected features were found");
                return;
            }

            // popupmanager requires a single popup, retreiving first popup
            const popupManager = ArcGISRuntimeEnvironment.createObject("PopupManager", { popup: popups[0]});
            popupView.popupManager = popupManager;

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
}
