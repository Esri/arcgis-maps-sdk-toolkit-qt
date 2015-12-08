/*******************************************************************************
 * Copyright 2012-2015 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.0
import Esri.ArcGISRuntime 100.00
import Esri.ArcGISRuntime.Toolkit.Controls 1.1

Rectangle {
    width: 500
    height: 400

    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    MapView {
        id: mapview
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeDisabled
        magnifierEnabled: true
        zoomByPinchingEnabled: true
        rotationByPinchingEnabled: true

        Map {
            BasemapNationalGeographic {}
        }

        NorthArrow {
            id: northArrow

            anchors {
                top: parent.top
                right: parent.right
                topMargin: 20
                rightMargin: 10
            }
        }

        RotationToolbar {
            anchors {
                left: parent.left
                top: parent.top
                margins: 10 * displayScaleFactor
            }
        }

        Text {
            id: textBearing
            text: (Number(mapview.mapRotation)).toFixed(1) === "0.0" ? "00.0°" : (Number(mainMap.mapRotation)).toFixed(1) + "°"
            anchors {
                top: northArrow.bottom
                bottomMargin: 5 * displayScaleFactor
                right: parent.right
                rightMargin: 10 * displayScaleFactor
            }
            font.pixelSize: 16 * displayScaleFactor
            horizontalAlignment: Text.AlignHCenter
            style: Text.Outline
            styleColor: "white"
        }

    }
}

