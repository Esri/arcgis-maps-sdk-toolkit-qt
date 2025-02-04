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
    // height: childrenRect.height

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

    // ColumnLayout originally....
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

    // ListView {
    //     id: lv
    //     clip: true
    //     focus: true
    //     Layout.preferredHeight: childrenRect.height
    //     spacing: attachmentsPopupElementView.mediaMargin
    //     model: attachmentsPopupElementView.controller.popupAttachmentItems

    //     Layout.fillWidth: true
    //     Layout.leftMargin: attachmentsPopupElementView.mediaMargin
    //     Layout.rightMargin: attachmentsPopupElementView.mediaMargin

    //     delegate: GridLayout {
    //         // width: parent.width
    //         columns: 2
    //         rows: 2
    //         width: parent.width
    //         // Layout.alignment: Qt.AlignLeft

    //         Image {
    //             id: downIndicatorImage
    //             // Layout.preferredHeight: 32
    //             // Layout.preferredWidth:  32
    //             fillMode: Image.PreserveAspectFit
    //             // Layout.alignment: Qt.AlignVCenter
    //             // source: "qrc:/esri.com/imports/Calcite/images/download-to.svg"
    //             source: "qrc:/esri.com/imports/Calcite/images/download.svg"

    //             // sourceSize.width: 32
    //             // sourceSize.height: 32
    //         }

    //         Label {
    //             id: attachmentLabel
    //             text: model.name
    //             Layout.fillWidth: true
    //             wrapMode: Text.NoWrap
    //             Layout.preferredHeight: downIndicatorImage.height
    //             font.weight: Font.Bold
    //             // Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    //         }

    //         MenuSeparator {
    //             Layout.fillWidth: true
    //             Layout.columnSpan: 2
    //         }
    //     }

    //     // delegate: Column {
    //     //     width: parent.width

    //     //     Label {
    //     //         text: model.name
    //     //         wrapMode: Text.WordWrap
    //     //         width: parent.width
    //     //         font.weight: Font.Bold
    //     //         rightPadding: 10
    //     //         leftPadding: 20
    //     //     }
    //     // }

    //     // delegate: GridLayout {
    //     //     // width: parent.width
    //     //     columns: 2
    //     //     rows: 1
    //     //     width: parent.width
    //     //     Layout.alignment: Qt.AlignLeft

    //     //     // Rectangle {

    //     //     //     // opacity: 0
    //     //     //     // color: enabled ? (down.hovered ? Calcite.foreground2 : Calcite.foreground1) : Calcite.background
    //     //     //     // border {
    //     //     //     //     color: Calcite.border1
    //     //     //     // }
    //     // // }
    //     //     Item { // Use Item or Rectangle as a container
    //     //         Layout.fillWidth: true
    //     //         Layout.preferredHeight: 32 // Set preferred height for the container

    //     //         MouseArea {
    //     //             anchors.fill: parent // Fill the entire area of the parent
    //     //             onClicked: {
    //     //                 console.log("Mouse clicked on:", model.name); // Handle mouse click event
    //     //             }
    //     //             onEntered: {
    //     //                 // Optional: Change appearance on hover
    //     //                 parent.opacity = 0.8; // Example: Change opacity on hover
    //     //             }
    //     //             onExited: {
    //     //                 parent.opacity = 1.0; // Reset opacity when mouse exits
    //     //             }
    //     //         }

    //     //         Image {
    //     //             id: downIndicatorImage
    //     //             Layout.preferredHeight: 32
    //     //             Layout.preferredWidth:  32
    //     //             fillMode: Image.PreserveAspectFit
    //     //             Layout.alignment: Qt.AlignLeft
    //     //             // source: "qrc:/esri.com/imports/Calcite/images/download-to.svg"
    //     //             source: "qrc:/esri.com/imports/Calcite/images/download.svg"
    //     //         }

    //     //         Label {
    //     //             id: attachmentLabel
    //     //             text: model.name
    //     //             Layout.fillWidth: true
    //     //             wrapMode: Text.NoWrap
    //     //             Layout.preferredHeight: 32
    //     //             font.weight: Font.Bold
    //     //             Layout.alignment: Qt.AlignLeft
    //     //         }
    //     //     }

    //     //     MenuSeparator {
    //     //         Layout.fillWidth: true
    //     //         Layout.columnSpan: 2
    //     //     }
    //     // }

    // }

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

        delegate: Item { // Use Item as a container for the delegate
            width: parent.width // Ensure the item takes the full width of the ListView
            height: childrenRect.height

            MouseArea {
                anchors.fill: parent // Fill the entire area of the parent
                hoverEnabled: true
                property string tempName: ""
                onClicked: {
                    console.log("Mouse clicked on:", model.name); // Handle mouse click event
                    if (!model.listModelData.dataFetched)
                    {
                        model.listModelData.downloadAttachment();
                    }
                }
                onEntered: {
                    // parent.opacity = 0.8; // Optional: Change appearance on hover
                    print(tempName);
                    if( tempName === "")
                    {
                        tempName = name.text;
                    }
                    if(model.dataFetched)
                    {
                        name.text = "click to open";
                    }

                }
                onExited: {
                    // parent.opacity = 1.0; // Reset appearance when mouse exits
                    print("exit");
                    // if( tempName !== "")
                    // {
                    //     tempName = name.text;
                    // }
                    if(model.dataFetched)
                    {
                        name.text = tempName;
                    }
                }
            }

            GridLayout {
                columns: 2
                rows: 2
                width: parent.width
                clip: true
                columnSpacing: 0
                rowSpacing: 0

                // Image {
                //     id: downIndicatorImage
                //     fillMode: Image.PreserveAspectFit
                //     visible: !model.dataFetched
                //     source: "qrc:/esri.com/imports/Calcite/images/download.svg"
                // }

                Column {
                    Layout.fillWidth: true
                    clip: true
                    Label {
                        id: name
                        text: model.name
                        width: parent.width
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        onWidthChanged: print(width);


                        // visible: !fetchAttachment.running
                    }
                    Label {
                        id: fileSize
                        text: model.size
                        width: parent.width
                        wrapMode: Text.NoWrap
                        font.weight: Font.Bold
                        elide: Text.ElideRight

                        // visible: !fetchAttachment.running
                    }

                    // BusyIndicator {
                    //     id: fetchAttachment
                    //     Layout.fillWidth: true
                    //     Layout.alignment: Qt.AlignHCenter
                    //     running: model.listModelData.fetchingAttachment
                    //     visible: model.listModelData.fetchingAttachment
                    // }
                }

                Image {
                    id: downIndicatorImage
                    fillMode: Image.PreserveAspectFit
                    visible: !fetchAttachment.running ? !model.dataFetched : !fetchAttachment.running
                    source: "qrc:/esri.com/imports/Calcite/images/download.svg"
                }

                BusyIndicator {
                    id: fetchAttachment
                    // Layout.fillWidth: true
                    // Layout.alignment: Qt.AlignHCenter
                    // Layout.preferredWidth: downIndicatorImage.width
                    running: model.listModelData.fetchingAttachment
                    visible: model.listModelData.fetchingAttachment
                }

                // Label {
                //     id: attachmentLabel
                //     text: model.name
                //     Layout.fillWidth: true
                //     wrapMode: Text.NoWrap
                //     Layout.preferredHeight: downIndicatorImage.height
                //     font.weight: Font.Bold
                // }

                MenuSeparator {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                }
            }
        }
    }
}
