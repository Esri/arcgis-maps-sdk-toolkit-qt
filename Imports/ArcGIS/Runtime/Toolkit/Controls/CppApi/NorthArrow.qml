import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.Samples 1.0
import Esri.ArcGISExtras 1.1

Item {
    id: compassWindow

    property real scaleFactor: System.displayScaleFactor
    signal compassClicked

    NorthArrowController {
        id: controller
        objectName: "northArrowController"
    }

    height: 100 * scaleFactor
    width: 100 * scaleFactor

    Rectangle {
        anchors.fill: parent
        radius: 100
        opacity: 0.85
        border {
            width: 5
            color: "#808080"
        }

        visible: controller.autoHide && controller.heading === 0 ? false : true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                controller.heading = 0;
            }
        }

        color: "#55FFFFFF"

        Image {
            anchors {
                centerIn: parent
            }

            height: compassWindow.height * (4 / 5)
            width: compassWindow.width * (2 / 5)
            source: "NorthArrow.png"
            fillMode: Image.PreserveAspectFit
            rotation: -1 * controller.heading
        }
    }

}
