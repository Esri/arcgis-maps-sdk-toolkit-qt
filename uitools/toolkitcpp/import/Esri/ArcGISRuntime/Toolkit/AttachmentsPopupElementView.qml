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

    clip: true
    focus: true
    Layout.fillWidth: true
    spacing: layoutSpacing

    property real mediaMargin: 10
    property real imageTextMargin: 5
    property real layoutSpacing: 0

    property var controller: null

    Dialog {
        id: fullScreenImageDialog
        modal: true
        visible: false
        width: Overlay.overlay.width
        height: Overlay.overlay.height
        anchors.centerIn: Overlay.overlay

        Image {
            id: fullScreenImage
            anchors{
                top: parent.top
                bottom: closeButton.top
                left: parent.left
                right: parent.right
            }
            fillMode: Image.PreserveAspectFit
        }

        Button {
            id: closeButton
            text: "Close"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: fullScreenImageDialog.visible = false;
        }
    }

    MenuSeparator {
        Layout.fillWidth: true
        Layout.leftMargin: attachmentsPopupElementView.mediaMargin
        Layout.rightMargin: attachmentsPopupElementView.mediaMargin
    }

    Column {
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
        clip: true
        focus: true
        Layout.preferredHeight: childrenRect.height
        spacing: attachmentsPopupElementView.mediaMargin
        model: attachmentsPopupElementView.controller.popupAttachmentItems

        Layout.fillWidth: true
        Layout.leftMargin: attachmentsPopupElementView.mediaMargin
        Layout.rightMargin: attachmentsPopupElementView.mediaMargin

        delegate: Item {
            width: parent.width
            height: childrenRect.height

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    if (!model.listModelData.dataFetched) {
                        model.listModelData.downloadAttachment();
                    } else {
                        if( model.listModelData.popupAttachmentType === QmlEnums.PopupAttachmentTypeImage) {
                            fullScreenImage.source = model.listModelData.localData;
                            fullScreenImageDialog.visible = true;
                        } else {
                            if (Qt.platform.os !== "android" && Qt.platform.os !== "ios") {
                                Qt.openUrlExternally(model.listModelData.localData);
                            }
                        }
                    }
                }
                onEntered: {
                    if (model.listModelData.dataFetched && nameLabel.text !== "Click to open")
                        nameLabel.text = "Click to open";

                }
                onExited: {
                    if (model.dataFetched && nameLabel.text === "Click to open")
                        nameLabel.text = model.name;
                }
            }

            GridLayout {
                columns: 3
                rows: 2
                width: parent.width
                clip: true
                columnSpacing: 5
                rowSpacing: 0

                Image {
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: fileInfoColumn.height
                    Layout.preferredWidth: fileInfoColumn.height
                    source: model.listModelData.thumbnailUrl
                }

                Column {
                    id: fileInfoColumn

                    Layout.fillWidth: true
                    clip: true
                    Label {
                        id: nameLabel
                        text: model.name
                        width: parent.width
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                    }
                    Label {
                        id: fileSize
                        text: model.size
                        width: parent.width
                        wrapMode: Text.NoWrap
                        font.weight: Font.Bold
                        elide: Text.ElideRight
                    }
                }

                Image {
                    id: downloadImage
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: fileInfoColumn.height
                    Layout.preferredWidth: fileInfoColumn.height
                    visible: !fetchAttachment.running ? !model.dataFetched : !fetchAttachment.running
                    source: "qrc:/esri.com/imports/Calcite/images/download.svg"
                }

                BusyIndicator {
                    id: fetchAttachment
                    running: model.listModelData.fetchingAttachment
                    visible: model.listModelData.fetchingAttachment
                }

                MenuSeparator {
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                }
            }
        }
    }
}
