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

import QtQuick 2.12
import QtQuick.Controls 2.2

/*!
    \qmltype ClippingView
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
    property alias clippingDistance: slider.value

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
                text: "Clipping distance: " + Math.floor(slider.value) + " m"
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
            }
            Slider {
                id: slider
                width: parent.width
                from: 0.0
                to: 5000.0
                value: 0.0
            }
        }
    }
}
