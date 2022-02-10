
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
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15

T.ToolButton {
    id: control

    // extra colors not part of calcite specification
    readonly property int theme: Calcite.theme
    readonly property color textDown: theme === Calcite.Theme.Light ? "#004874" : "#59d6ff"
    readonly property color textIdle: theme === Calcite.Theme.Light ? "#2B2B2B" : "#FFFFFF"
    readonly property color backgroundDown: theme === Calcite.Theme.Light ? "#C7EAFF" : "#151515"
    readonly property color backgroundHovered: theme === Calcite.Theme.Light ? "#EAEAEA" : "#2B2B2B"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    padding: 6
    spacing: 6
    icon.width: 24
    icon.height: 24
    icon.color: control.flat ? (control.down || control.checked
                                || control.highlighted ? textDown : textIdle) : control.down
                               || control.checked
                               || control.highlighted ? Calcite.text1 : Calcite.text3
    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: control.icon.color
        alignment: control.display === AbstractButton.TextBesideIcon ? Qt.AlignLeft : Qt.AlignCenter
    }

    background: Rectangle {
        implicitHeight: 48
        implicitWidth: 48

        color: control.flat ? (control.down || control.checked
                               || control.highlighted ? backgroundDown : (control.hovered ? backgroundHovered : Calcite.background)) : control.down
                              || control.checked
                              || control.highlighted ? Calcite.foreground3 : (control.hovered ? Calcite.foreground2 : Calcite.foreground1)
    }
}
