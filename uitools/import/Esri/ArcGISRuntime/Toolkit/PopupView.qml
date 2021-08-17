/*******************************************************************************
 *  Copyright 2012-2020 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.11
import QtQuick.Controls 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3

/*!
   \qmltype PopupView
   \ingroup ArcGISQtToolkit
   \ingroup ArcGISQtToolkitUiQmlViews
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.10
   \brief A view for displaying and editing information about a feature.
  
   A PopupView can be used to display information for any type that
   implements the PopupSource interface. For example, FeatureLayer
   implements PopupSource. This means that it has a PopupDefinition,
   which defines how the Popup should look for any features in that
   layer.
   An example workflow for displaying a PopupView for a feature in a
   FeatureLayer would be:
   \list
     \li Declare a PopupView and anchor it to a desired location.
     \li Perform an identify operation on a GeoView and select a
     Feature from the identify result.
     \li Create a Popup from the Feature.
     \li Optionally obtain the Popup's PopupDefinition and set the
     title, whether to show attachments, and so on.
     \li Create a PopupManager from the Popup.
     \li Assign the PopupView's \c popupManager property the PopupManager
     created in the previous step.
   \endlist
   The PopupView is a QML Item that can be anchored, given to a dialog,
   or positioned using XY screen coordinates. Transform, Transition, and
   other QML animation types can be used to animate the showing and
   dissmisal of the view.
   For more information, please see the Popup and PopupManager
   documentation.
   \note Each time a change is made to the Popup, PopupDefinition,
   PopupManager, or any of their properties, the PopupManager must
   be re-set to the PopupView.
 */
Control {
    id: popupView

    /*!
       \brief The PopupManager that controls the information being displayed in
       the view.
       
       The PopupManager should be created from a Popup.
       \qmlproperty PopupManager popupManager
     */
    property var popupManager: null
    
    /*!
      \qmlproperty PopupViewController controller
      \brief the Controller handles reading from the PopupManager and monitoring
      the list-models.
      \qmlproperty PopupViewController controller
    */
    property var controller: PopupViewController { }

    /*!
       \brief Callback function called when the close button is clicked. When
       this property is set to null the close button does not render. When
       the close button is clicked the function in this property is called.
       Defaults to setting visible to false.
     */
    property var closeCallback: function() {
        popupView.visible = false;
    }

    /*!
       \qmlsignal PopupView::attachmentThumbnailClicked(var index)
       \brief Signal emitted when an attachment thumbnail is clicked.
       The \a index of the PopupAttachment in the PopupAttachmentListModel
       that was clicked on by the user.
     */
    signal attachmentThumbnailClicked(var index)

    Binding {
        target: controller
        property: "popupManager"
        value: popupView.popupManager
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

            // We must account for what is visible, including title headers as rows.
            rows: controller.showAttachments ? controller.fieldCount + controller.attachmentCount + 2
                                             : controller.fieldCount + 1

            // Title Header
            Text {
                Layout.fillWidth: true
                textFormat: Text.StyledText
                text: `<h2>${controller.title}</h2>`
                color: palette.text
                font: popupView.font
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
                visible: controller.showAttachments
                enabled: visible
                textFormat: Text.StyledText
                text: controller.attachmentCount > 0 ? "<h2>Attachments</h2>" : ""
                color: palette.text
                font: popupView.font
            }

            // Attachment names
            Repeater {
                model: controller.attachments
                Text {
                    visible: controller.showAttachments
                    enabled: visible
                    Layout.fillWidth: true
                    text: name
                    wrapMode: Text.WrapAnywhere
                    color: palette.text
                }
            }

            Button {
                text: "Close"
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    if (popupView.closeCallback)
                        popupView.closeCallback();
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
                    Layout.fillHeight: true
                    Layout.minimumWidth: controller.attachmentThumbnailWidth
                    Layout.minimumHeight: controller.attachmentThumbnailHeight
                    visible: controller.showAttachments
                    enabled: visible
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

