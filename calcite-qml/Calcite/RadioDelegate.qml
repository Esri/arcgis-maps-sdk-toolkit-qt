
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
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15

T.RadioDelegate {
    id: control

    // extra colors not part of calcite specification
    readonly property int theme: Calcite.theme
    readonly property color backgroundDown: theme === Calcite.Theme.Light ? "#C7EAFF" : "#151515"
    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 5
    spacing: 5

    font.bold: control.checked || control.highlighted

    contentItem: IconLabel {
        leftPadding: !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.mirrored ? control.indicator.width + control.spacing : 0

        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        alignment: control.display === IconLabel.IconOnly
                   || control.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft

        icon: control.icon
        text: control.text
        font: control.font
        color: control.enabled ? Calcite.text1 : Calcite.text3
    }

    indicator: Image {
        id: indicator
        visible: control.checked || control.highlighted || control.hovered
        // indicator is at the beginning of control.
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        source: "images/bullet-point.svg"
        sourceSize: Qt.size(24, 24)
        width: sourceSize.width
        height: sourceSize.height
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
               || control.down ? backgroundDown : control.hovered ? Calcite.foreground2 : Calcite.foreground1
    }
    opacity: control.enabled ? 1.0 : 0.3
}
