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
    enabled: controller.initialStartStep !== -1

    property bool playbackLoop: true
    property bool playbackReverse: false
    property bool animateReverse: false
    property bool needsRestart: false

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
    }

    property alias numberOfSteps : controller.numberOfSteps
    property real stepSize: bar.width / (numberOfSteps -1)

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
            controller.setStartAndEndIntervals(Math.max(controller.startStep - 1, 0),
                                               Math.max(controller.endStep - 1, 0));
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
            var newStart = -1;
            var newEnd = -1;
            var atEnd = false;

            if (needsRestart) {
                newStart = 0;
                newEnd = controller.endStep - controller.startStep;
                needsRestart = false;
                atEnd = newEnd === controller.numberOfSteps -1;
            } else {
                var delta = animateReverse ? -1 : 1;
                newStart = controller.startStep + delta;
                newEnd = controller.endStep + delta;

                atEnd = newStart === 0 || newEnd === controller.numberOfSteps -1;
            }

            if (newStart === -1 || newEnd === -1) {
                playButton.checked = false;
                return;
            }

            controller.setStartAndEndIntervals(newStart, newEnd);

            if (!atEnd)
                return;

            if (!playbackLoop)
                playButton.checked = false;
            else if (playbackReverse)
                animateReverse = !animateReverse;
            else
                needsRestart = true;
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
            controller.setStartAndEndIntervals(Math.min(controller.startStep + 1, controller.numberOfSteps -1),
                                               Math.min(controller.endStep + 1, controller.numberOfSteps -1));
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

        Row {
            anchors {
                top: bar.top
                left: bar.left
                right: bar.right
            }
            spacing: (bar.width - numberOfSteps) / (numberOfSteps -1)
            Repeater {
                id: steps
                model: numberOfSteps
                Rectangle {
                    width: 1; height: bar.height * 2
                    border.width: 1
                    color: "black"
                }
            }
        }

        Rectangle {
            id: startThumb

            anchors {
                verticalCenter: bar.verticalCenter
            }

            width: 32
            height: width
            radius: width

            color: startDrag.drag.active ? "transparent" : "red"

            x: (controller.startStep * stepSize) - (width * 0.5)

            MouseArea {
                id: startDrag
                anchors.fill: parent

                drag.target: startDragProxy
                drag.axis: Drag.XAxis
                drag.minimumX: - (startThumb.x + startDragProxy.halfWidth)
                drag.maximumX: endThumb.x - startThumb.x

                onReleased: {
                    var barPos = mapToItem(bar, startDragProxy.x + (startDragProxy.halfWidth), startDragProxy.y);
                    var newStep = barPos.x / stepSize;
                    startDragProxy.x = 0;

                    controller.setStartInterval(newStep);
                }

                Rectangle {
                    id: startDragProxy
                    property real halfWidth: width * 0.5
                    visible: startDrag.drag.active

                    width: startThumb.width
                    height: startThumb.height
                    radius: startThumb.radius

                    color: "red"
                }
            }
        }

        Rectangle {
            id: endThumb

            anchors {
                verticalCenter: bar.verticalCenter
            }

            width: startThumb.width
            height: width
            radius: width

            color: endDrag.drag.active ? "transparent" : "red"

            x: (controller.endStep * stepSize) - (width * 0.5)

            MouseArea {
                id: endDrag
                anchors.fill: parent

                drag.target: endDragProxy
                drag.axis: Drag.XAxis
                drag.minimumX: startThumb.x - endThumb.x
                drag.maximumX: bar.width - (endThumb.x + endDragProxy.halfWidth)

                onReleased: {
                    var barPos = mapToItem(bar, endDragProxy.x + (endDragProxy.halfWidth), endDragProxy.y);
                    var newStep = barPos.x / stepSize;
                    endDragProxy.x = 0;

                    controller.setEndInterval(newStep);
                }

                Rectangle {
                    id: endDragProxy
                    property real halfWidth: width * 0.5
                    visible: endDrag.drag.active

                    width: endThumb.width
                    height: endThumb.height
                    radius: endThumb.radius

                    color: "red"
                }
            }
        }
    }
}
