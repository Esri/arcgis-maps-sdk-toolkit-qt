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
    property var selectedFeature: null

    // Create MapView that contains a Map
    MapView {
        id: mapView
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeDisabled

        Map {
            // Set the initial basemap to Streets
            BasemapStreets { }

            // set viewpoint over The United States
            ViewpointCenter {
                Point {
                    x: -10800000
                    y: 4500000
                    spatialReference: SpatialReference {
                        wkid: 102100
                    }
                }
                scale: 3e7
            }

            FeatureLayer {
                id: featureLayer

                selectionColor: "cyan"
                selectionWidth: 3

                // declare as child of feature layer, as featureTable is the default property
                ServiceFeatureTable {
                    id: featureTable
                    url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0"

                    // make sure edits are successfully applied to the service
                    onApplyEditsStatusChanged: {
                        if (applyEditsStatus === Enums.TaskStatusCompleted) {
                            console.log("successfully updated feature");
                        }
                    }

                    // signal handler for the asynchronous updateFeature method
                    onUpdateFeatureStatusChanged: {
                        if (updateFeatureStatus === Enums.TaskStatusCompleted) {
                            // apply the edits to the service
                            console.log("update completed. applying edits...");
                            featureTable.applyEdits();
                        }

                        if(updateFeatureStatus === Enums.TaskStatusErrored)
                            console.log("error");
                    }
                }

                // signal handler for selecting features
                onSelectFeaturesStatusChanged: {
                    if (selectFeaturesStatus === Enums.TaskStatusCompleted) {

                        if (!selectFeaturesResult.iterator.hasNext)
                            return;

                        selectedFeature = selectFeaturesResult.iterator.next();

                        // if selected feature loads, use its attributes property as Model for the view
                        selectedFeature.loadStatusChanged.connect(function() {
                            if (selectedFeature.loadStatus === Enums.LoadStatusLoaded) {
                                attributeListView.model = selectedFeature.attributes;
                            }
                        });

                        selectedFeature.load();
                    }
                }
            }
        }

        QueryParameters {
            id: params
            maxFeatures: 1
        }

        onMouseClicked: {
            // reset selection and check if there is a feature at clicked point
            featureLayer.clearSelection();
            mapView.identifyLayer(featureLayer, mouse.x, mouse.y, 10, 1);
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

    AttributeListView {
        id: attributeListView
        anchors {
            left: parent.left
            top: parent.top
            margins: 5 * scaleFactor
        }

        // after the model has been changed, update the feature
        onAttributeValueChanged: {
            featureTable.updateFeature(selectedFeature);
        }
    }

}
