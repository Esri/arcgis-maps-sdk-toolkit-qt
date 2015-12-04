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

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import Esri.ArcGISRuntime 100.00

Button {
    id: styleButton
    property real size: 40
    property color color: "#4C4C4C"
    property color disabledColor: "#E5E6E7"
    property color hoveredColor: "#E1F0FB"
    property color pressedColor: "#90CDF2"
    property color backgroundColor: "#F7F8F8"
    property color focusBorderColor: "#AADBFA"
    property color borderColor: "#CBCBCB"
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property alias fader: fader

    width: internal._size
    height: width
    tooltip: qsTr("Style Button")
    style: buttonStyle

    onClicked: {
        fader.start();
    }

    Fader {
        id: fader
    }

    QtObject {
        id: internal
        property real _size: size * displayScaleFactor
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
                    color: control.enabled ? styleButton.color : disabledColor
                    text: control.text
                    font {
                        pixelSize: internal._size * 0.75
                        bold: true
                    }
                }
            }

            background: Rectangle {
                color: {
                    if ( Qt.platform.os == "android" || Qt.platform.os == "ios" )
                        color: backgroundColor
                    else
                        color: control.hovered ?  hoveredColor : (control.pressed ? pressedColor : backgroundColor)
                }
                border {
                    color: control.activeFocus ? focusBorderColor : borderColor
                    width: control.activeFocus ? 2 : 1
                }
                radius: 4
                implicitWidth: 40 * displayScaleFactor
                implicitHeight: 40 * displayScaleFactor
            }

            Connections {
                target: control
                onPressedChanged: {
                    if (control.pressed)
                        fader.start();
                }

                onHoveredChanged: {
                    if (control.hovered)
                        fader.stop();
                    else
                        fader.start();
                }
            }
        }
    }
}
