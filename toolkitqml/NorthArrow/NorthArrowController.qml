import QtQuick 2.0
import Esri.ArcGISRuntime 100.6

Item {
    property MapView mapView: null

    property int rotation: mapView ? mapView.mapRotation : 0

    function resetRotation() {
         mapView.setViewpointRotation(0);
    }
}
