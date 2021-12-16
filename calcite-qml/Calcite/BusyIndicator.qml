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
import QtQuick.Layouts 1.15

T.BusyIndicator {
    id: control

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding

    spacing: 5

    contentItem: Item {
        id: rects
        implicitWidth: 3 * (rectWidth + spacing)
        implicitHeight: rectMaxHeight

        readonly property double rectWidth: 13.6
        readonly property double rectHeight: 32
        readonly property double rectMaxHeight: 45

        Rectangle {
            id: rect1
            color: Calcite.brand
            width: rects.rectWidth
            height: rects.rectHeight
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            BusyIndicatorAnimation {
                id: anim1
                target: rect1
                running: control.visible && control.running
            }
        }

        Rectangle {
            id: rect2
            width: rects.rectWidth
            height: rects.rectHeight
            anchors.centerIn: parent
            color: Calcite.brand
            BusyIndicatorAnimation {
                id: anim2
                target: rect2
                delay: 160
                running: control.visible && control.running
            }
        }

        Rectangle {
            id: rect3
            width: rects.rectWidth
            height: rects.rectHeight
            color: Calcite.brand
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            BusyIndicatorAnimation {
                id: anim3
                target: rect3
                delay: 320
                running: control.visible && control.running
            }
        }
    }
}
