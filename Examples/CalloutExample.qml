
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

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent

        // add a map to the mapview
        Map {
            // add the streets basemap to the map
            BasemapStreets {}
        }

        onMouseClicked: {
            var spatialRef = ArcGISRuntimeEnvironment.createObject("SpatialReference", {wkid: 4326});
            var point = GeometryEngine.project(mouse.mapPoint, spatialRef);

            mapView.calloutData.location = mouse.mapPoint;
            mapView.calloutData.title = "Location";
            mapView.calloutData.detail = "Lat: " + point.y.toFixed(2) + " Long: " + point.x.toFixed(2);
            callout.showCallout();
        }
    }

    Callout {
        id: callout
        anchors.fill: parent
        calloutData: mapView.calloutData
        z: 100
    }
}
