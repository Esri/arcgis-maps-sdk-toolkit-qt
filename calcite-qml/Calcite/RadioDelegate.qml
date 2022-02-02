
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
import QtGraphicalEffects 1.15

T.RadioDelegate {
    id: control

    Component.onCompleted: console.log(width)
    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 5
    spacing: 5

    icon.width: 24
    icon.height: 24
    icon.source: "images/bullet-point.svg"
    font.bold: control.checked || control.highlighted

    contentItem: Text {
        leftPadding: !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.mirrored ? control.indicator.width + control.spacing : 0
        text: control.text
        font: control.font
        color: control.enabled ? Calcite.text1 : Calcite.text3
        elide: Text.ElideRight
        visible: control.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    indicator: Image {
        id: indicator
        visible: control.checked || control.highlighted || control.hovered
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        source: icon.source
        sourceSize: Qt.size(icon.width, icon.height)
        width: icon.width
        height: icon.height
        ColorOverlay {
            anchors.fill: indicator
            source: indicator
            color: control.checked
                   || control.highlighted ? Calcite.brand : control.hovered ? Calcite.border1 : "transparent"
            visible: indicator.visible
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.visualFocus
               || control.down ? Calcite.background2 : control.hovered ? Calcite.foreground2 : Calcite.foreground1
    }
}
