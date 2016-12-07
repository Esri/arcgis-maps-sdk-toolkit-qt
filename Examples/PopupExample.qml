
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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtPositioning 5.3
import QtSensors 5.3
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

    MapView {
        id: mapView
        anchors.fill: parent

        Map {
            id: map
            BasemapLightGrayCanvas{}

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

                // signal handler for selecting features
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
                    // get the objectid of the identifed object
                    params.objectIds = [identifyLayerResult.geoElements[0].attributes.attributeValue("objectid")];
                    // query for the feature using the objectid
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

        Button {
            text: "Show Popup From Mock Manager"
            onClicked: {
                popupView.popupManager = popupManager;
                popupView.show();
            }
        }

        Button {
            text: "Hide Popup"
            onClicked: {
                popupView.dismiss();
            }
        }
    }

    // This is what the user can create. They can anchor it anywhere, display in a dialog, etc
    PopupView {
        id: popupView
        anchors {
            right: parent.right
            top: parent.top
        }
    }

    // Mock PopupManager
    Item {
        id: popupManager

        property string title: "Damage to Commercial Buildings"
        property var displayedFields: attributeListModel
        property var attachmentMananger: attachments

        ListModel {
            id: attributeListModel
            ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }
            ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatReallyIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }ListElement { fieldName : "SomeName" ; fieldValue : "SomeValue" }
            ListElement { fieldName : "SomeNameThatIsLonger" ; fieldValue : "SomeNameThatReallyReallyIsLonger" }
            ListElement { fieldName : "SomeLongishName" ; fieldValue : "SomeLongishName" }
        }

        // Mock AttachmentManager
        Item {
            id: attachments

            property var attachmentListModel: _attachmentListModel

            ListModel {
                id: _attachmentListModel

                ListElement { name: "Attachment1" ; thumbnailUrl: "/Users/luca6804/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png" }
                ListElement { name: "AttachmentWithLongName" ; thumbnailUrl: "/Users/luca6804/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png" }
                ListElement { name: "AttachmentWithReallyLongName" ; thumbnailUrl: "/Users/luca6804/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png" }
                ListElement { name: "AttachmentWithReallyReallyLongName" ; thumbnailUrl: "/Users/luca6804/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png" }
                ListElement { name: "Attach.jpg" ; thumbnailUrl: "/Users/luca6804/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png" }
            }
        }
    }
}
