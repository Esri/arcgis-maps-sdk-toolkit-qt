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

    height: 80 * scaleFactor
    width: 80 * scaleFactor

    Rectangle {
        anchors.fill: parent
        border {
            width: 5
            color: "#808080"
        }
        radius: 100
        opacity: 0.85
        color: "#55FFFFFF"
        // should be === 0 but when zooming, heading will adjust by a miniscule amount
        visible: controller.autoHide && (controller.heading < 1e-05 || controller.heading === 360) ? false : true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                controller.heading = 0;
            }
        }

        Image {
            anchors {
                centerIn: parent
            }

            height: compassWindow.height * (3.5 / 5)
            width: compassWindow.width * (3.5 / 5)
            source: "qrc:/qt-project.org/imports/Esri/ArcGISRuntime/Toolkit/Controls/images/NorthArrow.png" // Will need to change because this path isn't defined for earlier qt versions
            fillMode: Image.PreserveAspectFit
            rotation: -1 * controller.heading
        }
    }
}
