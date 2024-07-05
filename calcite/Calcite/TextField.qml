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

T.TextField {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            placeholderText ? placeholder.implicitWidth + leftPadding + rightPadding : 0)
                   || contentWidth + leftPadding + rightPadding
    implicitHeight: Math.max(contentHeight + topPadding + bottomPadding,
                             background ? background.implicitHeight : 0,
                             placeholder.implicitHeight + topPadding + bottomPadding)

    leftPadding: 5
    rightPadding: 5
    topPadding: 5
    bottomPadding: 5

    color: control.enabled ? Calcite.text1 : Calcite.text2
    selectionColor: Calcite.brandHover
    selectedTextColor: Calcite.offWhite
    verticalAlignment: TextInput.AlignVCenter
    cursorDelegate: Rectangle {
        id: cursor
        color: Calcite.text1
        width: 1
        visible: control.activeFocus && control.selectionStart === control.selectionEnd

        Connections {
            target: control
            function onCursorPositionChanged() {
                // keep a moving cursor visible
                cursor.opacity = 1
                timer.restart()
            }
        }

        Timer {
            id: timer
            running: control.activeFocus
            repeat: true
            interval: Qt.styleHints.cursorFlashTime / 2
            onTriggered: cursor.opacity = !cursor.opacity ? 1 : 0
            // force the cursor visible when gaining focus
            onRunningChanged: cursor.opacity = 1
        }
    }

    Text {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)
        text: control.placeholderText
        font: control.font
        color: Calcite.text3
        horizontalAlignment: control.horizontalAlignment
        verticalAlignment: control.verticalAlignment
        elide: Text.ElideRight
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
    }

    background: Rectangle {
        implicitWidth: 150
        color: Calcite.foreground1
        opacity: control.enabled ? 1 : 0.3
        border {
            width: 1
            color: !control.acceptableInput ? control.activeFocus ? Calcite.dangerHover : Calcite.danger : control.activeFocus ? Calcite.brand : Calcite.borderInput
            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }
        }
    }
}
