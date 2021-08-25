import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import Esri.ArcGISRuntime.Toolkit.Controller 100.13

Control {
    id: scaleLine

    /*!
  \qmlproperty ScalelineController controller
  \brief
  */
    property var controller: null

    property int unitSystem

    spacing: 5
    implicitWidth: 100
    implicitHeight: font.pixelSize + spacing

    contentItem: Shape {
        anchors.fill:parent
        ShapePath {
            strokeColor: "black"
            strokeWidth: 3
            fillColor: Qt.rgba(255, 255, 255, 0.5)
            startX: 0; startY: scaleLine.height
            PathLine { x: 0; y: 0 }
            PathLine { x: internal.displayWidth; y: 0 }
            PathLine { x: internal.displayWidth; y: scaleLine.height }
        }
        Label {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: scaleLine.spacing
            }
            font: scaleLine.font
            palette: scaleLine.palette
            text: internal.displayDistance
        }
    }

    Component.onCompleted: {
        if (controller.mapView) {
            internal.calculateScale();
        }
    }

    QtObject {
        id: internal

        property real displayWidth
        property string displayDistance

        function calculateScale() {
            // Get the total scalebar width in pixels.
            const availableWidth = scaleLine.width;
            // Get the possible longest distance the scalebar would cover.
            const maxDistance = controller.calculateDistance(availableWidth);
            // Select the most appropriate distance the scalebar should cover.
            const displayDistance = controller.calculateBestScalebarLength(maxDistance);
            // Calculate the new displayed scalebar width.
            internal.displayWidth = controller.calculateDisplayWidth(displayDistance, maxDistance, availableWidth);
            // Display the value of the calculated distance.
            internal.displayDistance = controller.calculateDistanceInDisplayUnits(displayDistance, controller.unitSystem);
        }

        property Connections geoViewConnections: Connections {
            target: controller.mapView
            function onViewpointChanged() {
                internal.calculateScale();
            }
        }
    }
}
