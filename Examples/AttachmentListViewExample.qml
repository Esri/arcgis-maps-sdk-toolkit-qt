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
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Controls 2.0

Rectangle {
    width: 800
    height: 600

    property real scaleFactor: System.displayScaleFactor
    property var selectedFeature: null
    property var attachmentModel: null

    // Create MapView that contains a Map
    MapView {
        id: mapView
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeDisabled

        Map {
            // Set the initial basemap to Streets
            BasemapStreets { }

            ViewpointCenter {
                Point {
                    x: -10800000
                    y: 4500000
                    spatialReference: SpatialReference {
                        wkid: 102100
                    }
                }
                targetScale: 3e7
            }

            FeatureLayer {
                id: featureLayer
                selectionColor: "cyan"
                selectionWidth: 3

                // declare as child of feature layer, as featureTable is the default property
                ServiceFeatureTable {
                    id: featureTable
                    url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0"

                    onApplyEditsStatusChanged: {
                        if (applyEditsStatus === Enums.TaskStatusCompleted)
                            console.log("successfully applied attachment edits to service");
                    }
                }

                // signal handler for selecting features
                onSelectFeaturesStatusChanged: {
                    if (selectFeaturesStatus === Enums.TaskStatusCompleted) {

                        // if no feature located, stop
                        if (!selectFeaturesResult.iterator.hasNext)
                            return;

                        selectedFeature = selectFeaturesResult.iterator.next();

                        // if selected feature loads, use its attributes property as Model for the view
                        selectedFeature.loadStatusChanged.connect(function() {
                            if (selectedFeature.loadStatus === Enums.LoadStatusLoaded) {
                                attachmentModel = selectedFeature.attachments;
                                attachmentList.visible = true;
                            }
                        });

                        // load the selected feature
                        selectedFeature.load();
                    }
                }
            }
        }

        QueryParameters {
            id: params
            maxFeatures: 1
        }

        // hide the view after navigation
        onViewpointChanged: {
            attachmentList.visible = false;
        }

        onMouseClicked: {
            // reset to defaults
            featureLayer.clearSelection();
            attachmentList.visible = false;
            selectedFeature = null;

            // call identify on the mapview
            mapView.identifyLayer(featureLayer, mouse.x, mouse.y, 10);
        }

        onIdentifyLayerStatusChanged: {
            if (identifyLayerStatus === Enums.TaskStatusCompleted) {
                if (identifyLayerResult.geoElements.length > 0) {
                    // get the objectid of the identifed object
                    params.objectIds = [identifyLayerResult.geoElements[0].attributes.attributeValue("objectid")];
                    // query for the feature using the objectid
                    featureLayer.selectFeaturesWithQuery(params, Enums.SelectionModeNew);
                }
            }
        }
    }

    // attachment view
    AttachmentListView {
        id: attachmentList
        anchors.centerIn: parent
        model: attachmentModel

        onAddButtonClicked: {
            if (selectedFeature.loadStatus === Enums.LoadStatusLoaded)
                selectedFeature.attachments.addAttachment(fileUrl, contentType, name);
        }

        onDeleteButtonClicked: {
            if (selectedFeature.loadStatus === Enums.LoadStatusLoaded)
                selectedFeature.attachments.deleteAttachmentWithIndex(attachmentIndex);
        }
    }
}
