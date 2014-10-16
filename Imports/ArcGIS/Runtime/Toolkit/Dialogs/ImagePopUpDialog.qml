// Copyright 2014 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.0

Item {
    id: dialogComponent
    anchors.fill: parent
    property string platform: Qt.platform.os
    property double scalefactor: System.displayScaleFactor
    property string portBackground

    PropertyAnimation {
        target: dialogComponent
        property: "opacity"
        duration: 400
        from: 0
        to: 1
        easing.type: Easing.InOutQuad
        running: true
    }

    Rectangle {
        anchors.fill: parent
        id: overlay
        color: "#000000"
        opacity: 0.6
        MouseArea {
            anchors.fill: parent
            onClicked: {
                // make the image and background go away
                dialogComponent.visible = false;
            }
        }
    }

    Rectangle {
        id: portBackground2
        anchors.fill: parent
        color: "transparent"
        visible: true

        Image{
            anchors.fill:parent
            fillMode: Image.PreserveAspectFit
            source: portBackground
            onSourceChanged: console.log("new source",source)
        }
    }
}
