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

Item {
    id: attachmentListView
    visible: false

    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief Whether the contentType role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showType: true

    /*!
        \brief Whether the dataFetched role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showDataFetched: true

    /*!
        \brief Whether the size role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showSize: true

    /*!
        \brief Whether the attachmentUrl role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showUrl: true

    /*!
        \brief Whether thumbnails should be displayed for image attachments.
        The default value is \c true.
    */
    property bool showImage: true

    /*!
        \brief The color of the title bar of the view.
        The default color is \c "#3F51B5".
    */
    property color titleBarColor: "#3F51B5"

    /*!
        \brief The color of the text and buttons in the title bar.
        The default color is \c "#FFFFFF".
    */
    property color titleTextColor: "#FFFFFF"

    /*!
        \brief The background color of the unexpanded items in the view.
        The default color is \c "#F5F5F5".
    */
    property color backgroundColor: "#F5F5F5"

    /*!
        \brief The color of the text and buttons in the expanded attachment information view.
        The default color is \c "#FFFFFF".
    */
    property color informationTextColor: "#FFFFFF"

    /*!
        \brief The background color of the view.
        The default color is \c "#3F51B5".
    */
    property color infoBackgroundColor: "#3F51B5"

    /*!
        \brief The color of the symbol that displays the expansion status of an item.
        The default color is \c "darkgray".
    */
    property color expandButtonColor: "darkgray"

    /*!
        \brief The color of the symbol that displays the expansion status of an item.
        The default color is \c "darkgray".
    */
    property real rowHeight: 80 * scaleFactor

    /*!
        \brief This property holds the model that provides data for the list of attachments.
        When displaying the attachments of an ArcGISFeature, the ArcGISFeature must be loaded before using its attachments as a model.
    */
    property var model: null

    /*!
      \qmlsignal AttachmentListView::addButtonClicked(string fileUrl, string contentType, string name)
      \brief Emitted when the add button is clicked in the view and file is selected from the FileDialog.

      The signal parameters pass the necessary data to add the file as an attachment to an AttachmentListModel using the addAttachment function.
     */
    signal addButtonClicked(string fileUrl, string contentType, string name);

    /*!
      \qmlsignal AttachmentListView::deleteButtonClicked(int attachmentIndex)
      \brief Emitted when the delete button is clicked in the view

      The signal parameter pass the index of attachment to be deleted. It can be deleted using the AttachmentListModel's deleteAttachment function.
     */
    signal deleteButtonClicked(int attachmentIndex);

    /*!
      \qmlsignal AttachmentListView::deleteButtonBlicked(int attachmentIndex)
      \brief Emitted when the thumbnail of an image attachment is clicked.

      The signal parameter passes a string containing the Url of the attachment.
     */
    signal imageClicked(string imageUrl);

    /*!
      \qmlsignal AttachmentListView::closeButtonBlicked()
      \brief Emitted when the close button is clicked.
     */
    signal closeButtonClicked();

    // do not allow mouse actions to propagate down
    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
        onWheel: wheel.accepted = true
    }

    Column {
        anchors.fill: parent
        height: attachmentListView.height
        width: attachmentListView.width

        Rectangle {
            id: titleBar
            width: attachmentListView.width
            height: rowHeight * 0.75
            color: titleBarColor

            Text {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                renderType: Text.NativeRendering
                font.pixelSize: titleBar.height / 4
                text: "Attachments"
                color: titleTextColor
            }

            Rectangle {
                anchors {
                    right: closeButton.left
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                visible: attachmentListView.model !== null
                width: 45 * scaleFactor
                height: titleBar.height / 1.5
                color: "transparent"
                border.color: titleTextColor

                Text {
                    anchors.centerIn: parent
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    color: titleTextColor
                    text : "Add"
                }

                MouseArea {
                    anchors.fill: parent

                    // open file dialog so file can be selected
                    onClicked: {
                        fileDialog.open();
                    }
                }
            }

            Rectangle {
                id: closeButton
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                visible: attachmentListView.model !== null
                width: 45 * scaleFactor
                height: titleBar.height / 1.5
                color: "transparent"
                border.color: titleTextColor

                Text {
                    anchors.centerIn: parent
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    color: titleTextColor
                    text : "X"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        attachmentListView.visible = false;
                        closeButtonClicked();
                    }
                }
            }
        }

        ListView {
            height: attachmentListView.height - titleBar.height
            width: attachmentListView.width
            clip: true
            model: attachmentListView.model

            delegate: Column {
                Rectangle {
                    height: 25 * scaleFactor
                    width: attachmentListView.width
                    clip: true
                    color: backgroundColor

                    // show the attachment name
                    Text {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            margins: 5 * scaleFactor
                        }
                        text: name
                        width: attachmentListView.width - canvas.width - 10 * scaleFactor
                        maximumLineCount: 1
                        clip: true
                        elide: Text.ElideRight
                        font.pixelSize: 16 * scaleFactor
                        renderType: Text.NativeRendering
                    }

                    Canvas {
                        id: canvas
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                            margins:  5 * scaleFactor
                        }
                        width: attachmentListView.width / 20
                        height: width

                        Behavior on rotation {
                            SmoothedAnimation {
                                duration: 650
                            }
                        }

                        // draw a triangle
                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.clearRect(0, 0, canvas.width, canvas.height);
                            ctx.strokeStyle = expandButtonColor;
                            ctx.fillStyle = expandButtonColor;
                            ctx.lineJoin = "round";

                            ctx.moveTo(0, canvas.height * 0.10);
                            ctx.lineTo(canvas.width, canvas.height * 0.10);
                            ctx.lineTo(canvas.width / 2, canvas.height / 2 + canvas.height * 0.10);

                            ctx.closePath();
                            ctx.stroke();
                            ctx.fill();
                            ctx.restore();
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            canvas.rotation += 180;
                            informationRect.expanded = !informationRect.expanded;
                        }
                    }
                }

                // expanded information window
                Rectangle {
                    id: informationRect
                    height: expanded ? rowHeight : 0
                    width: attachmentListView.width
                    clip: true
                    color: infoBackgroundColor

                    property bool expanded: false

                    Behavior on height {
                        SpringAnimation {
                            spring: 3
                            damping: .4
                        }
                    }

                    // displayed roles on left
                    Column {
                        anchors {
                            left: parent.left
                            margins: 5 * scaleFactor
                        }

                        Text {
                            visible: showUrl
                            width: attachmentListView.width * 0.97
                            text: attachmentUrl
                            clip: true
                            maximumLineCount: 1
                            elide: Text.ElideLeft
                            font.pixelSize: 12 * scaleFactor
                            renderType: Text.NativeRendering
                            color: informationTextColor
                        }

                        Text {
                            visible: showType
                            text: "Type: " + contentType
                            wrapMode: Text.WrapAnywhere
                            maximumLineCount: 1
                            elide: Text.ElideRight
                            font.pixelSize: 15 * scaleFactor
                            renderType: Text.NativeRendering
                            color: informationTextColor
                        }

                        Text {
                            visible: showSize
                            text: "Size: " + size + " bytes"
                            wrapMode: Text.WrapAnywhere
                            maximumLineCount: 1
                            elide: Text.ElideRight
                            font.pixelSize: 15 * scaleFactor
                            renderType: Text.NativeRendering
                            color: informationTextColor
                        }

                        Text {
                            visible: showDataFetched
                            text: "Fetched: " + dataFetched
                            wrapMode: Text.WrapAnywhere
                            maximumLineCount: 1
                            elide: Text.ElideRight
                            font.pixelSize: 15 * scaleFactor
                            renderType: Text.NativeRendering
                            color: informationTextColor
                        }
                    }

                    // Delete button and thumbnail on right side
                    Row {
                        anchors {
                            bottom: parent.bottom
                            right: parent.right
                            margins: 5 * scaleFactor
                        }
                        spacing: 10 * scaleFactor

                        Rectangle {
                            width: 44 * scaleFactor
                            height: width
                            color: "transparent"
                            border.color: titleTextColor

                            Text {
                                anchors.centerIn: parent
                                renderType: Text.NativeRendering
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 12 * scaleFactor
                                color: titleTextColor
                                text : "Delete"
                            }

                            MouseArea {
                                anchors.fill: parent

                                // emit signal so user can handle deletion
                                onClicked: {
                                    attachmentListView.deleteButtonClicked(index);
                                }
                            }
                        }

                        // view will not load images if showImage property is false
                        Loader {
                            active: showImage
                            visible: showImage

                            sourceComponent: Component {
                                Image {
                                    id: image
                                    visible: showImage && status !== Image.Error
                                    width: 44 * scaleFactor
                                    height: width
                                    fillMode: Image.PreserveAspectFit
                                    source: attachmentUrl

                                    // refine this idea a little bit
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            attachmentListView.imageClicked(attachmentUrl);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // file dialog for selecting a file to add as an attachment
    FileDialog {
        id: fileDialog

        // when file is selected, emit signal that passes necessary data to add an attachment
        onAccepted: {
            fileInfo.url = fileDialog.fileUrl;
            attachmentListView.addButtonClicked(fileDialog.fileUrl, "application/octet-stream", fileInfo.fileName);
        }
    }

    // file info used for obtaining the file name
    FileInfo {
        id: fileInfo
    }

    onModelChanged: {
        if (model !== null)
            attachmentListView.visible = true;
    }
}
