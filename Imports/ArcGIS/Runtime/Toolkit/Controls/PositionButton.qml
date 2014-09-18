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

Button {
    id: positionButton
    property real size: 40
    property color color: "#4C4C4C"
    property color disabledColor: "#E5E6E7"
    property color hoveredColor: "#E1F0FB"
    property color pressedColor: "#90CDF2"
    property color backgroundColor: "#F7F8F8"
    property color focusBorderColor: "#AADBFA"
    property color borderColor: "#CBCBCB"

    readonly property int buttonPosition: 0x08
    property alias fader: fader

    property bool isActive: map && map.positionDisplay.positionSource && map.positionDisplay.positionSource.active
    property int maxModes: map.positionDisplay.isCompassAvailable ? 4 : 3;

    visible: buttonPosition && map && map.positionDisplay.positionSource
    width: internal._size
    height: width
    iconSource: isActive ? modeImage(map.positionDisplay.mode) : "images/position-off.png"
    tooltip: qsTr("Location")
    style: buttonStyle

    MouseArea {
        anchors.fill: parent

        onPressAndHold: {
            fader.start();

            if (map.positionDisplay.positionSource.active) {
                map.positionDisplay.positionSource.active = false;
            }
        }

        onClicked: {
            fader.start();

            if (map.positionDisplay.positionSource.active) {
                map.positionDisplay.mode = (map.positionDisplay.mode + 1) % positionButton.maxModes;
            } else {
                map.positionDisplay.positionSource.active = true;
                map.positionDisplay.mode = 1;
            }
        }
    }

    function modeImage(mode) {
        switch (mode) {
        case 0 :
            return "images/position-on.png";

        case 1 :
            return "images/position-autopan.png";

        case 2 :
            return "images/position-navigation.png";

        case 3 :
            return "images/position-compass.png"
        }
    }

    Fader {
        id: fader
    }

    QtObject {
        id: internal
        property real _size: size * System.displayScaleFactor
    }

    Component {
        id: buttonStyle

        ButtonStyle {

            label: Item {
                Image {
                    anchors.centerIn: parent
                    width: internal._size * 0.8
                    height: width
                    source: control.iconSource
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    anchors.centerIn: parent
                    color: control.enabled ? positionButton.color : disabledColor
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
                implicitWidth: 40 * System.displayScaleFactor
                implicitHeight: 40 * System.displayScaleFactor
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
