/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

import QtQuick
import QtQuick.Controls

Item {
    id: root
    implicitWidth: label.implicitWidth + slider.implicitWidth
    implicitHeight: Math.max(label.implicitHeight, slider.implicitHeight)

    // The text to display on the left of the slider.
    property alias text: label.text

    // The width of the text.
    property alias textWidth: label.width

    // The value of the calibration slider.
    property double sliderValue: 0.0

    // The multiply factor to applied to offset.
    property double factor: 1.0

    // This property holds whether the slider is pressed by either touch, mouse, or keys.
    property alias pressed: slider.pressed

    // Resets the slider value to 0.0
    function reset() {
        sliderValue = 0.0;
    }

    Label {
        id: label
        anchors {
            top: root.top
            bottom: root.bottom
            left: root.left
        }
        height: slider.height
        verticalAlignment: Text.AlignVCenter
    }

    Slider {
        id: slider
        anchors {
            top: root.top
            bottom: root.bottom
            right: root.right
            left: label.right
        }
        from: -3.0
        to: 3.0
        value: 0.0

        onPressedChanged: value = 0.0;
    }

    // update the value with the offset 10^value.
    function update() {
        sliderValue += Math.sign(slider.value) * Math.pow(10, Math.abs(slider.value)) * factor;
    }
}
