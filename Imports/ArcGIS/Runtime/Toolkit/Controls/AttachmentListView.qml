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
    \qmltype AttachmentListView
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since 2.0
    \brief A view for displaying and editing attachments in an AttachmentListModel from an ArcGISFeature.

    Although the AttachmentListView provides graphical ways to interact with attachments, it cannot directly modify a model.
    In order to add and remove attachments, the appropriate AttachmentListModel methods must be called in the onAddButtonClicked and onDeleteButtonClicked signal handlers using the signal parameters.
    Please see the AttachmentListViewExample in the Toolkit for an example on how to do this.
*/

Item {
    id: attachmentListView
    
    
    /*========================================
         Configurable properties
    ========================================*/

    /*!
        \brief Whether the view is visible.
        The default is \c false.
    */
    visible: false
    
    /*!
        \brief The view's opacity.
        The default is \c 0.95.
    */
    opacity: 0.95
    
    /*!
        \brief The view's height.
        The default is \c 200.
    */
    height: 200 * scaleFactor
    
    /*!
        \brief The view's width.
        The default is \c 250.
    */
    width: 250 * scaleFactor

    /*!
        \brief Whether the contentType role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showType: true

    /*!
        \brief Whether the dataFetched role of the Attachment should be displayed.
        The default value is \c false.
    */
    property bool showDataFetched: false

    /*!
        \brief Whether the size role of the Attachment should be displayed.
        The default value is \c true.
    */
    property bool showSize: true

    /*!
        \brief Whether the attachmentUrl role of the Attachment should be displayed.
        The default value is \c false.
    */
    property bool showUrl: false

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
        \brief The height of the row that contains the attachment's information.
        The default value is \c 80 multiplied by the display's scale factor.
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

      \list
        \li \a fileUrl The Url of the attachment passed as a string.
        \li \a contentType the MIME Type of the attachment passed as a string
        \li \a name The name of the attachment passed as a string.
      \endlist

     */
    signal addButtonClicked(string fileUrl, string contentType, string name);

    /*!
      \qmlsignal AttachmentListView::deleteButtonClicked(int attachmentIndex)
      \brief Emitted when the delete button is clicked in the view

      The signal parameter \a attachmentIndex passes the index of attachment to be deleted in the model. It can be deleted using the AttachmentListModel's deleteAttachment function.
     */
    signal deleteButtonClicked(int attachmentIndex);

    /*!
      \qmlsignal AttachmentListView::deleteButtonBlicked(int attachmentIndex)
      \brief Emitted when the thumbnail of an image attachment is clicked.

      The signal parameter \a imageUrl passes a string containing the Url of the attachment.
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
            radius: 2

            Text {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                renderType: Text.NativeRendering
                font.pixelSize: titleBar.height / 3
                text: "Attachments"
                color: titleTextColor
            }

            Image {
                anchors {
                    right: closeButton.left
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                visible: attachmentListView.model !== null
                width: 30 * scaleFactor
                height: width
                source: "images/add.png"

                MouseArea {
                    anchors.fill: parent

                    // open file dialog so file can be selected
                    onClicked: {
                        fileDialog.open();
                    }
                }
            }

            Image {
                id: closeButton
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 5 * scaleFactor
                }
                visible: attachmentListView.model !== null
                source: "images/exit.png"
                width: 30 * scaleFactor
                height: width

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
                    radius: 2

                    // show the attachment name
                    Text {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            margins: 5 * scaleFactor
                        }
                        text: name
                        width: attachmentListView.width - dropDownIndicator.width - 10 * scaleFactor
                        maximumLineCount: 1
                        clip: true
                        elide: Text.ElideRight
                        font.pixelSize: 16 * scaleFactor
                        renderType: Text.NativeRendering
                    }

                    Image {
                        id: dropDownIndicator
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            margins:  5 * scaleFactor
                        }
                        width: 25 * scaleFactor
                        height: width
                        rotation: 180
                        source: "images/triangle.png"

                        Behavior on rotation {
                            SmoothedAnimation {
                                duration: 650
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            dropDownIndicator.rotation += 180;
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
                            damping: 0.4
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
                            id: deleteButton
                            width: 44 * scaleFactor
                            height: width
                            color: pressed ? backgroundColor : "transparent"
                            border.color: "transparent"

                            property bool pressed: false

                            Image {
                                anchors.centerIn: parent
                                height: parent.width * 0.75
                                width: height
                                source: "images/delete.png"
                            }

                            MouseArea {
                                anchors.fill: parent

                                onPressed: deleteButton.pressed = true;
                                onReleased: deleteButton.pressed = false;

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

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            // pass image url when image is clicked
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

    // when valid model is set, make view visible
    onModelChanged: {
        if (model !== null)
            attachmentListView.visible = true;
    }
}
