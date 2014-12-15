/*******************************************************************************
 * Copyright 2012-2015 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.0
import QtQuick.Window 2.2

Item {
    id: dialogComponent
    anchors.fill: parent
    property string platform: Qt.platform.os
    property double scalefactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
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
