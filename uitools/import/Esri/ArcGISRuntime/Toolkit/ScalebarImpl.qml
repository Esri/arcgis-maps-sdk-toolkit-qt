import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import Esri.ArcGISRuntime.Toolkit.Controller 100.13

/*!
  \internal
  This is the display portion of a Scalebar. A single scalebar can be
  composed of multiple display portions (i.e. showing imperial and metric scalebars side
  by side).
 */
Control {
    id: scalebar

    /*!
      \qmlproperty ScalebarController controller
      \brief The controller used for calculations based on the mapView.
    */
    property var controller: null

    /*!
      \qmlproperty unitSystem
      \brief UnitSystem for the Scalebar. For this display portion,
      only metric and imperial are valid values.
    */
    property int unitSystem

    contentItem: Shape {
        anchors.fill:parent
        ShapePath {
            strokeColor: "black"
            strokeWidth: 3
            fillColor: Qt.rgba(255, 255, 255, 0.5)
            startX: 0; startY: 0
            PathLine { x: 0; y: scalebar.height }
            PathLine { x: internal.displayWidth; y: scalebar.height }
            PathLine { x: internal.displayWidth; y: 0 }
        }
        Label {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: scalebar.spacing
            }
            font: scalebar.font
            palette: scalebar.palette
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
            const availableWidth = scalebar.width;
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
