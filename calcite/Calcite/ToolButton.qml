
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
import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl

T.ToolButton {
    id: control

    // extra colors not part of calcite specification
    readonly property int theme: Calcite.theme
    readonly property color textDown: flat ? (theme === Calcite.Theme.Light ? "#004874" : "#59d6ff") : Calcite.text1
    readonly property color backgroundDown: flat ? (theme === Calcite.Theme.Light ? "#C7EAFF" : "#151515") : Calcite.foreground3
    readonly property color backgroundHovered: flat ? (theme === Calcite.Theme.Light ? "#EAEAEA" : "#2B2B2B" ) : Calcite.foreground2
    readonly property color backgroundIdle: Calcite.background
    property var alignment : control.display === AbstractButton.TextBesideIcon ? Qt.AlignLeft : Qt.AlignCenter

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    padding: 6
    spacing: 6
    icon.width: 24
    icon.height: 24
    icon.color: control.down || control.checked
                                || control.highlighted ? textDown : control.hovered && control.enabled ? Calcite.text1 : Calcite.text3
    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: control.icon.color
        alignment: control.alignment
    }

    background: Rectangle {
        implicitHeight: 48
        implicitWidth: 48
        color: control.down || control.checked
                               || control.highlighted ? backgroundDown : control.hovered && control.enabled ? backgroundHovered : backgroundIdle
    }
    opacity: control.enabled ? 1.0 : 0.3
}
