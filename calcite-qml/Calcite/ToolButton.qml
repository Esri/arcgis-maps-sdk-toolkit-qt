
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

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6
    icon.width: 24
    icon.height: 24
    icon.color: control.down || control.checked
                || control.highlighted ? Calcite.foreground4 : Calcite.text4
    //icon.color: visualFocus ? control.palette.highlight : control.palette.buttonText
    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: control.down || control.checked
               || control.highlighted ? Calcite.foreground4 : Calcite.text4
        //color: control.visualFocus ? Calcite.text1 : Calcite.text3
    }

    background: Rectangle {
        implicitHeight: 48
        implicitWidth: 48

        //opacity: control.down ? 1.0 : 0.5
        color: control.down || control.checked
               || control.highlighted ? Calcite.background2 : (control.hovered ? Calcite.foreground5 : Calcite.background)
        border.color: control.down || control.checked
                      || control.highlighted ? control.icon.color : "transparent"
    }
}
