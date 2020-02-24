import QtQuick 2.12

import Esri.ArcGISRuntime.Toolkit 100.7

Item {
    id: compassView
    property var geoView;
    property bool autoHide: false

    property var controller: NorthArrowController {
    }

    // Two way binding
    Binding {
        target: compassView
        property: "geoView"
        value: controller.geoView
    }

    Binding {
        target: controller
        property: "geoView"
        value: compassView.geoView
    }

    Binding {
        target: compassImage
        property: "rotation"
        value: isNaN(controller.heading) ? 0 : -controller.heading
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
                controller.setHeading(0);
        }
    }
}
