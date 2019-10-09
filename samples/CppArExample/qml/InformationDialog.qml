import QtQuick 2.12
import QtQuick.Controls 2.2
import Esri.CppArExample 1.0
import Esri.ArcGISArToolkit 1.0

Rectangle {

    property alias text: title.text

    anchors {
        margins: 10
        bottomMargin: 30
    }
    height: title.height + 20
    color: "#88ffffff"
    radius: 5
    Text {
        id: title
        anchors.centerIn: parent
        font.bold: true
    }
}
