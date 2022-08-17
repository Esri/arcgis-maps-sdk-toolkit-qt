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

T.DialogButtonBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)

    spacing: 5
    padding: 16
    delegate: Button { }

    contentItem: ListView {
          model: control.contentModel
          spacing: control.spacing
          orientation: ListView.Horizontal
          boundsBehavior: Flickable.StopAtBounds
          snapMode: ListView.SnapToItem
      }

        background: Rectangle {
            implicitHeight: 60
            x: 2.5
            y: 2.5
            width: parent.width - 5
            height: parent.height - 5
            border {
                width: 1
                color: Calcite.border1
            }
            color: Calcite.background
        }
}
