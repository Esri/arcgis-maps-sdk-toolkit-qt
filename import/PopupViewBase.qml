/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.11
import QtQuick.Controls 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3

Control {
    id: popupViewBase

    property alias popupManager: internal.popupManager
    readonly property alias attachments: internal.attachments

    signal attachmentThumbnailClicked(var index)

    signal popupViewDismissed()

    QtObject {
        id: internal
        property var popupManager: null
        property var attachments: null
        onPopupManagerChanged: {
            if (popupManager && popupManager.attachmentManager) {
                attachments = popupManager.attachmentManager.attachmentsModel
            } else {
                attachments = null
            }
        }
    }

    background: Rectangle {
        color: "#f2f3f4"
        border {
            color: "#4f4f4f"
            width: 2
        }
        radius: 2
    }

    contentItem: ColumnLayout {
        id: heading
        spacing: 10
        clip: true
        height: childrenRect.height

        Text {
            Layout.fillWidth: true
            text: popupManager ? popupManager.title : ""
            elide: Text.ElideRight
            renderType: Text.NativeRendering
        }
        // TODO: CLose button here.

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ColumnLayout {
                anchors.fill: parent

                Repeater {
                    model: popupManager ? popupManager.displayedFields : null;
                    RowLayout {
                        Text {
                            text: fieldName ? fieldName : ""
                            wrapMode: Text.WrapAnywhere
                        }
                        Text {
                            text: formattedValue
                            wrapMode: Text.WrapAnywhere
                        }
                    }
                }

                Text {
                    text: "Attachments"
                }

                Repeater {
                    model: attachments
                    Row {
                        anchors.left: parent.left
                        clip: true
                        spacing: 5

                        Text {
                            width: popupColumn.width * 0.6
                            text: name
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 14
                            color: attributeNameTextColor
                        }

                        Image {
                            source: thumbnailUrl
                            width: attachments.thumbnailWidth * 0.5
                            height: attachments.thumbnailHeight * 0.5

                            MouseArea {
                                anchors.fill: parent
                                onClicked: attachmentThumbnailClickedInternal(index)
                            }
                        }
                    }
                }
            }
        }
    }
}
