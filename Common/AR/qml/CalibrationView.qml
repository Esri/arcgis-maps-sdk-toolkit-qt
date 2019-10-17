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

/*!
    \qmltype CalibrationView
    \ingroup ArcGISQtToolkitAR
    \ingroup ArcGISQtToolkitARCppApi
    \ingroup ArcGISQtToolkitARQmlApi
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.AR
    \since Esri.ArcGISRutime 100.6
    \brief A item displaying controls for adjusting a scene view's location, heading, and elevation.

    This item can be used to change manually the location and heading of the scene, to ensure
    accuracy between the device live video and the scene.

*/

Item {
    id: root
    height: rect.height

    /*!
      \brief The interval between triggers, in milliseconds.

      Default is 250 milliseconds.
     */
    property alias interval: timer.interval

    /*!
      \brief The multiplication factor to apply to the latitude value.

      Default is 0.0001.
     */
    property alias latitudeFactor: latitudeSlider.factor

    /*!
      \brief The multiplication factor to apply to the longitude value.

      Default is 0.0001.
     */
    property alias longitudeFactor: longitudeSlider.factor

    /*!
      \brief The multiplication factor to apply to the altitude value.

      Default is 0.01.
     */
    property alias altitudeFactor: altitudeSlider.factor

    /*!
      \brief The multiplication factor to apply to the heading value.

      Default is 0.01.
     */
    property alias headingFactor: headingSlider.factor

    /*!
      \qmlsignal triggered(double latitude, double longitude, double altitude, double heading)
      \brief Sends the updated values for location and heading.

      This signal is emitted every time the timer is triggered.
     */
    signal triggered(double latitude, double longitude, double altitude, double heading);

    /*!
      \qmlmethod void reset()
      \brief Resets the calibration to location (0, 0, 0) and heading 0.0.
     */
    function reset() {
        latitudeSlider.reset();
        longitudeSlider.reset();
        altitudeSlider.reset();
        headingSlider.reset();
        root.triggered(latitudeSlider.sliderValue, longitudeSlider.sliderValue,
                       altitudeSlider.sliderValue, headingSlider.sliderValue);
    }

    // private
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

            if (latitudeSlider.sliderValue !== 0.0 || longitudeSlider.sliderValue !== 0.0 ||
                    altitudeSlider.sliderValue !== 0.0 || headingSlider.sliderValue !== 0.0) {
                root.triggered(latitudeSlider.sliderValue, longitudeSlider.sliderValue,
                               altitudeSlider.sliderValue, headingSlider.sliderValue);
            }
        }
    }

    Rectangle {
        id: rect
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: column.height + 20
        color: "#88ffffff" // transparent white
        radius: 5

        Column {
            id: column
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: 10
            }
            spacing: 5

            Text {
                width: parent.width
                text: "Calibration"
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
            }
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
}
