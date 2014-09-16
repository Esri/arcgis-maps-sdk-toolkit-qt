/*******************************************************************************
 * Copyright 2012-2014 Esri
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

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import ArcGIS.Runtime.AppKit 1.0
import ArcGIS.Runtime 10.3

GridLayout {
    id: zoomButtons

    property Map map: null
    property real size: 40
    property real zoomRatio: 2

    property color color: "#4C4C4C"
    property color disabledColor: "#E5E6E7"
    property color hoveredColor: "#E1F0FB"
    property color pressedColor: "#90CDF2"
    property color backgroundColor: "#F7F8F8"
    property color focusBorderColor: "#AADBFA"
    property color borderColor: "#CBCBCB"
    property string orientation: "portrait"

    property Envelope homeExtent

    property alias fader: fader

    readonly property int buttonZoomIn: 0x01
    readonly property int buttonZoomOut: 0x02
    readonly property int buttonHome: 0x04
    readonly property int buttonPosition: 0x08

    property int buttons: buttonZoomIn + buttonZoomOut + buttonHome + buttonPosition
    columns: orientation === "portrait" ? 1 : 4
    rows: orientation === "portrait" ? 4 : 1
    rowSpacing: orientation === "landscape" ? 0 : 1
    columnSpacing: orientation === "portrait" ? 0 : 1

    //--------------------------------------------------------------------------

    Component.onCompleted: {
        if (!map && parent && parent.objectType && parent.objectType === "Map") {
            map = parent;
        }
    }

    //--------------------------------------------------------------------------

    QtObject {
        id: internal

        property real _size: size * System.displayScaleFactor
    }

    //--------------------------------------------------------------------------

    Fader {
        id: fader
    }

    //--------------------------------------------------------------------------
    // Zoom-In Button
    ZoomInButton {
        id: zoomIn
        style: buttonStyle

    }

    //--------------------------------------------------------------------------
    // Zoom-Out Button
    ZoomOutButton {
        id: zoomOut
        style: buttonStyle
    }

    //--------------------------------------------------------------------------
    // Home Button
    HomeButton {
        id: home
        style: buttonStyle
    }

    //--------------------------------------------------------------------------
    // MyCurrentLocationButton
    MyCurrentLocationButton {
        id: currentLocation
        style: buttonStyle
    }

    //--------------------------------------------------------------------------
    // Button Style Component
    Component {
        id: buttonStyle

        ButtonStyle {

            label: Item {
                Image {
                    width: internal._size * 0.8
                    height: width
                    source: control.iconSource
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    anchors.centerIn: parent
                    color: control.enabled ? zoomButtons.color : disabledColor
                    text: control.text
                    font {
                        pixelSize: internal._size * 0.75
                        bold: true
                    }
                }
            }

            background: Rectangle {
                color: control.hovered ?  hoveredColor : (control.pressed ? pressedColor : backgroundColor)
                border {
                    color: control.activeFocus ? focusBorderColor : borderColor
                    width: control.activeFocus ? 2 : 1
                }
                radius: 4
                implicitWidth: 40
                implicitHeight: 40
            }

            Connections {
                target: control

                onPressedChanged: {
                    if (control.pressed) {
                        fader.start();
                    }
                }

                onHoveredChanged: {
                    if (control.hovered) {
                        fader.stop();
                    } else {
                        fader.start();
                    }
                }
            }
        }
    }
}
