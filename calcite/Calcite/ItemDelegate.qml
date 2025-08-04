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
import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.ItemDelegate {
    id: control

    Component.onCompleted: {
        // disable hover behavior on mobile
        const platform = Qt.platform.os;
        if (platform === "android" || platform === "ios") {
            hoverEnabled = false;
        }
    }

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 5
    spacing: 5

    icon.width: 16
    icon.height: 16

    property color color : control.enabled ? control.hovered || control.down ? Calcite.offWhite : Calcite.text1 : Calcite.text3
    icon.color: color

    indicator: Image {
        id: indicator
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        visible: control.highlighted
        source: "images/check.svg"
    }
    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        leftPadding: {
            if (!control.mirrored && control.indicator) {
                const width = control.indicator.width;
                if (width > 0) {
                    return control.spacing + width;
                }
            }
            return 0;
        }
        rightPadding: {
            if (control.mirrored && control.indicator) {
                const width = control.indicator.width;
                if (width > 0) {
                    return control.spacing + width;
                }
            }
            return 0;
        }
        alignment: control.display === IconLabel.IconOnly || control.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft

        icon: control.icon
        text: control.text
        font: control.font
        color: control.color
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.visualFocus || control.down ? Calcite.brandPress : control.hovered ? Calcite.brand : "transparent"
    }
}
