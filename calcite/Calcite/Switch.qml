/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
import QtQuick.Templates as T

T.Switch {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    spacing: 16

    indicator:  Item {
        id: indicator
        implicitWidth: 36
        implicitHeight: 32
        opacity: control.enabled ? 1 : 0.3

        property Item control: control
        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2

        Rectangle {
            width: parent.width
            height: 20
            radius: 30
            y: parent.height / 2 - height / 2
            color: control.checked ? Calcite.brand : Calcite.background
            border {
                width: 1
                color: control.checked ? Calcite.brandPress : Calcite.border1
            }
        }

        Rectangle {
            id: handle
            x: Math.max(0, Math.min(parent.width - width, control.visualPosition * parent.width - (width / 2)))
            y: (parent.height - height) / 2
            width: 20
            height: 20
            radius: 30
            color: Calcite.foreground1
            border {
                width: 2
                color: control.checked ? Calcite.brandPress : control.activeFocus || control.hovered && control.enabled ? Calcite.brandHover : Calcite.borderInput
            }

            Behavior on x {
                enabled: !control.pressed
                SmoothedAnimation {
                    duration: 250
                    easing.type: Easing.InOutCubic
                }
            }
        }
    }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: Calcite.text1
        elide: Text.ElideRight
        visible: control.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
