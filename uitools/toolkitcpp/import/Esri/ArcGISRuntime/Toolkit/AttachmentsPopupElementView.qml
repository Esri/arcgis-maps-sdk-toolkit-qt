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
        id: lv
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
                    console.log("Mouse clicked on:", model.name); // Handle mouse click event
                    if (!model.listModelData.dataFetched) {
                        model.listModelData.downloadAttachment();
                    } else {
                        let res = Qt.openUrlExternally(model.listModelData.localData);
                    }
                }
                onEntered: {
                    if (model.listModelData.dataFetched && name.text !== "Click to open")
                        name.text = "Click to open";

                }
                onExited: {
                    if (model.dataFetched && name.text === "Click to open")
                        name.text = model.name;
                }
            }

            GridLayout {
                columns: 3
                rows: 2
                width: parent.width
                clip: true
                columnSpacing: 0
                rowSpacing: 0

                Image {
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: fileInfoColumn.height
                    Layout.preferredWidth: fileInfoColumn.height
                    Component.onCompleted: {
                        switch(model.listModelData.popupAttachmentType)
                        {
                            case QmlEnums.PopupAttachmentTypeImage:
                                source = "qrc:/esri.com/imports/Calcite/images/image.svg";
                                break;
                            case QmlEnums.PopupAttachmentTypeVideo:
                                source = "qrc:/esri.com/imports/Calcite/images/video.svg";
                                break;
                            case QmlEnums.PopupAttachmentTypeDocument:
                                source = "qrc:/esri.com/imports/Calcite/images/file.svg";
                                break;
                            case QmlEnums.PopupAttachmentTypeOther:
                                source = "qrc:/esri.com/imports/Calcite/images/other.svg";
                                break;
                            default:
                                source = "qrc:/esri.com/imports/Calcite/images/other.svg";
                                break;
                        }
                    }
                }

                Column {
                    id: fileInfoColumn

                    Layout.fillWidth: true
                    clip: true
                    Label {
                        id: name
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
