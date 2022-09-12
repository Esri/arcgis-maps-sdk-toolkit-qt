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
import QtQuick.Templates 2.15 as T
import Qt5Compat.GraphicalEffects

T.MenuItem {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 5

    contentItem: Text {
        leftPadding: control.checkable && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.checkable && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.enabled ? control.hovered ? Calcite.offWhite : Calcite.text1 : Calcite.text3
        elide: Text.ElideRight
        visible: control.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Image {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        visible: control.checked
        source: control.checkable ? "images/check.svg" : ""
        ColorOverlay {
            anchors.fill: parent
            source: parent
            color: control.contentItem.color
        }
    }

    background: Item {
        implicitWidth: 200
        implicitHeight: 40

        Rectangle {
            width: parent.width
            height: parent.height
            color: control.visualFocus || control.down ? Calcite.brandPress : control.hovered ? Calcite.brand : "transparent"
        }
    }
}
