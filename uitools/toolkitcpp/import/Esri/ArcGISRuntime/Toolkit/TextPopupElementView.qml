/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
import QtWebView

Item {
    id: textPopupElementView
    property var popupElement: null

    property var controller: null

    Binding {
        target: controller
        property: "popupElement"
        value: textPopupElementView.popupElement
    }

    implicitHeight: richText.implicitHeight
    Rectangle {
        anchors.fill: parent

        Text {
            id: richText
            text: controller.text
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