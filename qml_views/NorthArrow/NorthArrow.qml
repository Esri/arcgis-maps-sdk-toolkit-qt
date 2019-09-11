import QtQuick 2.0
import esri.arcgisruntime.toolkit.controllers 1.0

Rectangle {

    property alias mapView: controller.mapView

    Image{
        anchors.fill: parent
        source: "qrc:/images/compass.png"
        rotation: - controller.rotation
    }

    NorthArrowController {
        id: controller
    }

    MouseArea{
        anchors.fill: parent
        onDoubleClicked: {
            controller.resetRotation();
        }
    }
}
