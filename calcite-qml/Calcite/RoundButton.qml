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
import QtQuick 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Templates 2.15 as T

T.RoundButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    topPadding: 6
    bottomPadding: 6
    leftPadding: 16
    rightPadding: 16
    spacing: 6

    icon.width: 24
    icon.height: 24

    property color color: control.flat ? Calcite.textLink : Calcite.textInverse
    icon.color: color

    contentItem: IconLabel {
        id: textItem
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.color
        opacity: control.flat && !control.enabled ? 0.3 : 1.0
    }

    background: Rectangle {
        radius: control.radius
        implicitWidth: 80
        implicitHeight: 24
        border {
            color: {
                if (control.flat) {
                   return "transparent";
                } else {
                    return control.hovered ? Calcite.brandHover : Calcite.brand;
                }
            }
            width: 1
        }
        opacity: control.enabled ? 1.0 : 0.3
        color: {
            if (control.flat) {
                return control.pressed || control.checked ? Calcite.foreground3 :
                                                            control.hovered ? Calcite.foreground2
                                                                            : "transparent"
            }  else {
                return control.pressed || control.checked ? Calcite.brandPress
                                                          : control.hovered ? Calcite.brandHover
                                                                            : Calcite.brand
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: 50
            }
            enabled: !control.flat
        }
    }
}
