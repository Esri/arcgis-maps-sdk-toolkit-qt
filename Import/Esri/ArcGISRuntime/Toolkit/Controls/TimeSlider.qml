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
import Esri.ArcGISRuntime 100.3

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
    height: implicitHeight
    property bool isInstant: instantCB.checked
    enabled: controller.initialStartStep !== -1

    Rectangle {
        anchors.centerIn: parent
        width: parent.width
        height: 200
        radius: 8
        color: "white"
        opacity: 0.5
    }

    property alias geoView: controller.geoView
    TimeSliderController {
        id: controller

        onInitialStartStepChanged: {
            console.log("initial start step", initialStartStep);
            thumb1.x = (initialStartStep * stepSize) + thumb1.halfWidth;
        }

        onInitialEndStepChanged: {
            console.log("initial end step", initialEndStep);
            thumb2.x = (initialEndStep * stepSize) + thumb2.halfWidth;
        }
    }

    onIsInstantChanged: {
        if (!isInstant && thumb1.x > thumb2.x)
            thumb1.x = thumb2.x;
    }

    property alias numberOfSteps : controller.numberOfSteps
    property int stepSize: bar.width / numberOfSteps

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

        text: controller.fullExtent ? controller.fullExtent.startTime : ""
    }

    Label {
        id: endExtentLabel
        anchors {
            bottom: bar.top
            right: parent.right
        }

        text: controller.fullExtent ? controller.fullExtent.endTime : ""
    }

    Button {
        id: backButton
        anchors {
            right: playButton.left
            bottom: bar.top
            margins: 16
        }
        text: "-"

        onClicked: {
            thumb1.x -= stepSize;
            thumb2.x -= stepSize;
            controller.setStartAndEndIntervals(thumb1.x/stepSize, thumb2.x/stepSize);
        }
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
            thumb1.x += stepSize;
            thumb2.x += stepSize;
            controller.setStartAndEndIntervals(thumb1.x/stepSize, thumb2.x/stepSize);
        }
    }

    Button {
        id: forwardsButton
        anchors {
            left: playButton.right
            bottom: bar.top
            margins: 16
        }
        text: "+"

        onClicked: {
            thumb1.x += stepSize;
            thumb2.x += stepSize;
            controller.setStartAndEndIntervals(thumb1.x/stepSize, thumb2.x/stepSize);
        }
    }

    Rectangle {
        id: bar
        anchors {
            bottom: parent.bottom
            left: startExtentLabel.horizontalCenter
            right: endExtentLabel.horizontalCenter
        }

        height: 8
        color: "transparent"
        border {
            color: "black"
            width: 2
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

        Row {
            anchors {
                top: bar.top
                left: bar.left
                right: bar.right
            }
            spacing: stepSize
            Repeater {
                model: numberOfSteps
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

            property int halfWidth: width * 0.5
            property bool inBar: (x + halfWidth >= bar.x) && x <= thumb2.x

            color: "red"

            Drag.active: thum1DragArea.drag.active
            Drag.hotSpot.x: halfWidth
            Drag.hotSpot.y: halfWidth

            onXChanged: {
                if (x < 0)
                    x = 0;
                var draggedX = x;
                if (draggedX <0)
                    draggedX = 0;
                if (draggedX > thumb2.x)
                    draggedX = thumb2.x;

                draggedX = Math.round(x/stepSize) * stepSize;
                draggedX += halfWidth;
                controller.setStartInterval(draggedX/stepSize);
            }

            MouseArea {
                id: thum1DragArea
                anchors.fill: parent

                drag.target: parent
            }
        }

        Rectangle {
            id: thumb2
            anchors {
                verticalCenter: bar.verticalCenter
            }
            height: 32
            width: height

            property int minX: isInstant ? bar.x - halfWidth : thumb1.x
            property int halfWidth: width * 0.5
            property bool inBar: x >= minX && x - halfWidth <= (bar.x + bar.width)

            color: "blue"

            Drag.active: thum2DragArea.drag.active
            Drag.hotSpot.x: halfWidth
            Drag.hotSpot.y: halfWidth

            onXChanged: {
                if (!Drag.active)
                    return;

                var draggedX = x;
                if (draggedX < minX)
                    draggedX = minX;
                else if ((draggedX + halfWidth) > (bar.x + bar.width))
                    draggedX = (bar.x + bar.width) - halfWidth;

                draggedX = Math.round(draggedX/stepSize) * stepSize;
                draggedX += halfWidth;
                controller.setEndInterval(draggedX/stepSize);
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
            text: controller.currentExtent ? controller.currentExtent.startTime : ""
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: thumb2Label
            visible: !isInstant
            anchors {
                bottom: parent.bottom
                left: thumb2.right
            }
            text: controller.currentExtent ? controller.currentExtent.endTime : ""
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
}
