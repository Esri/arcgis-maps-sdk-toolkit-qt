/*******************************************************************************
 *  Copyright 2012-2018 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0
import "LeaderPosition.js" as Enums

/*!
    \qmltype TimeSlider
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.3
    \brief
*/
Item {
    id: root
    x: 0
    y: 0
    width: 800
    height: implicitHeight
    property bool isInstant: instantCB.checked

    onIsInstantChanged: {
        if (!isInstant && thumb1.x > thumb2.x)
            thumb1.x = thumb2.x;
    }

    property int layerMinX : 100
    property int layerMaxX : 500
    property int intervals : 25

    CheckBox {
        id: instantCB
        anchors {
            top: parent.top
            right: parent.right
        }
        text: "is instant"
        checked: false
    }

    Label {
        id: startExtentLabel
        anchors {
            bottom: bar.top
            left: parent.left
        }

        text: "June 22, 2008"
    }

    Label {
        id: endExtentLabel
        anchors {
            bottom: bar.top
            right: parent.right
        }

        text: "November 23, 2008"
    }

    Button {
        id: playButton
        anchors {
            horizontalCenter: bar.horizontalCenter
            bottom: bar.top
            margins: 16
        }

        text: ">"
        checkable: true
        checked: false
    }

    Timer {
        id: playAnimation
        running: playButton.checked
        repeat: true

        onTriggered: {
            thumb2.x = thumb2.x + (bar.width / intervals);
            thumb1.x = thumb1.x + (bar.width / intervals);
        }
    }

    Rectangle {
        id: layerExtentFill
        color: "lightsteelblue"
        anchors {
            verticalCenter: bar.verticalCenter
        }
        height: bar.height
        x: bar.x + layerMinX
        width: layerMaxX - layerMinX
    }

    Rectangle {
        id: currentExtentFill
        color: "darkblue"
        anchors {
            verticalCenter: bar.verticalCenter
            left: isInstant ? thumb2.horizontalCenter : thumb1.horizontalCenter
            right: thumb2.horizontalCenter
        }
        height: bar.height
    }

    Rectangle {
        id: bar
        anchors {
            bottom: thumb1Label.top
            left: startExtentLabel.horizontalCenter
            right: endExtentLabel.horizontalCenter
        }

        height: 8
        color: "transparent"
        border {
            color: "black"
            width: 2
        }
    }

    Row {
        anchors {
            top: bar.top
            left: bar.left
            right: bar.right
        }
        spacing: width / intervals
        Repeater {
            model: intervals
            Rectangle {
                width: 1; height: bar.height * 2
                border.width: 1
                color: "black"
            }
        }
    }

    Rectangle {
        id: thumb1
        anchors {
            verticalCenter: bar.verticalCenter
        }
        visible: !isInstant
        height: 32
        width: height
        x: bar.x
        property int halfWidth: width * 0.5
        property bool inBar: (x + halfWidth >= bar.x) && x <= thumb2.x

        color: "red"

        Drag.active: thum1DragArea.drag.active
        Drag.hotSpot.x: halfWidth
        Drag.hotSpot.y: halfWidth

        onXChanged: {
            if ((x + halfWidth) < bar.x)
                x = bar.x - halfWidth;
            else if (x > thumb2.x)
                x = thumb2.x;
        }

        MouseArea {
            id: thum1DragArea
            anchors.fill: parent

            drag.target: parent.inBar ? parent : null
        }
    }

    Rectangle {
        id: thumb2
        anchors {
            verticalCenter: bar.verticalCenter
        }
        height: 32
        width: height
        x: bar.x
        property int minX: isInstant ? bar.x - halfWidth : thumb1.x
        property int halfWidth: width * 0.5
        property bool inBar: x >= minX && x - halfWidth <= (bar.x + bar.width)

        color: "blue"

        Drag.active: thum2DragArea.drag.active
        Drag.hotSpot.x: halfWidth
        Drag.hotSpot.y: halfWidth

        onXChanged: {
            if (x < minX)
                x = minX;
            else if ((x + halfWidth) > (bar.x + bar.width))
                x = (bar.x + bar.width) - halfWidth;
        }

        MouseArea {
            id: thum2DragArea
            anchors.fill: parent

            drag.target: parent.inBar ? parent : null
        }
    }

    Label {
        id: thumb1Label
        visible: thumb1.visible
        anchors {
            bottom: parent.bottom
            right : thumb1.left
        }
        text: "June 27, 2008"
        horizontalAlignment: Text.AlignHCenter
    }

    Label {
        id: thumb2Label
        visible: !isInstant
        anchors {
            bottom: parent.bottom
            left: thumb2.right
        }
        text: "June 27, 2008"
    }

    Label {
        id: thumb2LabelB
        visible: isInstant
        anchors {
            bottom: parent.bottom
            horizontalCenter: thumb2.horizontalCenter
        }
        text: "June 27, 2008"
    }
}
