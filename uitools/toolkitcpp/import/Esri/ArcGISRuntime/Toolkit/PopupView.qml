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

import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window
import QtQuick.Layouts


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
   \image docs/popupview.png popupview
   Example code in the QML API (C++ API might differ):
   \snippet qml_quick/src/demos/PopupViewDemoForm.qml Set up Popup View
 */
Page {
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
      \brief The Controller handles reading from the PopupManager and monitoring
      the list-models.

      The QML controller is documented \l{PopupViewController}{here} and the CPP controller is documented \l{Esri::ArcGISRuntime::Toolkit::PopupViewController}{here}.
    */
    property var controller: PopupViewController {}


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

    spacing: 5
    leftPadding: popupView.spacing
    rightPadding: popupView.spacing

    title: controller.title

    // Title Header
    header: Label {
        textFormat: Text.StyledText
        text: `<h2>${popupView.title}</h2>`
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        font: popupView.font
        leftPadding: popupView.spacing
        rightPadding: popupView.spacing
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
            rows: controller.showAttachments ? controller.fieldCount + controller.attachmentCount + 1
                                             : controller.fieldCount
            rowSpacing: popupView.spacing
            columnSpacing: 30
            // Field names
            Repeater {
                model: controller.displayFields
                Label {
                    text: label ?? fieldName ?? ""
                    Layout.maximumWidth: flickable.width / 2
                    wrapMode: Text.Wrap
                    font: popupView.font
                }
            }

            // Attachments header
            Label {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                visible: controller.showAttachments
                enabled: visible
                textFormat: Text.StyledText
                horizontalAlignment: Text.AlignHCenter
                text: controller.attachmentCount > 0 ? "<h2>Attachments</h2>" : ""
                font: popupView.font
            }

            // Attachment names
            Repeater {
                model: controller.attachments
                Label {
                    Layout.fillWidth: true
                    visible: controller.showAttachments
                    enabled: visible
                    text: name
                    wrapMode: Text.Wrap
                    font: popupView.font
                }
            }

            // Field contents
            Repeater {
                model: controller.displayFields
                Label {
                    Layout.fillWidth: true
                    text: formattedValue
                    wrapMode: Text.Wrap
                    font: popupView.font
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

    footer: ColumnLayout {
        visible: popupView.closeCallback
        Button {
            text: "Close"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                if (popupView.closeCallback)
                    popupView.closeCallback()
            }
            Layout.bottomMargin: popupView.spacing
        }
    }
}
