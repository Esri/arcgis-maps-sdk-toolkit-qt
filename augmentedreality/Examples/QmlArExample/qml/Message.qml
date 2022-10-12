import QtQuick
import QtQuick.Controls
import Esri.ArcGISRuntime
import Esri.ArcGISArToolkit

Rectangle {
    property alias text: label.text
    height: label.height + 10
    color: "#88ffffff" // transparent white
    radius: 5
    Text {
        id: label
        anchors.centerIn: parent
        font.bold: true
    }
}
