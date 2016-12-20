/*******************************************************************************
 * Copyright 2012-2016 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import Esri.ArcGISExtras 1.1

/*!
    \qmltype PopupViewBase
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since 2.0
    \brief
*/

Item {
    id: popupViewBase
    anchors.fill: parent

    /* public properties */
    property real scaleFactor: System.displayScaleFactor
    property color backgroundColor: "#f2f3f4"
    property color borderColor: "#4f4f4f"
    property real borderWidth: 2 * scaleFactor
    property real popupRadius: 2 * scaleFactor
    property string titleText: popupManagerInternal ? popupManagerInternal.title : ""
    property real titleTextSize: 13 * scaleFactor
    property var popupManagerInternal: null
    property var displayedFields: popupManagerInternal ? popupManagerInternal.displayedFields : null
    property var attachments: popupManagerInternal ? popupManagerInternal.attachmentMananger.attachmentsModel : null
    property bool useFilteredAttachmentsList: false
    
    onUseFilteredAttachmentsListChanged: {
        if (attachments)
            attachments.applyFilter = useFilteredAttachmentsList;
    }
    
    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        border {
            color: borderColor
            width: borderWidth
        }
        radius: popupRadius
        clip: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                mouse.accepted = true;
            }
        }

        Column {
            id: heading
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 10 * scaleFactor
            }
            spacing: 10
            clip: true

            Text {
                width: parent.width
                text: titleText + ":"
                elide: Text.ElideRight
                font {
                    family: "serif"
                    pixelSize: titleTextSize
                    bold: true
                }
                renderType: Text.NativeRendering
            }

            Rectangle {
                width: parent.width
                height: 1 * scaleFactor
                color: borderColor
            }
        }

        Flickable {
            anchors {
                left: parent.left
                right: parent.right
                top: heading.bottom
                bottom: parent.bottom
                margins: 10 * scaleFactor
            }
            contentHeight: popupColumn.height
            clip: true

            Column {
                id: popupColumn
                width: parent.width
                spacing: 10
                clip: true


                // display of attributes
                Column {
                    spacing: 5
                    width: parent.width
                    Repeater {
                        model: displayedFields

                        Row {
                            anchors.left: parent.left
                            clip: true
                            spacing: 5

                            Text {
                                width: popupColumn.width * 0.55
                                text: fieldName ? fieldName : ""
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 14 * scaleFactor
                                color: "gray"
                            }

                            Text {
                                width: popupColumn.width * 0.4
                                text: fieldValue ? fieldValue : ""
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 14 * scaleFactor
                                color: borderColor
                            }
                        }
                    }
                }

                Text {
                    text: "Attachments:"
                    font {
                        family: "serif"
                        pixelSize: titleTextSize
                        bold: true
                    }
                    renderType: Text.NativeRendering
                }

                // display of attachments
                Column {
                    spacing: 5
                    width: parent.width

                    Repeater {
                        model: attachments

                        Column {
                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Url"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: attachmentUrl
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Name"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: name
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Content Type"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: contentType
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Size"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: size
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Local"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: local
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Thumbnail"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Image {
                                    source: thumbnailUrl
                                }
                            }

                            Row {
                                anchors.left: parent.left
                                clip: true
                                spacing: 5

                                Text {
                                    width: popupColumn.width * 0.6
                                    text: "Full Image"
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 14 * scaleFactor
                                    color: borderColor
                                }

                                Image {
                                    source: fullImageUrl
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
