import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISArToolkit 1.0

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
