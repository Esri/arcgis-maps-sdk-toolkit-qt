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

T.ProgressBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    contentItem: Item {
        implicitWidth: 160
        implicitHeight: 4
        clip: true

        Rectangle {
            id: bar
            height: parent.height
            radius: 2
            color: Calcite.brand
            width: control.indeterminate ? parent.width * 0.4 : control.position * parent.width
            x: 0

            SequentialAnimation on x {
                loops: Animation.Infinite
                running: control.indeterminate

                PropertyAction { value: - parent.width * 0.4 }
                NumberAnimation {
                    to: bar.parent.width
                    duration: 1000
                }

                PauseAnimation { duration: 1250 }
            }
        }
    }

    background: Rectangle {
        implicitWidth: 160
        implicitHeight: 4
        color: Calcite.border2
        radius: 2
        opacity: control.enabled ? 1.0 : 0.3
    }
}
