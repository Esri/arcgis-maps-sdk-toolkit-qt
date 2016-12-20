
// Copyright 2016 ESRI
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
import QtQuick.Controls 1.5
import QtPositioning 5.3
import QtSensors 5.3
import QtQuick.Dialogs 1.2
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Controls 2.0

Rectangle {
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor
    property bool featureSelected: false
    property Point newLocation
    property var selectedFeature: null
    property var displayMode: ["default", "leftpanel", "rightpanel", "fullscreen", "dialog"]

    MapView {
        id: mapView
        anchors.fill: parent

        Map {
            id: map
            BasemapDarkGrayCanvasVector{}

            FeatureLayer {
                id: featureLayer

                ServiceFeatureTable {
                    id: featureTable
                    url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0"
                }

                function loadPopup(){
                    if (selectedFeature.loadStatus === Enums.LoadStatusLoaded) {
                        // create a popup
                        var newPopup = ArcGISRuntimeEnvironment.createObject("Popup", {initGeoElement: selectedFeature});
                        newPopup.popupDefinition.title = "Damage to Commercial Buildings";

                        // create a popup manager
                        var newPopupManager = ArcGISRuntimeEnvironment.createObject("PopupManager", {popup: newPopup});

                        // show the popup view
                        popupView.popupManager = newPopupManager;
                        popupView.show();
                    }
                }

                onSelectFeaturesStatusChanged: {
                    if (selectFeaturesStatus === Enums.TaskStatusCompleted) {
                        if (selectFeaturesResult.iterator.hasNext) {
                            selectedFeature = selectFeaturesResult.iterator.next();
                            selectedFeature.onLoadStatusChanged.connect(loadPopup);
                            selectedFeature.load();
                        }
                    }
                }
            }
        }

        QueryParameters {
            id: params
            maxFeatures: 1
        }

        onMouseClicked: {
            featureLayer.clearSelection();
            mapView.identifyLayer(featureLayer, mouse.x, mouse.y, 5, false);
        }

        onIdentifyLayerStatusChanged: {
            if (identifyLayerStatus === Enums.TaskStatusCompleted) {
                if (identifyLayerResult.geoElements.length > 0) {
                    params.objectIds = [identifyLayerResult.geoElements[0].attributes.attributeValue("objectid")];
                    featureLayer.selectFeaturesWithQuery(params, Enums.SelectionModeNew);
                } else {
                    // hide the view
                    popupView.popupManager = null;
                    popupView.dismiss();
                }
            }
        }
    }

    Column {
        anchors {
            left: parent.left
            top: parent.top
            margins: 5
        }
        spacing: 5

        ComboBox {
            width: 250 * scaleFactor
            model: displayMode

            onCurrentIndexChanged: {
                popupView.state = displayMode[currentIndex];
            }
        }

        Button {
            text: "Filter Attachments (on/off)"
            width: 250 * scaleFactor
            onClicked: {
                popupView.viewFilteredAttachments = !popupView.viewFilteredAttachments
            }
        }

        Button {
            text: "Hide Popup"
            width: 250 * scaleFactor
            onClicked: {
                if (popupView.state === "dialog") {
                    popupAsDialog.close();
                } else {
                    popupView.dismiss();
                }
            }
        }
    }

    // This is what the user can create. They can anchor it anywhere, display in a dialog, etc
    PopupView {
        id: popupView

        anchors {
            top: parent.top
            right: parent.right
        }

        states: [
            State {
                name: "default"

                AnchorChanges {
                    target: popupView
                    anchors.top: parent.top
                    anchors.left: undefined
                    anchors.right: parent.right
                    anchors.bottom: undefined
                }
            }, State {
                name: "leftpanel"

                AnchorChanges {
                    target: popupView
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: undefined
                    anchors.bottom: parent.bottom
                }
            }, State {
                name: "rightpanel"

                AnchorChanges {
                    target: popupView
                    anchors.top: parent.top
                    anchors.left: undefined
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }, State {
                name: "fullscreen"

                AnchorChanges {
                    target: popupView
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
            }, State {
                name: "dialog"

                AnchorChanges {
                    target: popupView
                    anchors.top: undefined
                    anchors.left: undefined
                    anchors.right: undefined
                    anchors.bottom: undefined
                }
            }
        ]

        Text {
            anchors {
                right: parent.right
                top: parent.top
                margins: 10 * scaleFactor
            }
            text: "X"
            font.pixelSize: 14 * scaleFactor

            MouseArea {
                anchors.fill: parent
                onClicked: popupView.dismiss();
            }
        }
    }

    Dialog {
        id: popupAsDialog
        standardButtons: StandardButton.Close
        modality: Qt.NonModal

        contentItem: PopupView {
            id: popupViewDialog
            implicitHeight: 400
            implicitWidth: 400
            popupVisible: true

            Text {
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 10 * scaleFactor
                }
                text: "X"
                font.pixelSize: 14 * scaleFactor

                MouseArea {
                    anchors.fill: parent
                    onClicked: popupAsDialog.close();
                }
            }
        }
    }
}
