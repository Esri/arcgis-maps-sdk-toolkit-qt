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
import QtQuick.Layouts

ListView {
    id: fieldsPopupElementView

    /*!
      \qmlproperty FieldsPopupElementView controller
      \brief The Controller handles reading from the FieldsPopupElement.

      \sa Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController
    */
    property var controller: null

    height: contentHeight
    interactive: false
    model: controller.labelsAndValues
    clip: true
    focus: true
    spacing: 10

    header: Label {
        text: controller.title !== "" ? controller.title : "Fields"
        wrapMode: Text.WordWrap
        width: parent.width
        font.pixelSize: 20
        rightPadding: 10
        leftPadding: 10
    }

    delegate: Column {
        width: parent.width

        MenuSeparator {
            width: parent.width
            leftPadding: 20
            rightPadding: 10
        }
        Label {
            text: model.modelData["label"]
            wrapMode: Text.WordWrap
            width: parent.width
            font.weight: Font.Bold
            rightPadding: 10
            leftPadding: 20
        }
        Label {
            text: model.modelData["formattedValue"]
            wrapMode: Text.WordWrap
            width: parent.width
            rightPadding: 10
            leftPadding: 20
            Component.onCompleted: {
                // Checks to see if the formatted value is a hyperlink.
                // If it is, it will modify the text to be a clickable link
                // displayed as `View`.
                if (text.toLowerCase().startsWith("http")) {
                    let link = text;
                    text = `<a href="${link}">View</a>`;
                    textFormat = Text.RichText;
                }
            }
            onLinkActivated: (link) => {
                if (link !== "" && link !== undefined) {
                    Qt.openUrlExternally(link);
                }
            }
        }
    }
}