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


ColumnLayout {
    id: mediaPopupElementView
    height: childrenRect.height
    clip: true
    focus: true
    Layout.fillWidth: true
    spacing: layoutSpacing

    property real mediaMargin: 10
    property real imageTextMargin: 5
    property real layoutSpacing: 0

    /*!
      \qmlproperty MediaPopupElementView controller
      \brief The Controller handles reading from the MediaPopupElement.

      \sa Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController
    */
    property var controller: null

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: mediaMargin
        Layout.rightMargin: mediaMargin
    }

    ColumnLayout {
        clip: true
        focus: true
        spacing: layoutSpacing

        Label {
            text: controller.title !== "" ? controller.title : "Media"
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            font.weight: Font.Black

            Layout.fillWidth: true
            Layout.leftMargin: mediaMargin
            Layout.rightMargin: mediaMargin
        }
        Label {
            text: controller.description
            visible: controller.description !== ""
            wrapMode: Text.WordWrap

            Layout.fillWidth: true
            Layout.leftMargin: mediaMargin
            Layout.rightMargin: mediaMargin
        }
    }

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: mediaMargin
        Layout.rightMargin: mediaMargin
    }

    ListView {
        orientation: ListView.Horizontal
        clip: true
        focus: true
        height: 170
        spacing: mediaMargin
        model: controller.popupMediaItems

        Layout.fillWidth: true
        Layout.leftMargin: mediaMargin
        Layout.rightMargin: mediaMargin

        delegate: Item {
            height: 170
            width: 220

            Image {
                id: imageMediaPreview
                source: model.sourceUrl
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                cache: true

                anchors.fill: parent
                Layout.leftMargin: mediaMargin

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.openUrlExternally(model.linkUrl);
                    }
                }
            }

            // Overlay text
            Rectangle {
                visible: model.title !== ""
                height: overlayTextLayout.height
                color: "white"
                opacity: 0.7

                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                ColumnLayout {
                    id: overlayTextLayout
                    height: childrenRect.height
                    clip: true
                    focus: true
                    spacing: layoutSpacing

                    anchors.bottom: parent.bottom

                    Label {
                        visible: model.title !== ""
                        text: model.title
                        font.weight: Font.Bold
                        color: "black"
                        elide: Text.ElideRight

                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: imageMediaPreview.width - imageTextMargin
                        Layout.leftMargin: imageTextMargin
                        Layout.rightMargin: imageTextMargin
                    }

                    Label {
                        visible: model.caption !== ""
                        text: model.caption
                        color: "black"
                        elide: Text.ElideRight

                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: imageMediaPreview.width - imageTextMargin
                        Layout.leftMargin: imageTextMargin
                        Layout.rightMargin: imageTextMargin
                    }
                }
            }
        }
    }
}

