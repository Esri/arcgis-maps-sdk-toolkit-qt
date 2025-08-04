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

T.GroupBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            label ? label.implicitWidth + leftPadding + rightPadding : 0,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: 8
    topPadding: padding + (label && label.implicitWidth > 0 ? label.implicitHeight + spacing : 0)
    spacing: 5

    label: Text {
        x: control.leftPadding
        width: control.availableWidth

        text: control.title
        font: control.font
        color: control.enabled ? Calcite.text2 : Calcite.text3
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        width: parent.width
        height: parent.height

        color: "transparent"
        border.color: Calcite.border1
    }
}
