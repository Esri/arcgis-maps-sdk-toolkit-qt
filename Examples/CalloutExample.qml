/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtPositioning 5.3
import QtSensors 5.3
import Esri.ArcGISRuntime 100.5
import Esri.ArcGISExtras 1.1
import Esri.ArcGISRuntime.Toolkit.Controls 100.5

Rectangle {
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
                targetScale: 2e6
            }
        }

        // set the location display's position source
        locationDisplay {
            positionSource: PositionSource {
            }
            compass: Compass {}
        }

        onMouseClicked: {
            clickedPoint = mouse.mapPoint;

            console.log("x = ", mouse.x, " y = ", mouse.y);
            // call identify on the feature layer
            mapView.identifyLayerWithMaxResults(featureLayer, mouse.x, mouse.y, 10, Enums.IdentifyReturnsGeoElementsOnly, 1);
        }

        onIdentifyLayerStatusChanged: {
            if (identifyLayerStatus === Enums.TaskStatusCompleted) {
                if (identifyLayerResult.geoElements.length > 0) {
                    var geoElement = identifyLayerResult.geoElements[0];
                    featureLayer.clearSelection();
                    featureLayer.selectFeature(geoElement);
                    mapView.calloutData.geoElement = geoElement;
                    console.log(geoElement.attributes);
                    mapView.calloutData.title = geoElement.attributes.attributeValue("objectid");
                    mapView.calloutData.detail = geoElement.attributes.attributeValue("typdamage");
                } else {
                    console.log("No identify results");
                    var spatialRef = ArcGISRuntimeEnvironment.createObject("SpatialReference", {wkid: 4326});
                    var point = GeometryEngine.project(clickedPoint, spatialRef);

                    //no identify results, so display map location
                    mapView.calloutData.location = clickedPoint;
                    mapView.calloutData.title = "Location";
                    mapView.calloutData.detail = "Lat: " + point.y.toFixed(2) + " Long: " + point.x.toFixed(2);
                    mapView.calloutData.imageUrl = "images/RedShinyPin.png";
                }
                callout.showCallout();
            }
        }

        Callout {
            id: callout
            //anchors.fill: parent
            calloutData: mapView.calloutData
            z: 100
        }
    }

    Image {
        source: "images/On.png"
        width: 40 * System.displayScaleFactor
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
}
