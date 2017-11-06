
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
import Esri.ArcGISRuntime 100.2
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Controls 100.2

Rectangle {
    id: root
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor
    property var selectedFeature: null
    property var displayMode: ["rightpanel", "leftpanel", "fullscreen", "dialog", "animate"]
    property var currentAttachment
    property real adjustedX: width - popupView.width
    property real originX: width

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
                        if (displayModeComboBox.currentText !== "dialog") {
                            popupView.popupManager = newPopupManager;
                            if (displayModeComboBox.currentText === "animate") {
                                popupView.slideHorizontal(originX, adjustedX);
                            } else {
                                popupView.show();
                            }
                        } else {
                            popupViewDialog.popupManager = newPopupManager;
                            popupAsDialog.open();
                        }

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
                    if (displayModeComboBox.currentText === "animate") {
                        if (popupView.x === originX || popupView.x === 0) {
                            popupView.dismiss();
                        } else {
                            popupView.slideHorizontal(adjustedX, originX);
                        }
                    } else {
                        popupView.dismiss();
                    }

                }
            }
        }
    }

    ComboBox {
        id: displayModeComboBox
        anchors {
            left: parent.left
            top: parent.top
            margins: 5 * scaleFactor
        }
        model: displayMode

        onCurrentIndexChanged: popupView.state = displayMode[currentIndex];
    }

    // This is what the user can create. They can anchor it anywhere, display in a dialog, etc
    PopupView {
        id: popupView

        states: [
            State {
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
            }, State {
                name: "animate"

                AnchorChanges {
                    target: popupView
                    anchors.top: parent.top
                    anchors.left: undefined
                    anchors.right: undefined
                    anchors.bottom: parent.bottom
                }
            }
        ]

        onAttachmentThumbnailClicked: {
            // get the attachment
            currentAttachment = popupView.popupManager.attachmentMananger.attachmentsModel.get(index);

            // set up signal handler
            currentAttachment.fullImageStatusChanged.connect(function() {
                if (currentAttachment.fullImageStatus === Enums.TaskStatusCompleted) {
                    // assign the full image URL
                    attachmentImg.source = currentAttachment.fullImageUrl;
                    // show the dialog
                    imgDialog.open();
                }
            });

            // fetch the full image
            currentAttachment.fullImage();
        }

        Text {
            anchors {
                right: parent.right
                top: parent.top
                margins: 10 * scaleFactor
            }
            text: "x"
            font.pixelSize: 14 * scaleFactor
            visible: popupView.state === "fullscreen"

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
            implicitHeight: 400 * scaleFactor
            implicitWidth: 400 * scaleFactor
            visible: true
        }
    }

    Dialog {
        id: imgDialog
        standardButtons: StandardButton.Close
        modality: Qt.NonModal

        contentItem: Rectangle {
            implicitHeight: 400 * scaleFactor
            implicitWidth:  400 * scaleFactor

            Image {
                id: attachmentImg
                anchors.centerIn: parent
                width: parent.width
                height: parent.height
            }
        }
    }
}
