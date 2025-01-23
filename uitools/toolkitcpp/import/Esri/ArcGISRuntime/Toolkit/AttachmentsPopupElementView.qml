/*******************************************************************************
 *  Copyright 2012-2025 Esri
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

import QtGraphs
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*!
  \internal
  This is an internal implementation detail and is subject to change.
*/

ColumnLayout {
    id: attachmentsPopupElementView
    height: childrenRect.height
    clip: true
    focus: true
    Layout.fillWidth: true
    spacing: layoutSpacing

    property real mediaMargin: 10
    property real imageTextMargin: 5
    property real layoutSpacing: 0

    property var controller: null

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: attachmentsPopupElementView.mediaMargin
        Layout.rightMargin: attachmentsPopupElementView.mediaMargin
    }

    ColumnLayout {
        clip: true
        focus: true
        spacing: attachmentsPopupElementView.layoutSpacing

        Label {
            text: attachmentsPopupElementView.controller.title !== "" ? controller.title : "Attachments"
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            font.weight: Font.Black

            Layout.fillWidth: true
            Layout.leftMargin: attachmentsPopupElementView.mediaMargin
            Layout.rightMargin: attachmentsPopupElementView.mediaMargin
        }
        Label {
            text: attachmentsPopupElementView.controller.description
            visible: attachmentsPopupElementView.controller.description !== ""
            wrapMode: Text.WordWrap

            Layout.fillWidth: true
            Layout.leftMargin: attachmentsPopupElementView.mediaMargin
            Layout.rightMargin: attachmentsPopupElementView.mediaMargin
        }
    }

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: attachmentsPopupElementView.mediaMargin
        Layout.rightMargin: attachmentsPopupElementView.mediaMargin
    }

    ListView {
        id: lv
        // orientation: ListView.Horizontal
        clip: true
        focus: true
        implicitHeight: 170
        spacing: attachmentsPopupElementView.mediaMargin
        model: attachmentsPopupElementView.controller.popupAttachmentItems

        Layout.fillWidth: true
        Layout.leftMargin: attachmentsPopupElementView.mediaMargin
        Layout.rightMargin: attachmentsPopupElementView.mediaMargin

        delegate: Column {
            width: parent.width

            Label {
                text: model.name
                wrapMode: Text.WordWrap
                width: parent.width
                font.weight: Font.Bold
                rightPadding: 10
                leftPadding: 20
            }
        }
    }
}
