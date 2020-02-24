import QtQuick 2.12

import Esri.ArcGISRuntime.Toolkit 100.7

Item {
    property alias geoView: controller.geoView
    property bool autoHide: true

    NorthArrowController {
        id: controller
        geoView: geoView
    }

    Binding {
        when: geoView !== null
        target: compassImage
        property: "rotation"
        value: -controller.heading
    }

    Image {
        id: compassImage
        anchors.fill: parent
        source: "images/compass.png"
        fillMode: Image.PreserveAspectFit
        visible: autoHide && (controller.heading < 1e-05 || controller.heading === 360) ? false : true
    }

    MouseArea {
        anchors.fill: parent
        onDoubleClicked: {
            if (geoView)
                controller.heading = 0;
        }
    }
}
