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

import QtQuick 2.12
import QtQuick.Controls 2.2

Item {
    id: root

    // The interval between triggers, in milliseconds. Default is 250 milliseconds.
    property alias interval: timer.interval

    // /...
    signal triggered(double latitude, double longitude, double altitude, double heading);

    Timer {
        id: timer
        repeat: true
        interval: 25
        running: latitudeSlider.pressed || longitudeSlider.pressed || altitudeSlider.pressed || headingSlider.pressed;

        // update the value with the offet 10^value.
        onTriggered: {
            latitudeSlider.update();
            longitudeSlider.update();
            altitudeSlider.update();
            headingSlider.update();
            root.triggered(latitudeSlider.value, longitudeSlider.value, altitudeSlider.value, headingSlider.value);
        }
    }

    Column {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }

        spacing: 5

        CalibrationSlider {
            id: latitudeSlider
            width: parent.width
            text: "Latitude"
            textWidth: longitudeSlider.textWidth
            factor: 0.0001
        }
        CalibrationSlider {
            id: longitudeSlider
            width: parent.width
            text: "Longitude"
            factor: 0.0001
        }
        CalibrationSlider {
            id: altitudeSlider
            width: parent.width
            text: "Altitude"
            textWidth: longitudeSlider.textWidth
            factor: 0.01
        }
        CalibrationSlider {
            id: headingSlider
            width: parent.width
            text: "Heading"
            textWidth: longitudeSlider.textWidth
            factor: 0.01
        }
    }
}
