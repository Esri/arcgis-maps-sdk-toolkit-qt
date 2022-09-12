/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
import QtQuick 2.15
import QtQuick.Templates 2.15 as T
import Qt5Compat.GraphicalEffects

T.SpinBox {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentItem.implicitWidth +
                            up.implicitIndicatorWidth +
                            down.implicitIndicatorWidth)
    implicitHeight: Math.max(implicitContentHeight + topPadding + bottomPadding,
                             implicitBackgroundHeight,
                             up.implicitIndicatorHeight,
                             down.implicitIndicatorHeight)
    topPadding: 5
    bottomPadding: 5

    spacing: 6

    leftPadding: (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextInput {
        id: textInput
        text: control.displayText

        font: control.font
        color : Calcite.text1
        selectionColor: Calcite.brandHover
        selectedTextColor: Calcite.foreground1
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
    }

    up.indicator: Item {
        x: control.mirrored ? 0 : parent.width - width
        implicitWidth: 32
        implicitHeight: 32
        height: parent.height
        width: height
        Rectangle {
            width: parent.width
            height: parent.width
            color: enabled ? (up.hovered ? Calcite.foreground2 : Calcite.foreground1) : Calcite.background
            border {
                color: Calcite.border1
            }

            Image {
                id: upIndicatorImage
                fillMode: Image.PreserveAspectFit
                anchors {
                    fill: parent
                }
                source: "images/chevron-right.svg"
                sourceSize.width: parent.width
                sourceSize.height: parent.width
            }

            ColorOverlay {
                anchors.fill : upIndicatorImage
                source: upIndicatorImage
                color: up.hovered ? Calcite.text1 : Calcite.text3
            }
        }
    }


    down.indicator: Item {
        x: control.mirrored ? parent.width - width : 0
        implicitWidth: 32
        implicitHeight: 32
        height: parent.height
        width: height

        Rectangle {
            width: parent.width
            height: parent.height
            color: enabled ? (down.hovered ? Calcite.foreground2 : Calcite.foreground1) : Calcite.background
            border {
                color: Calcite.border1
            }

            Image {
                id: downIndicatorImage
                fillMode: Image.PreserveAspectFit
                anchors {
                    fill: parent
                }
                source: "images/chevron-left.svg"
                sourceSize.width: parent.width
                sourceSize.height: parent.height
            }

            ColorOverlay {
                anchors.fill : downIndicatorImage
                source: downIndicatorImage
                color: down.hovered ? Calcite.text1 : Calcite.text3
            }
        }
    }

    background: Item {
        implicitWidth: 192
        implicitHeight: 32

        Rectangle {
            id: backgroundRect
            x: parent.width / 2 - width / 2
            width: control.availableWidth
            height: parent.height
            color: enabled ? Calcite.foreground1 : Calcite.background
            border {
                // error box not shown. No error shown in the calcite interactive design.
                color: control.activeFocus ? Calcite.brand : Calcite.borderInput
            }
        }
    }
}
