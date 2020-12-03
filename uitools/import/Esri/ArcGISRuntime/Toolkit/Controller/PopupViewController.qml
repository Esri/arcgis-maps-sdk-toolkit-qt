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

import QtQml 2.12

/*!
   \qmltype PopupViewController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.10
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief In MVC architecture, this is the controller for the corresponding
   PopupView.
   
   This controller is a thin wrapper around a PopupManager. It re-exposes some
   PopupManager properties, including the number of total rows to render as a 
   property.
 */
QtObject {
    id: controller

    /*!
      \brief The PopupManager that populates this controller with data.
     */
    property var popupManager: null

    /*!
      \brief A list model containing the key/value fields of the
      Popup associated with this PopupManager.
      \note This can be null.
     */
    readonly property alias displayFields: internal.displayFields

    /*!
      Exposes the number of rows in the list model returned by `displayFields`.
     */
    readonly property alias fieldCount: internal.fieldCount

    /*!
      \brief A list model containing the attachment images of the
      Popup associated with this PopupManager. 
      \note This can be null.
     */
    readonly property alias attachments: internal.attachments

    /*!
      \brief Exposes the number of rows in the list model returned by 
      `attachments`.
     */
    readonly property alias attachmentCount: internal.attachmentCount

    /*!
      \brief The mimimum attachment thumbnail width.
     */
    property alias attachmentThumbnailWidth: internal.attachmentThumbnailWidth

    /*!
      \brief The mimimum attachment thumbnail height.
     */
    property alias attachmentThumbnailHeight: internal.attachmentThumbnailHeight

    /*!
      \brief Returns the title of the PopupManager.
      \note This is re-exposed from PopupManager as PopupManager does not have
      NOTIFY/CONSTANT modifiers on its title property, so the Controller
      re-exposes title to suppress warnings about ths.
     */
    readonly property alias title: internal.title

    /*!
      \brief Returns the showAttachments flag of the PopupManager.
      \note This is re-exposed from PopupManager as PopupManager does not have
      NOTIFY/CONSTANT modifiers on its showAttachments property, so the Controller
      re-exposes showAttachments to suppress warnings about ths.
     */
    readonly property alias showAttachments: internal.showAttachments

    /*! \internal */
    property QtObject internal : QtObject {
      id: internal

      property var displayFields: null

      property int fieldCount: 0

      property var attachments: null

      property int attachmentCount: 0

      property string title: ""

      property bool showAttachments: false

      property int attachmentThumbnailWidth: 0

      property int attachmentThumbnailHeight: 0

      property Connections attachmentConnection: Connections {
          target: attachments
          function onRowsInserted() {
              internal.attachmentCount = attachments.rowCount();
          }
          function onRowsRemoved() {
              internal.attachmentCount = attachments.rowCount();
          }
      }

      property Connections fieldsConnection: Connections {
          target: displayFields
          function onRowsInserted() {
              internal.fieldCount = displayFields.rowCount();
          }
          function onRowsRemoved() {
              internal.fieldCount = displayFields.rowCount();
          }
      }
    }

    onPopupManagerChanged: {
        // PopupManager properties have no NOTIFY or CONST properties.
        // To avoid property warnings we will instead cache 
        // what comes out of PopupManager.
        if (popupManager) {
            if (popupManager.attachmentManager) {
                internal.attachments = popupManager.attachmentManager.attachmentsModel;
                internal.attachmentCount = attachments.rowCount();
                internal.attachmentThumbnailWidth = Qt.binding(() => internal.attachments.thumbnailWidth);
                internal.attachmentThumbnailHeight = Qt.binding(() => internal.attachments.thumbnailHeight);
            } else {
                internal.attachments = null;
                internal.attachmentCount = 0;
                internal.attachmentThumbnailWidth = 0;
                internal.attachmentThumbnailHeight = 0;
            }

            internal.displayFields = popupManager.displayedFields;
            internal.fieldCount = displayFields.rowCount();
            internal.title = popupManager.title;
            internal.showAttachments = popupManager.showAttachments;
        } else {
            internal.attachments = null;
            internal.attachmentCount = 0;
            internal.displayFields = null;
            internal.fieldCount = 0;
            internal.attachmentThumbnailWidth = 0;
            internal.attachmentThumbnailHeight = 0;
            internal.title = "";
            internal.showAttachments = false;
        }
    }
}
