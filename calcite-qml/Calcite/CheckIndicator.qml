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
import QtGraphicalEffects 1.12

Rectangle {
    property var control;
    width: 24
    height: 24
    border {
        width: 2
        color: control.checkState !== Qt.Unchecked ? control.hovered ? Calcite.brandHover : Calcite.brand :
        control.hovered ? Calcite.text3 : Calcite.borderInput
    }
    color: control.checkState === Qt.Unchecked ? Calcite.offWhite : control.hovered ? Calcite.brandHover : Calcite.brand
    radius: 1
    opacity: enabled ? 1.0 : 0.3

    Image {
        id: checkmark
        anchors {
            fill: parent
            margins: control.checkState === Qt.Checked ? -2 : 2
        }
        source: control.checkState === Qt.Checked ? "images/check.svg": "images/line-solid.svg"
        visible: false
    }

    ColorOverlay {
        anchors.fill: checkmark
        source: checkmark
        color: Calcite.textInverse
        visible: control.checkState !== Qt.Unchecked
    }
}
