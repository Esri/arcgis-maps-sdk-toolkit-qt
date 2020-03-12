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

    property var popupManager: null

    signal attachmentThumbnailClicked(var index)

    /*!
    * \qmlproperty PopupViewController controller
    * \brief the Controller handles connections writing/reading to the popupManager.
    */
    property var controller: PopupViewController { }

    Binding {
        target: controller
        property: "popupManager"
        value: popupViewBase.popupManager
    }

    implicitWidth: 300 + padding

    implicitHeight: 300 + padding

    padding: 5

    background: Rectangle {
        color: palette.base
        border {
            color: palette.shadow
            width: 2
        }
        radius: 2
    }

    contentItem: Flickable {
        id: flickable
        clip: true
        contentHeight: fieldsLayout.height
        GridLayout {
            id: fieldsLayout
            flow: GridLayout.TopToBottom
            anchors {
                left: parent.left
                right: parent.right
            }
            rows: controller.fieldCount + controller.attachmentCount + 2

            // Title
            Text {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                textFormat: Text.StyledText
                visible: controller.fieldCount > 0
                text: `<h2>${popupManager ? popupManager.title : ""}</h2>`
                color: palette.text
            }

            // Field names
            Repeater {
                model: controller.displayFields
                Text {
                    Layout.fillWidth: true
                    text: fieldName ? fieldName : ""
                    wrapMode: Text.WrapAnywhere
                    color: palette.text
                }
            }

            // Attachments header
            Text {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                textFormat: Text.StyledText
                visible: controller.attachmentCount > 0
                text: "<h2>Attachments</h2>"
                color: palette.text
            }

            // Attachment names
            Repeater {
                model: controller.attachments
                Text {
                    Layout.fillWidth: true
                    text: name
                    wrapMode: Text.WrapAnywhere
                    color: palette.text
                }
            }

            // Field contents
            Repeater {
                model: controller.displayFields
                Text {
                    Layout.fillWidth: true
                    text: formattedValue
                    wrapMode: Text.WrapAnywhere
                    color: palette.text
                }
            }

            // Attachment images
            Repeater {
                model: controller.attachments
                Image {
                    source: thumbnailUrl
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            attachmentThumbnailClicked(index)
                        }
                    }
                }
            }
        }
    }
}
