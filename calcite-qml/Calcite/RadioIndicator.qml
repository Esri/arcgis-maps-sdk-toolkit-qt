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
import QtQuick 2.12


Rectangle {
        property var control;

        implicitWidth: 20
        implicitHeight: 20
        border {
            width: 2
            color: control.checked ? control.hovered ? Calcite.brandHover : Calcite.brand : control.hovered ? Calcite.text3 : Calcite.borderInput
        }
        radius: width / 2
        color: Calcite.offWhite
        opacity: enabled ? 1 : 0.3

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 12
            height: 12
            radius: width / 2
            color: control.hovered ? Calcite.brandHover : Calcite.brand
            visible: control.checked
        }
    }
