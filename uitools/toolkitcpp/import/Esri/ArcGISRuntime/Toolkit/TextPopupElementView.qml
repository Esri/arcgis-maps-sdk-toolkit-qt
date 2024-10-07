/*******************************************************************************
 *  Copyright 2012-2024 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window
import QtQuick.Layouts

Item {
    id: textPopupElementView

    /*!
      \qmlproperty TextPopupElementView controller
      \brief The Controller handles reading from the TextPopupElement.

      The CPP controller is documented \l{Esri::ArcGISRuntime::Toolkit::TextPopupElementViewController}{here}.
    */
    property var controller: null

    implicitHeight: richText.implicitHeight
    Rectangle {
        anchors.fill: parent

        Text {
            id: richText
            text: controller ? controller.text : ""
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            anchors.fill: parent
            padding: 10

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    let link = richText.hoveredLink;

                    if (link !== "" && link !== undefined) {
                        Qt.openUrlExternally(link);
                    }
                }
            }
        }
    }
}
