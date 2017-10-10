import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISRuntime.Toolkit.CppApi 100.2

Item {
    property real scaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property bool autoHideCompass: true

    // ArcGISCompassController must be registered as a QML type in C++ code
    ArcGISCompassController {
        id: controller
        objectName: "arcGISCompassController"
        autoHide: autoHideCompass
    }

    height: 32 * scaleFactor
    width: 32 * scaleFactor
    opacity: 0.85

    Image {
        anchors.fill: parent
        source: "images/compass.png"
        fillMode: Image.PreserveAspectFit
        rotation: -1 * controller.heading

        // When zooming in a Scene, the heading will adjust by a miniscule amount. Using the < comparison rather than === prevents flickering while zooming
        visible: controller.autoHide && (controller.heading < 1e-05 || controller.heading === 360) ? false : true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                controller.heading = 0;
            }
        }
    }
}
