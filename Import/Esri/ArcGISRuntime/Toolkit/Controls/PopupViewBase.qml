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
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.11

Item {
    id: popupViewBase

    property color backgroundColorInternal
    property color borderColorInternal
    property real borderWidthInternal
    property real radiusInternal
    property string titleTextInternal
    property real titleTextSizeInternal
    property color titleTextColorInternal
    property color attributeNameTextColorInternal
    property color attributeValueTextColorInternal
    property color closeButtonColorInternal
    property var popupManagerInternal: null
    property var displayedFields: null
    property var attachments: null
    property bool showAttachments: false
    signal attachmentThumbnailClickedInternal(var index)
    signal popupViewDismissed()
    
    onPopupManagerInternalChanged: {
        if (popupManagerInternal) {
            displayedFields = popupManagerInternal.displayedFields;
            attachments = popupManagerInternal.attachmentManager ? popupManagerInternal.attachmentManager.attachmentsModel : null;
            showAttachments = popupManagerInternal.showAttachments;
            titleTextInternal = popupManagerInternal.title;
        }
    }

    /*! internal */
    function drawCloseButton(canvas) {
        var canvasWidth = canvas.width;
        var canvasHeight = canvas.height;

        var ctx = canvas.getContext("2d");
        ctx.strokeStyle = closeButtonColorInternal;
        ctx.lineWidth = canvasHeight / 10;
        ctx.clearRect(0, 0, canvasWidth, canvasHeight);
        ctx.save();

        // place at origin
        ctx.translate(0, 0);
        ctx.beginPath();

        // line 1
        ctx.moveTo(canvasWidth, canvasHeight);
        ctx.lineTo(0, 0);

        // line 2
        ctx.moveTo(canvasWidth, 0);
        ctx.lineTo(0, canvasHeight);

        ctx.stroke();
        ctx.restore();
    }

    Rectangle {
        anchors.fill: parent
        color: backgroundColorInternal
        border {
            color: borderColorInternal
            width: borderWidthInternal
        }
        radius: radiusInternal
        clip: true

        MouseArea {
            anchors.fill: parent
            onClicked: mouse.accepted = true;
            onWheel: wheel.accepted = true;
        }

        Column {
            id: heading
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 10
            }
            spacing: 10
            clip: true

            Row {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    width: heading.width * 0.9
                    text: titleTextInternal
                    elide: Text.ElideRight
                    color: titleTextColorInternal
                    font {
                        family: "serif"
                        pixelSize: titleTextSizeInternal
                        bold: true
                    }
                    renderType: Text.NativeRendering
                }

                Canvas {
                    id: closeButtonCanvas
                    anchors.verticalCenter: parent.verticalCenter
                    antialiasing: true
                    height: parent.height
                    width: height

                    onPaint: {
                        drawCloseButton(closeButtonCanvas);
                    }

                    Component.onCompleted: {
                        if (Qt.platform.os === "ios")
                            renderTarget = Canvas.Image;
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            popupViewDismissed();
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: borderColorInternal
            }
        }

        Flickable {
            anchors {
                left: parent.left
                right: parent.right
                top: heading.bottom
                bottom: parent.bottom
                margins: 10
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
                            visible: attributeVisible

                            Text {
                                width: popupColumn.width * 0.55
                                text: fieldName ? fieldName : ""
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 14
                                color: attributeNameTextColorInternal
                            }

                            Text {
                                width: popupColumn.width * 0.4
                                text: formattedValue
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 14
                                color: attributeValueTextColorInternal
                            }
                        }
                    }
                }

                // display of attachments
                Column {
                    spacing: 5
                    width: parent.width
                    visible: showAttachments

                    Text {
                        text: "Attachments:"
                        font {
                            family: "serif"
                            pixelSize: titleTextSizeInternal
                            bold: true
                        }
                        color: titleTextColorInternal
                        renderType: Text.NativeRendering
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

    onCloseButtonColorInternalChanged: {
        closeButtonCanvas.requestPaint();
    }
}
