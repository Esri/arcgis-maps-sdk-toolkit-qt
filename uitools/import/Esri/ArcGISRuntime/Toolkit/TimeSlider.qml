/*******************************************************************************
 *  Copyright 2012-2020 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

/*!
    \qmltype TimeSlider
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \ingroup ArcGISQtToolkitUiQmlViews
    \since Esri.ArcGISRuntime 100.10
    \brief The slider provides a user interface for manually setting or animating
    changes to the current time extent of the \c GeoView.
    
    A time slider can be bound to a geoView
    (\c MapView or \c SceneView) to allow filtering on temporal data.
    The time extents of all layers in the map or scene will be used to set up the
    slider with the full temporal range and the current time extent.
*/
Control {
    id: timeSlider

    /*!
      \brief Options for Slider label styling.

      Valid options are:

      \value LabelMode.Node No labels are applied.
      \value LabelMode.Thumbs Labels are applied to the slider thumbs.
      \value LabelMode.Ticks Labels are applied to the slider tick marks.

      The default is \c LabelMode.Thumbs.
      */
    enum LabelMode { None, Thumbs, Ticks }

    /*!
      \qmlproperty enum labelMode
      \brief Current option for applying labels to the Slider.

      Valid options are:

      \value LabelMode.Node No labels are applied.
      \value LabelMode.Thumbs Labels are applied to the slider thumbs.
      \value LabelMode.Ticks Labels are applied to the slider tick marks.

      The default is \c LabelMode.Thumbs.
      */
    property int labelMode: TimeSlider.LabelMode.Thumbs

    /*!
      \qmlproperty int labelSliderTickInterval
      \brief The interval at which slider ticks should be labeled.
      
      The default is \c 20.
     */
    property int labelSliderTickInterval: 20

    /*!
      \qmlproperty GeoView geoView
      \brief The GeoView for this tool. Should be a SceneView or a MapView.
     */
    property var geoView;

    /*!
      \qmlproperty TimeSliderController controller
      \brief The controller handles calculating steps and setting extents on the
       GeoView.

       A default TImeSliderController is provided.
    */
    property var controller: TimeSliderController { }

    /*!
      \qmlproperty var fullExtentLabelFormat
      \brief The format for displaying
      \l {https://doc.qt.io/qt-5/qml-qtqml-date.html}{Date} values
      for the full time extent. - for example "yy/MM/dd".

      The default is \l {https://doc.qt.io/qt-5/qt.html#DateFormat-enum}
      {\c Qt.DefaultLocaleShortDate}.
      \sa Qt.formatDateTime
    */
    property string fullExtentLabelFormat:
        Qt.locale().dateTimeFormat(Locale.ShortFormat);

    /*!
      \qmlproperty var timeStepIntervalLabelFormat
      \brief The date format for displaying time step intervals, 
      for example: "yy/MM/dd".
      The default is \l {https://doc.qt.io/qt-5/qt.html#DateFormat-enum}
      {\c Qt.DefaultLocaleShortDate}.
      \sa Qt.formatDateTime
      */
    property var timeStepIntervalLabelFormat:
        Qt.locale().dateTimeFormat(Locale.ShortFormat);

    /*!
      \qmlproperty bool startTimePinned
      \brief Whether the start time of the time window can
      be manipulated.
      
      The default is \c false.
    */
    property bool startTimePinned: false

    /*!
      \qmlproperty bool endTimePinned
      \brief Whether the end time of the time window can
      be manipulated.
      
      The default is \c false.
    */
    property bool endTimePinned: false

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
      The default is \c false.
    */
    property bool playbackReverse: false

    /*!
    \qmlproperty int playbackInterval
    \brief The amount of time (in milliseconds) during playback
    that will elapse before the slider advances to the next time step.
    
    The default is \c 500.
    */
    property alias playbackInterval : playAnimation.interval

    /*!
     \qmlproperty icon TimeSlider::stepBackIcon
     \brief The icon for the step-back button.
     */
    property alias stepBackIcon: stepBackButton.icon

    /*!
     \qmlproperty icon icon TimeSlider::playIcon
     \brief The icon for the play button.
     */
    property alias playIcon: playButton.icon

    /*!
     \qmlproperty icon TimeSlider::stepForwardIcon
     \brief The icon for the step-forward button.
     */
    property alias stepForwardIcon: stepForwardButton.icon

    background: Rectangle { }

    contentItem: GridLayout {
        columns: 5

        Label {
            id: startLabel
            horizontalAlignment: Qt.AlignLeft
            palette: timeSlider.palette
            Connections {
                target: controller
                function onExtentsChanged() {
                    startLabel.text = Qt.formatDateTime(
                                controller.timeForStep(0),
                                fullExtentLabelFormat);
                }
            }
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            Layout.fillWidth: true
            Layout.margins: 5
        }

        Button {
            id: stepBackButton
            icon.source: "images/step_back.png"
            enabled: !startTimePinned || !endTimePinned
            palette: timeSlider.palette
            onClicked: timeSlider.incrementFrame(-1);
            Layout.alignment: Qt.AlignLeft
            Layout.margins: 5
        }

        Button {
            id: playButton
            icon.source: checked ? "images/pause.png"
                                 : "images/play.png"
            enabled: !startTimePinned || !endTimePinned
            checkable: true
            palette: timeSlider.palette
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: 5
            Timer {
                id: playAnimation
                running: parent.checked
                repeat: true
                interval: 500
                onTriggered: play();
            }
        }

        Button {
            id: stepForwardButton
            icon.source: "images/step.png"
            enabled: !startTimePinned || !endTimePinned
            palette: timeSlider.palette
            onClicked: timeSlider.incrementFrame(1);
            Layout.alignment: Qt.AlignRight
            Layout.margins: 5
        }

        Label {
            text: Qt.formatDateTime(
                      controller.timeForStep(controller.numberOfSteps),
                      fullExtentLabelFormat);
            horizontalAlignment: Qt.AlignRight
            palette: timeSlider.palette
            Layout.alignment: Qt.AlignRight  | Qt.AlignBottom
            Layout.fillWidth: true
            Layout.margins: 5
        }

        RangeSlider {
            id: slider
            stepSize: 1.0
            palette: timeSlider.palette
            snapMode: RangeSlider.SnapAlways
            from: 0
            to: controller.numberOfSteps
            first {
                handle: Rectangle {
                    x: slider.leftPadding + slider.first.visualPosition * (slider.availableWidth) - width /2
                    y: slider.topPadding - height/2
                    implicitWidth: enabled ? 26 : 15
                    implicitHeight: 26
                    radius: enabled ? 13 : 1
                    color: slider.first.pressed && enabled ? slider.palette.midlight
                                                           : slider.palette.base
                    border.color: slider.palette.mid
                    enabled: !startTimePinned
                }
                onValueChanged: {
                    if (slider.first.pressed) {
                        if (slider.first.handle.enabled) {
                            controller.setSteps(first.value, controller.endStep);
                        } else { // Reset
                            slider.first.value = controller.startStep;
                        }
                    }
                }
            }
            second {
                handle: Rectangle {
                    x: slider.leftPadding + slider.second.visualPosition * (slider.availableWidth) - width/2
                    y: slider.topPadding - height/2
                    implicitWidth: enabled ? 26 : 15
                    implicitHeight: 26
                    radius: enabled ? 13 : 1
                    color: slider.second.pressed && enabled ? slider.palette.midlight
                                                            : slider.palette.base
                    border.color: slider.palette.mid
                    enabled: !endTimePinned
                }
                onValueChanged: {
                    if (slider.second.pressed) {
                        if (slider.second.handle.enabled) {
                            controller.setSteps(controller.startStep, second.value);
                        } else { // Reset
                            slider.second.value = controller.endStep;
                        }
                    }
                }
            }
            background:  Item {
                id: sliderBackground
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                height: childrenRect.height
                y: slider.topPadding
                Rectangle { // The bar itself.
                    id: sliderBar
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }
                    implicitHeight: 4
                    height: implicitHeight
                    radius: 2
                    color: slider.palette.midlight
                    Rectangle { // The "filled in" portion.
                        x: slider.first.visualPosition * parent.width
                        width: slider.second.visualPosition * parent.width - x
                        height: parent.height
                        color: slider.palette.shadow
                        radius: 2
                    }
                }
                Repeater {
                    id: repeater
                    property int marks: slider.to - slider.from
                    property var firstHandleLabel: null;
                    property var secondHandleLabel: null;
                    model: marks > 0 ? marks + 1 : 0
                    Item { // Holds a tickmark and the label for that mark.
                        id: tickHold
                        anchors.top: sliderBar.bottom
                        x: (slider.availableWidth  - width)
                           * (index / repeater.marks) + slider.leftPadding
                        width: tickMark.width
                        height: childrenRect.height
                        Rectangle  { // Mark itself.
                            id: tickMark
                            anchors.top: parent.top
                            width: 2
                            height: index % labelSliderTickInterval === 0 ? 20 
                                                                          : 10
                            color: slider.palette.midlight
                        }
                        Label { // Tickmark label
                            id: tickLabel
                            anchors {
                                top: tickMark.bottom
                                topMargin: index % labelSliderTickInterval === 0 ? 0 : 10
                            }

                            property string defaultLabelText:
                                Qt.formatDateTime(
                                    controller.timeForStep(index),
                                    timeStepIntervalLabelFormat);

                            property int tickIndex: index;

                            // For the handles case where two handles are close
                            // together, we combine the info of two handles into
                            // one variable label.
                            property string combinedLabelText: repeater.secondHandleLabel ? `${
                                Qt.formatDateTime(
                                    controller.timeForStep(tickIndex),
                                    timeStepIntervalLabelFormat)} - ${
                                Qt.formatDateTime(
                                    controller.timeForStep(repeater.secondHandleLabel.tickIndex),
                                    timeStepIntervalLabelFormat)}` : "";

                            property real defaultLabelWidth: 
                                fontMetric.boundingRect(defaultLabelText).width;

                            property real defaultLabelX: {
                                const minX = -tickHold.x;
                                const maxX = -tickHold.x + sliderBackground.width - defaultLabelWidth;
                                return Math.min(maxX, Math.max(-defaultLabelWidth/2, minX));
                            }

                            x: {
                                const minX = -tickHold.x;
                                const maxX = -tickHold.x + sliderBackground.width - width;
                                return Math.min(maxX, Math.max(-defaultLabelWidth/2, minX));
                            }

                            text: defaultLabelText;

                            palette: slider.palette
                            font: slider.font

                            // By default the label mark is only visible on
                            // the "long" ticks.
                            visible: index > 0 &&
                                     index < repeater.marks &&
                                     index % labelSliderTickInterval === 0 &&
                                     labelMode === TimeSlider.LabelMode.Ticks 

                            states: [
                                // Change the state of this single label to 
                                // visible if not an endpoint.
                                State {
                                    name: "singleVisible"
                                    PropertyChanges {
                                        target: tickLabel
                                        visible: index > 0 && 
                                                 index < repeater.marks
                                    }
                                },
                                // Change the state of this label to visible and
                                // have it represented a "combined" label of 
                                // this and the second handle position(s).
                                State {
                                    name: "combinedVisible"
                                    PropertyChanges {
                                        target: tickLabel
                                        text: tickLabel.combinedLabelText
                                        visible: index > 0 && 
                                                 index < repeater.marks
                                    }
                                }
                            ]

                            // If the sliders change value, test to see if this
                            // label becomes the handle that is now pointed-to.
                            Connections {
                                target: slider.first
                                function onValueChanged() {
                                    if (labelMode !== TimeSlider.LabelMode.Thumbs) {
                                        return;
                                    }

                                    if (index !== slider.first.value) {
                                        return;
                                    }

                                    if (index > 0 && index < repeater.marks) {
                                        repeater.firstHandleLabel = tickLabel;
                                    } else {
                                        repeater.firstHandleLabel = null;
                                    }
                                }
                            }
                            Connections {
                                target: slider.second
                                function onValueChanged() {
                                    if (labelMode !== TimeSlider.LabelMode.Thumbs) {
                                        return;
                                    }

                                    if (index !== slider.second.value) {
                                        return;
                                    }

                                    if (index > 0 && index < repeater.marks) {
                                        repeater.secondHandleLabel = tickLabel;
                                    } else {
                                        repeater.secondHandleLabel = null;
                                    }
                                }
                            }

                            // If the current handle labels change, reset this
                            // state of all other labels back to default.
                            Connections {
                                target: repeater
                                function onFirstHandleLabelChanged() {
                                    if (tickLabel !== repeater.firstHandleLabel &&
                                        tickLabel !== repeater.secondHandleLabel) {
                                        tickLabel.state = "";
                                    }
                                }
                                function onSecondHandleLabelChanged() {
                                    if (tickLabel !== repeater.firstHandleLabel &&
                                        tickLabel !== repeater.secondHandleLabel) {
                                        tickLabel.state = "";
                                    }
                                }
                            }
                        }
                    }
                    onFirstHandleLabelChanged: handlesChanged()
                    onSecondHandleLabelChanged: handlesChanged()
                    function handlesChanged() {
                        if (firstHandleLabel && firstHandleLabel == secondHandleLabel) {
                            firstHandleLabel.state = "singleVisible";
                        } else if (firstHandleLabel && secondHandleLabel 
                                   && horizontalOverlaps(firstHandleLabel, secondHandleLabel)) {
                            firstHandleLabel.state = "combinedVisible";
                            secondHandleLabel.state = "";
                        } else {
                            if (firstHandleLabel) {
                                firstHandleLabel.state = "singleVisible";
                            }

                            if (secondHandleLabel) {
                                secondHandleLabel.state = "singleVisible";
                            }
                        }
                    }
                    function horizontalOverlaps(item1, item2) {
                        const r1 = Qt.rect(
                            item1.parent.x + item1.defaultLabelX, item1.y,
                            item1.defaultLabelWidth, item1.height
                        );
                        const r2 = Qt.rect(
                            item2.parent.x + item2.defaultLabelX, item2.y,
                            item2.defaultLabelWidth, item2.height
                        );
                        if (r1.right < r2.left) {
                            return false;
                        }
                        else if (r2.right < r1.left) {
                            return false;
                        }
                        return true;
                    }
                }
                FontMetrics {
                    id: fontMetric
                        font: slider.font
                }
            }
            Connections {
                target: controller
                function onStepsChanged() { 
                    slider.setValues(controller.startStep,
                                     controller.endStep);
                }
            }
            Layout.columnSpan: 5
            Layout.fillWidth: true
            Layout.margins: 5
            Layout.minimumHeight: sliderBackground.height
        }
    }

    Binding {
        target: controller
        property: "geoView"
        value: timeSlider.geoView
    }

    /*!
     \brief Increments both handles by \a count.
     
     Count may be negative to decrement the handles.
     */
    function incrementFrame(count) {
        const s = startTimePinned ? controller.startStep
                                  : controller.startStep + count;

        const e = endTimePinned ? controller.endStep
                                : controller.endStep + count;

        if (e <= controller.numberOfSteps && s >= 0 && s <= e) {
            controller.setSteps(s, e);
            return true;
        } else {
            return false;
        }
    }

    /*!
     \internal 
     \brief Increments/decrements both handles by 1 or -1 depending on the
            value of \l playbackReverse.

            If \l playbackLoop is true (and there are no pinned handles), the 
            playback loops around to the start of the animation once again.
     */
    function play() {
        const success = incrementFrame(playbackReverse ? -1 : 1);
        const loops = playbackLoop && !(startTimePinned || endTimePinned);
        if (loops && !success) {
            let range = controller.endStep - controller.startStep;
            if (playbackReverse) {
                controller.setSteps(controller.numberOfSteps - range,
                                    controller.numberOfSteps);
            } else {
                controller.setSteps(0, range);
            }
        }
    }
}
