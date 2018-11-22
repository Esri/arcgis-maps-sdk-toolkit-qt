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
import Esri.ArcGISRuntime.Toolkit.CppApi 100.3

/*!
    \qmltype TimeSlider
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.3
    \brief The slider provides a user interface for manually setting or animating
    changes to the current time extent of the geoView.

    A time slider can be bound to a geoView
    (mapView or sceneView) to allow filtering on temporal data.

    The time extents of all layers in the map will be used to set up the
    slider with the full temporal range and the current time extent.

    If the map/scene does not contain any layers with time support, the
    slider will be disabled.

    Here is an example of how to use this control from QML.

    \code
        // import the toolkit
        import Esri.ArcGISRuntime.Toolkit.CppApi 100.3
        ...

        // add a mapView component (the geoView)
        MapView {
            anchors.fill: parent
            objectName: "mapView"
            id: mapView

            // declare a TimeSlider and bind it to the geoView
            TimeSlider {
                id: timeSlider
                anchors {
                    left: mapView.left
                    right: mapView.right
                    bottom: mapView.attributionTop
                }

                geoView: mapView
            }
        }
    \endcode
*/
Item {
    id: root

    enabled: controller.startStep !== -1
    clip: true
    height: backgroundRectangle.height

    readonly property int labelModeNone: 0
    readonly property int labelModeThumbs: 0
    readonly property int labelModeTicks: 2

    signal currentExtentChanged

    /*!
      \qmlproperty int textColor
      \brief The color of coordinate notation text and labels on this tool.

      The default value is \c "black".
     */
    property color textColor: "black"

    /*!
      \qmlproperty int fontFamily
      \brief The font family for text on this tool.

      The default is \c "helvetica".
     */
    property string fontFamily : "helvetica"

    /*!
      \qmlproperty int pixelSizeInDips
      \brief The font size of for text on this tool in device independant pixels (DIPS).

      The default value is \c 12.
     */
    property int pixelSizeInDips : 12

    /*!
      \qmlproperty int backgroundColor
      \brief The color of used to for background UI elements in this tool.

      The default value is \c "lightgrey".
     */
    property color backgroundColor: "lightgrey"

    /*!
      \qmlproperty real backgroundOpacity
      \brief The opacity of the background rectangle.
      */
    property alias backgroundOpacity: backgroundRectangle.opacity

    /*!
      \qmlproperty real radius
      \brief The radius of the background rectangle.
      */
    property alias radius: backgroundRectangle.radius

    /*!
      \qmlproperty var fullExtentLabelFormat
      \brief The format for displaying \l Date values
      for the full time extent. - for example "yy/MM/dd".

      The default is \c Qt.DefaultLocaleShortDate.

      \sa Qt.formatDateTime
      */
    property var fullExtentLabelFormat

    /*!
      \qmlproperty color fullExtentFillColor
      \brief The color for the full extent of the slider.

      The default is \c "darkgray".
      */
    property alias fullExtentFillColor: sliderBar.color

    /*!
      \qmlproperty var currentExtentLabelFormat
      \brief The format for displaying \l Date values
      for the current time extent. - for example "yy/MM/dd".

      The default is \c Qt.DefaultLocaleShortDate.

      \sa Qt.formatDateTime
      */
    property var currentExtentLabelFormat

    /*!
      \qmlproperty color currentExtentFillColor
      \brief The color for portion of the slider covering
      the current time extent.

      The default is \c "black".
      */
    property alias currentExtentFillColor: currentExtentFill.color

    /*!
      \qmlproperty color thumbFillColor
      \brief The color for the slider thumb controls.

      The default is \c "white".
      */
    property color thumbFillColor: "white"

    /*!
      \qmlproperty color thumbBorderColor
      \brief The color for the border of the slider thumb controls.

      The default is \c "black".
      */
    property color thumbBorderColor: "black"

    /*!
      \qmlproperty int labelMode
      \brief How to apply labels to the Slider.

      Valid options are:
      \list
        \li labelModeNone. No labels are applied
        \li labelModeThumbs. Labels are applied to the slider thumbs.
        \li labelModeTicks. Labels are applied to the slider tick marks.
      \endlist

      The default is \c labelModeThumbs.
      */
    property int labelMode: labelModeThumbs

    /*!
      \qmlproperty int labelSliderTickInterval
      \brief The interval at which slider ticks should be labelled

      The default is \c 20.
      */
    property int labelSliderTickInterval: 20

    /*!
      \qmlproperty bool playbackLoop
      \brief Whether to loop when the animation reaches the
      end of the slider.

      The default is \c "true".
      */
    property bool playbackLoop: true

    /*!
      \qmlproperty bool playbackReverse
      \brief Whether to reverse the animation direction when
      the animation reaches the end of the slider.

      \note This property has no effect if \l playbackLoop
      is \c false.

      The default is \c "false".
      */
    property bool playbackReverse: false

    /*!
      \qmlproperty bool startTimePinned
      \brief Whether the start time of the time window can
      be manipulated

      The default is \c "false".
      */
    property bool startTimePinned: false

    /*!
      \qmlproperty bool endTimePinned
      \brief Whether the end time of the time window can
      be manipulated

      The default is \c "false".
      */
    property bool endTimePinned: false

    /*!
      \qmlproperty int playbackInterval
      \brief The amount of time (in milliseconds) during playback
      that will elapse before the slider advances to the next time step

      The default is \c 500.
      */
    property alias playbackInterval : playAnimation.interval

    /*!
      \qmlproperty var timeStepIntervalLabelFormat
      \brief The date format for displaying time step intervals -
      for example "yy/MM/dd".

      The default is \c Qt.DefaultLocaleShortDate.

      \sa Qt.formatDateTime
      */
    property var timeStepIntervalLabelFormat

    /*!
      /internal
      */
    property bool animateReverse: false

    /*!
      /internal
      */
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

    /*!
      \qmlproperty var geoView
      \brief The GeoView for this tool. Should be a SceneQuickView or a MapQuickView.

      This property is the entry point for the time extent of the geoView itself and
      also for any layers which support time.

      \note this property must be set for the TimeSlider control to function correctly.
     */
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

        onCurrentTimeExtentChanged: currentExtentChanged();
    }

    Label {
        id: startExtentLabel
        anchors {
            top: playButton.top
            left: parent.left
            margins: 4
        }

        font {
            family: fontFamily
            pixelSize: root.pixelSizeInDips
        }
        color: textColor
        text: fullExtentLabelFormat ? Qt.formatDateTime(controller.fullExtentStart, fullExtentLabelFormat)
                                    : Qt.formatDateTime(controller.fullExtentStart)

    }

    Label {
        id: endExtentLabel
        anchors {
            top: playButton.top
            right: parent.right
            margins: 4
        }

        color: textColor
        text: fullExtentLabelFormat ? Qt.formatDateTime(controller.fullExtentEnd, fullExtentLabelFormat)
                                    : Qt.formatDateTime(controller.fullExtentEnd)

        font {
            family: fontFamily
            pixelSize: root.pixelSizeInDips
        }
    }

    Button {
        id: backButton
        anchors {
            right: playButton.left
            verticalCenter: playButton.verticalCenter
            margins: 16
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
                pixelSize: root.pixelSizeInDips
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
            controller.setStartAndEndIntervals(startTimePinned ? controller.startStep
                                                               : Math.max(controller.startStep - 1, 0),
                                               endTimePinned ? controller.endStep
                                                             : Math.max(controller.endStep - 1, 0));
        }
    }

    Button {
        id: playButton
        anchors {
            horizontalCenter: slider.horizontalCenter
            top: parent.top
            margins: 4
        }
        height: width
        width: 24

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
                pixelSize: root.pixelSizeInDips
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
        interval: 500

        onTriggered: {
            var newStart = -1;
            var newEnd = -1;
            var atEnd = false;

            if (needsRestart) {
                newStart = 0;
                newEnd = controller.endStep - controller.startStep;
                needsRestart = false;
                atEnd = newEnd === controller.numberOfSteps - 1;
            } else {
                var delta = animateReverse ? -1 : 1;
                newStart = controller.startStep + delta;
                newEnd = controller.endStep + delta;

                atEnd = (!startTimePinned && (newStart < 0 || newStart > (controller.numberOfSteps - 1))) ||
                        (!endTimePinned && (newEnd < 0 || newEnd > (controller.numberOfSteps - 1)));
            }

            if (!atEnd) {
                controller.setStartAndEndIntervals(startTimePinned ? controller.startStep
                                                                   : newStart,
                                                   endTimePinned ? controller.endStep
                                                                 : newEnd);
                return;
            }

            if (!playbackLoop)
                playButton.checked = false;
            else if (playbackReverse)
                animateReverse = !animateReverse;
            else if (startTimePinned || endTimePinned)
                playButton.checked = false;
            else
                needsRestart = true;
        }
    }

    Button {
        id: forwardsButton
        anchors {
            left: playButton.right
            verticalCenter: playButton.verticalCenter
            margins: 16
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
                pixelSize: root.pixelSizeInDips
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
            controller.setStartAndEndIntervals(startTimePinned ? controller.startStep
                                                               : Math.min(controller.startStep + 1, controller.numberOfSteps - 1),
                                               endTimePinned ? controller.endStep
                                                             : Math.min(controller.endStep + 1, controller.numberOfSteps - 1));
        }
    }

    RangeSlider {
        id: slider

        anchors {
            top: playButton.bottom
            left: parent.left
            right: parent.right
            leftMargin: 16
            rightMargin: 16
            topMargin: 8
        }

        from: 0
        to: controller.numberOfSteps - 1

        onToChanged: {
            if (to < 0)
                return;

            setValues(controller.startStep, controller.endStep);
        }

        stepSize: 1.0
        snapMode: RangeSlider.SnapAlways

        height: 32

        background: Rectangle {
            id: sliderBar
            anchors {
                top: slider.top
                left: slider.left
                right: slider.right
            }
            height: 8
            radius: 2
            color: "darkgray"
            border {
                color: "black"
                width: 0.5
            }

            Rectangle {
                id: currentExtentFill
                anchors.top: parent.top
                x: slider.first.visualPosition * parent.width
                width: slider.second.visualPosition * parent.width - x
                height: parent.height
                color: "black"
                radius: 2
            }

            Row {
                id: tickMarksRow
                anchors {
                    top: sliderBar.bottom
                    left: sliderBar.left
                    right: sliderBar.right
                }
                property int stepsWidth: 1
                spacing: controller.numberOfSteps === -1 ? 0 :
                                                           (sliderBar.width - (controller.numberOfSteps * stepsWidth)) / (controller.numberOfSteps - 1)

                Repeater {
                    id: steps
                    model: controller.numberOfSteps === -1 ? 0 : controller.numberOfSteps
                    Rectangle {
                        width: tickMarksRow.stepsWidth
                        height: index % 10 === 0 ? sliderBar.height : sliderBar.height * 0.5
                        color: tickMarksRow.spacing < 5 ? (index % 5 === 0 ? "black" : "transparent")
                                                                        : "black"

                        Label {
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                top: parent.bottom
                            }
                            color: textColor
                            horizontalAlignment: Text.AlignHCenter
                            visible: (labelMode === labelModeTicks) && index % labelSliderTickInterval === 0 && parent.color !== "transparent"
                            text: controller.stepTimes[index] ? timeStepIntervalLabelFormat ?
                                                                    Qt.formatDateTime(controller.stepTimes[index], timeStepIntervalLabelFormat)
                                                                  : Qt.formatDateTime(controller.stepTimes[index])
                                                              : ""
                        }
                    }
                }
            }

            Label {
                id: combinedLabel
                visible: (labelMode === labelModeThumbs) && (slider.second.visualPosition - slider.first.visualPosition) < 0.4
                anchors {
                    left: sliderBar.left
                    right: sliderBar.right
                }
                color: textColor
                y: (sliderBar.height * 0.5) + 0.5 * startThumb.height
                leftPadding: Math.min(currentExtentFill.x, sliderBar.width * 0.5)

                text: controller.startStep === controller.endStep ? currentStartLabel.text
                                                                  : currentStartLabel.text + " - " + currentEndLabel.text

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips
                }
            }
        }

        first.handle: Rectangle {
            id: startThumb
            visible: !startTimePinned
            enabled: !startTimePinned
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.first.visualPosition * parent.width) - (width * 0.5)

            width: 16
            height: width
            radius: width
            color: thumbFillColor
            border.color: thumbBorderColor

            Label {
                id: currentStartLabel
                visible: (labelMode === labelModeThumbs) && !combinedLabel.visible && controller.startStep !== controller.numberOfSteps - 1 && controller.startStep !== 0
                anchors {
                    top: startThumb.bottom
                    horizontalCenter: startThumb.horizontalCenter
                }

                leftPadding: (slider.width * slider.first.visualPosition) < 48 ? 48 : 0

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips
                }

                color: textColor
                text: currentExtentLabelFormat ? Qt.formatDateTime(controller.currentExtentStart, currentExtentLabelFormat) :
                                                 Qt.formatDateTime(controller.currentExtentStart)
                elide: Text.ElideLeft
            }
        }

        second.handle: Rectangle {
            id: endThumb
            visible: !endTimePinned
            enabled: !endTimePinned
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.second.visualPosition * parent.width) - (width * 0.5)
            width: 16
            height: width
            radius: width
            color: thumbFillColor
            border.color: thumbBorderColor

            Label {
                id: currentEndLabel
                visible: (labelMode === labelModeThumbs) && !combinedLabel.visible && (controller.endStep < (controller.numberOfSteps - 1)) && controller.endStep !== 0
                anchors {
                    top: endThumb.bottom
                    horizontalCenter: endThumb.horizontalCenter
                }

                rightPadding: (slider.width * slider.second.visualPosition) > slider.width - 48 ? 48 : 0

                font {
                    family: fontFamily
                    pixelSize: root.pixelSizeInDips
                }

                color: textColor
                text:  currentExtentLabelFormat ? Qt.formatDateTime(controller.currentExtentEnd, currentExtentLabelFormat)
                                                : Qt.formatDateTime(controller.currentExtentEnd)
                elide: Text.ElideLeft
            }
        }

        first.onValueChanged: {
            if (controller.startStep === slider.first.value)
                return;

            if (startTimePinned) {
                setValues(controller.startStep, controller.endStep);
                return;
            }

            controller.setStartInterval(slider.first.value);
        }

        second.onValueChanged: {
            if (controller.endStep === slider.second.value)
                return;

            if (endTimePinned) {
                setValues(controller.startStep, controller.endStep);
                return;
            }

            controller.setEndInterval(slider.second.value);
        }

        first.onPressedChanged: {
            if (!startTimePinned)
                return;

            first.pressed = false;
        }

        second.onPressedChanged: {
            if (!endTimePinned)
                return;

            second.pressed = false;
        }

        Rectangle {
            id: pinnedStart
            visible: startTimePinned
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.first.visualPosition * parent.width) - (width * 0.5)
            height: 16
            width: 4
            color: thumbFillColor
            border.color: thumbBorderColor
            radius: 1
        }

        Rectangle {
            id: pinnedEnd
            visible: endTimePinned
            anchors.verticalCenter: sliderBar.verticalCenter
            x: (slider.second.visualPosition * parent.width) - (width * 0.5)
            height: 16
            width: 4
            color: thumbFillColor
            border.color: thumbBorderColor
            radius: 1
        }
    }
}
