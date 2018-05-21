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

    property color textColor: "black"
    property string fontFamily : "helvetica"
    property int pixelSizeInDips : 12
    property color backgroundColor: "lightgrey"

    property alias backgroundOpacity: backgroundRectangle.opacity
    property alias radius: backgroundRectangle.radius

    property string fullExtentLabelLocale: ""
    property int fullExtentLabelFormat: Locale.LongFormat
    property alias fullExtentFillColor: sliderBar.color

    property string currentExtentLabelLocale: ""
    property int currentExtentLabelFormat: Locale.NarrowFormat
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
            top: parent.top
            bottom: slider.bottom
            left: parent.left
            right: parent.right
        }
        color: backgroundColor
    }

    property alias geoView: controller.geoView
    TimeSliderController {
        id: controller

        onStartStepChanged: {
            if (startStep < 0 || numberOfSteps === -1)
                return;

            if (slider.first.value !== startStep)
                slider.setValues(startStep, endStep);
        }

        onEndStepChanged: {
            if (endStep < 0 || numberOfSteps === -1)
                return;

            if (slider.second.value !== endStep) {
                slider.setValues(startStep, endStep);
            }
        }
    }

    property real stepSize: sliderBar.width / (controller.numberOfSteps -1)

    Label {
        id: startExtentLabel
        anchors {
            top: playButton.top
            left: parent.left
            margins: 4 * scaleFactor
        }

        font {
            family: fontFamily
            pixelSize: root.pixelSizeInDips * scaleFactor
        }
        color: textColor
        text: controller.fullExtent ? controller.fullExtent.startTime.toLocaleDateString(Qt.locale(fullExtentLabelLocale), fullExtentLabelFormat)
                                    : ""
    }

    Label {
        id: endExtentLabel
        anchors {
            top: playButton.top
            right: parent.right
            margins: 4 * scaleFactor
        }

        color: textColor
        text: controller.fullExtent ? controller.fullExtent.endTime.toLocaleDateString(Qt.locale(fullExtentLabelLocale), fullExtentLabelFormat)
                                    : ""

        font {
            family: fontFamily
            pixelSize: root.pixelSizeInDips * scaleFactor
        }
    }

    Button {
        id: backButton
        anchors {
            right: playButton.left
            verticalCenter: playButton.verticalCenter
            margins: 16 * scaleFactor
        }
        height: width
        width: playButton.width

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: "images/step.png"
            rotation: 180
        }

        contentItem: Text {
            text: backButton.text
            anchors.centerIn: parent
            font {
                bold: true
                family: fontFamily
                pixelSize: root.pixelSizeInDips * scaleFactor
            }
            color: textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: width
            radius: width
            color: fullExtentFillColor
        }

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
        height: width
        width: 24 * scaleFactor

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: playButton.checked ? "images/pause.png" : "images/play.png"
        }

        checkable: true
        checked: false

        contentItem: Text {
            text: playButton.text
            anchors.centerIn: parent
            font {
                bold: true
                family: fontFamily
                pixelSize: root.pixelSizeInDips * scaleFactor
            }
            color: textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: width
            radius: width
            color: playButton.checked ? backgroundColor : fullExtentFillColor
        }
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
            verticalCenter: playButton.verticalCenter
            margins: 16 * scaleFactor
        }
        height: width
        width: playButton.width

        Image {
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: "images/step.png"
        }

        contentItem: Text {
            text: forwardsButton.text
            anchors.centerIn: parent
            font {
                bold: true
                family: fontFamily
                pixelSize: root.pixelSizeInDips * scaleFactor
            }
            color: textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: width
            radius: width
            color: fullExtentFillColor
        }

        onClicked: {
            controller.setStartAndEndIntervals(Math.min(controller.startStep + 1, controller.numberOfSteps -1),
                                               Math.min(controller.endStep + 1, controller.numberOfSteps -1));
        }
    }

    RangeSlider {
        id: slider

        anchors {
            top: playButton.bottom
            left: parent.left
            right: parent.right
            leftMargin: 16 * scaleFactor
            rightMargin: 16 * scaleFactor
            topMargin: 8 * scaleFactor
        }

        from: 0
        to: controller.numberOfSteps

        onToChanged: {
            if (to === -1)
                return;

            setValues(controller.startStep, controller.endStep);
        }

        stepSize: 1.0
        snapMode: RangeSlider.SnapAlways

        height: 32 * scaleFactor

        background: Rectangle {
            id: sliderBar
            anchors {
                top: slider.top
                left: slider.left
                right: slider.right
            }
            height: 8 * scaleFactor
            radius: 2 * scaleFactor
            color: "darkgray"
            border {
                color: "black"
                width: 0.5 * scaleFactor
            }

            Rectangle {
                id: currentExtentFill
                anchors.top: parent.top
                x: slider.first.visualPosition * parent.width
                width: slider.second.visualPosition * parent.width - x
                height: parent.height
                color: "black"
                radius: 2 * scaleFactor
            }

            Row {
                id: tickMarksRow
                anchors {
                    top: sliderBar.bottom
                    left: sliderBar.left
                    right: sliderBar.right
                }
                property int stepsWidth: 1 * scaleFactor
                spacing: controller.numberOfSteps === -1 ? 0 :
                                                           (sliderBar.width - (controller.numberOfSteps * stepsWidth)) / (controller.numberOfSteps -1)

                Repeater {
                    id: steps
                    model: controller.numberOfSteps === -1 ? 0 : controller.numberOfSteps
                    Rectangle {
                        width: tickMarksRow.stepsWidth
                        height: index % 10 === 0 ? sliderBar.height : sliderBar.height * 0.5
                        color: tickMarksRow.spacing < (5 * scaleFactor) ? (index % 5 === 0 ? "black" : "transparent")
                                                                        : "black"
                    }
                }
            }

            Label {
                id: combinedLabel
                visible: (slider.second.visualPosition - slider.first.visualPosition) < 0.4
                anchors {
                    left: sliderBar.left
                    right: sliderBar.right
                }
                y: (sliderBar.height * 0.5) + 0.5 * startThumb.height
                leftPadding: Math.min(currentExtentFill.x, sliderBar.width * 0.5)

                text: controller.startStep === controller.endStep ? currentStartLabel.text
                                                                  : currentStartLabel.text + " - " + currentEndLabel.text

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips * scaleFactor
                }
            }
        }

        first.handle: Rectangle {
            id: startThumb
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.first.visualPosition * parent.width) - (width * 0.5)

            width: 16 * scaleFactor
            height: width
            radius: width
            color: thumbFillColor
            border.color: thumbBorderColor

            Label {
                id: currentStartLabel
                visible: !combinedLabel.visible && controller.startStep !== controller.numberOfSteps -1 && controller.startStep !== 0
                anchors {
                    top: startThumb.bottom
                    horizontalCenter: startThumb.horizontalCenter
                }

                leftPadding: (slider.width * slider.first.visualPosition) < (48 * scaleFactor) ? 48 * scaleFactor : 0

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips * scaleFactor
                }

                color: textColor
                text: controller.currentExtent ? controller.currentExtent.startTime.toLocaleDateString(Qt.locale(currentExtentLabelLocale), currentExtentLabelFormat)
                                               : ""
                elide: Text.ElideLeft
            }
        }

        second.handle: Rectangle {
            id: endThumb
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.second.visualPosition * parent.width) - (width * 0.5)
            width: 16 * scaleFactor
            height: width
            radius: width
            color: thumbFillColor
            border.color: thumbBorderColor

            Label {
                id: currentEndLabel
                visible: !combinedLabel.visible && (controller.endStep < (controller.numberOfSteps -1)) && controller.endStep !== 0
                anchors {
                    top: endThumb.bottom
                    horizontalCenter: endThumb.horizontalCenter
                }

                rightPadding: (slider.width * slider.second.visualPosition) > slider.width - (48 * scaleFactor) ? 48 * scaleFactor : 0

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips * scaleFactor
                }

                color: textColor
                text: controller.currentExtent ? controller.currentExtent.endTime.toLocaleDateString(Qt.locale(currentExtentLabelLocale), currentExtentLabelFormat)
                                               : ""
                elide: Text.ElideLeft
            }
        }

        first.onValueChanged: {
            if (controller.startStep === slider.first.value)
                return;

            controller.setStartInterval(slider.first.value);
        }

        second.onValueChanged: {
            if (controller.endStep === slider.second.value)
                return;

            controller.setEndInterval(slider.second.value);
        }
    }
}
