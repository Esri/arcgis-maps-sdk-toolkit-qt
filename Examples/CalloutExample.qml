
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
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISRuntime.Toolkit.Controls 2.0

Item {
    id: appWindow
    width: 800
    height: 600

    property Point clickedPoint

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent

        // add a map to the mapview
        Map {
            // add the streets basemap to the map
            BasemapStreets {}

            FeatureLayer {
                id: featureLayer
                ServiceFeatureTable {
                    url: "http://holistic30.esri.com:6080/arcgis/rest/services/DamageInspection6/FeatureServer/0"
                }
            }

            // Set the initial viewpoint over the Netherlands
            initialViewpoint: ViewpointCenter {
                center: Point {
                    x: 544871.19
                    y: 6806138.66
                    spatialReference: SpatialReference { wkid: 102100 }
                }
                scale: 2e6
            }
        }

        // set the location display's position source
        locationDisplay {
            positionSource: PositionSource {
            }
            compass: Compass {}
        }

        QueryParameters {
            id: params
            maxFeatures: 1
        }

        onMouseClicked: {
            clickedPoint = mouse.mapPoint;
            // call identify on the feature layer
            mapView.identifyLayer(featureLayer, mouse.x, mouse.y, 10);
        }

        onIdentifyLayerStatusChanged: {
            if (identifyLayerStatus === Enums.TaskStatusCompleted) {
                if (identifyLayerResult.geoElements.length > 0) {
                    var geoElement = identifyLayerResult.geoElements[0];
                    // get the objectid of the identifed object
                    params.objectIds = [identifyLayerResult.geoElements[0].attributes.attributeValue("objectid")];
                    // query for the feature using the objectid
                    featureLayer.selectFeaturesWithQuery(params, Enums.SelectionModeNew);
                    mapView.calloutData.geoElement = geoElement;
                    console.log(geoElement.attributes);
                    mapView.calloutData.title = geoElement.attributes.attributeValue("objectid");
                    mapView.calloutData.detail = geoElement.attributes.attributeValue("typdamage");
                    callout.showCallout();
                } else {
                    var spatialRef = ArcGISRuntimeEnvironment.createObject("SpatialReference", {wkid: 4326});
                    var point = GeometryEngine.project(clickedPoint, spatialRef);

                    //no identify results, so display map location
                    mapView.calloutData.location = clickedPoint;
                    mapView.calloutData.title = "Location";
                    mapView.calloutData.detail = "Lat: " + point.y.toFixed(2) + " Long: " + point.x.toFixed(2);
                    mapView.calloutData.imageUrl = System.userHomePath + "/ArcGIS/Runtime/UnitTests/images/RedShinyPin.png";
                    callout.showCallout();
                }

            }
        }
    }

    Image {
        source: System.userHomePath + "/ArcGIS/Runtime/UnitTests/images/On.png"
        width: 40 * scaleFactor
        height: width
        anchors {
           right: parent.right
           bottom: parent.bottom
        }
        MouseArea {
            anchors.fill: parent

            onClicked: {
                mapView.locationDisplay.start();
                mapView.locationDisplay.autoPanMode = Enums.LocationDisplayAutoPanModeRecenter;
                mapView.calloutData.showCalloutForLocationDisplay = true;
                mapView.calloutData.title = "Current Location";
                mapView.calloutData.detail = "x: " + mapView.locationDisplay.mapLocation.x +  " y: " + mapView.locationDisplay.mapLocation.y;
                callout.showCallout();
            }
        }
    }

    Callout {
        id: callout
        anchors.fill: parent
        calloutData: mapView.calloutData
        z: 100
    }
}
