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

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
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
    enabled: controller.startStep !== -1
    clip: true
    height: backgroundRectangle.height

    property real scaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    property string fontFamily : "helvetica"
    property int pixelSize : 12
    property color backgroundColor: "lightgrey"

    property alias backgroundOpacity: backgroundRectangle.opacity

    property string fullExtentLabelLocale: ""
    property int fullExtentLabelFormat: Locale.LongFormat
    property alias fullExtentFillColor: bar.color

    property string currentExtentLabelLocale: ""
    property int currentExtentLabelFormat: Locale.LongFormat
    property alias currentExtentFillColor: currentExtentFill.color

    property color thumbFillColor: "white"
    property color thumbBorderColor: "black"

    property bool playbackLoop: true
    property bool playbackReverse: false
    property bool animateReverse: false
    property bool needsRestart: false

    Rectangle {
        id: backgroundRectangle
        anchors{
            top: playButton.top
            bottom: slider.bottom
            left: parent.left
            right: parent.right
        }
        color: backgroundColor
    }

    property alias geoView: controller.geoView
    TimeSliderController {
        id: controller
    }

    property real stepSize: bar.width / (controller.numberOfSteps -1)

    Label {
        id: startExtentLabel
        anchors {
            top: parent.top
            left: parent.left
            margins: 4 * scaleFactor
        }

        font {
            family: fontFamily
            pixelSize: root.pixelSize * scaleFactor
        }

        text: controller.fullExtent ? controller.fullExtent.startTime.toLocaleDateString(Qt.locale(fullExtentLabelLocale), fullExtentLabelFormat)
                                    : ""
    }

    Label {
        id: endExtentLabel
        anchors {
            top: startExtentLabel.top
            right: parent.right
            margins: 4 * scaleFactor
        }

        text: controller.fullExtent ? controller.fullExtent.endTime.toLocaleDateString(Qt.locale(fullExtentLabelLocale), fullExtentLabelFormat)
                                    : ""

        font {
            family: fontFamily
            pixelSize: root.pixelSize * scaleFactor
        }
    }

    Button {
        id: backButton
        anchors {
            right: playButton.left
            top: parent.top
            margins: 4 * scaleFactor
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
            horizontalCenter: slider.horizontalCenter
            top: parent.top
            margins: 4 * scaleFactor
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
            top: parent.top
            margins: 4 * scaleFactor
        }
        text: "+"

        onClicked: {
            controller.setStartAndEndIntervals(Math.min(controller.startStep + 1, controller.numberOfSteps -1),
                                               Math.min(controller.endStep + 1, controller.numberOfSteps -1));
        }
    }

    Item {
        id: slider

        anchors {
            top: playButton.bottom
            left: parent.left
            right: parent.right
            margins: 16 * scaleFactor
        }

        height: childrenRect.height

        Rectangle {
            id: bar
            anchors {
                top: slider.top
                left: slider.left
                right: slider.right
            }

            height: 8 * scaleFactor
            color: "darkgray"
            border {
                color: "black"
                width: 1 * scaleFactor
            }
        }

        Row {
            id: tickMarksRow
            anchors {
                top: bar.bottom
                left: bar.left
                right: bar.right
            }
            property int stepsWidth: 1 * scaleFactor
            spacing: controller.numberOfSteps === -1 ? 0 :
                                                       (bar.width - (controller.numberOfSteps * stepsWidth)) / (controller.numberOfSteps -1)

            Repeater {
                id: steps
                model: controller.numberOfSteps === -1 ? 0 : controller.numberOfSteps
                Rectangle {
                    width: tickMarksRow.stepsWidth
                    height: bar.height * 0.5
                    color: "black"
                }
            }
        }

        Rectangle {
            id: currentExtentFill

            visible: !startDrag.drag.active && !endDrag.drag.active

            anchors {
                top: bar.top
                bottom: bar.bottom
                left: startThumb.horizontalCenter
                right: endThumb.horizontalCenter
            }

            color: "black"
        }

        Rectangle {
            id: startThumb

            anchors.verticalCenter: bar.verticalCenter

            width: 16 * scaleFactor
            height: width
            radius: width

            color: startDrag.drag.active ? "transparent" : thumbFillColor
            border {
                color: startDrag.drag.active ? "transparent" : thumbBorderColor
                width: 1 * scaleFactor
            }

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

                    color: thumbFillColor
                    border {
                        color: thumbBorderColor
                        width: 1 * scaleFactor
                    }
                }
            }
        }

        Rectangle {
            id: endThumb

            anchors.verticalCenter: bar.verticalCenter

            width: startThumb.width
            height: width
            radius: width

            color: endDrag.drag.active ? "transparent" : thumbFillColor
            border {
                color: endDrag.drag.active ? "transparent" : thumbBorderColor
                width: 1 * scaleFactor
            }

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

                    color: thumbFillColor
                    border {
                        color: thumbBorderColor
                        width: 1 * scaleFactor
                    }
                }
            }
        }

        Label {
            id: currentStartLabel
            visible: !startDrag.drag.active && controller.startStep !== controller.numberOfSteps -1 && controller.startStep !== 0
            anchors {
                top: startThumb.bottom
                right: startThumb.horizontalCenter
            }

            font {
                family: fontFamily
                pixelSize: root.pixelSize * scaleFactor
            }

            text: controller.currentExtent ? controller.currentExtent.startTime.toLocaleDateString(Qt.locale(currentExtentLabelLocale), currentExtentLabelFormat)
                                           : ""
            elide: Text.ElideLeft
        }

        Label {
            id: currentEndLabel
            visible: !endDrag.drag.active && controller.endStep !== controller.numberOfSteps -1 && controller.endStep !== 0
            anchors {
                top: endThumb.bottom
                left: endThumb.horizontalCenter
            }

            font {
                family: fontFamily
                pixelSize: root.pixelSize * scaleFactor
            }

            text: controller.currentExtent ? controller.currentExtent.endTime.toLocaleDateString(Qt.locale(currentExtentLabelLocale), currentExtentLabelFormat)
                                           : ""
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignRight
        }
    }
}
