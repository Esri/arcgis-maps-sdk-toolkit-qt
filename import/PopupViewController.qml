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

import QtQml 2.12

QtObject {
    id: controller

    property var popupManager: null

    readonly property alias displayFields: internal.displayFields

    readonly property alias fieldCount: internal.fieldCount

    readonly property alias attachments: internal.attachments

    readonly property alias attachmentCount: internal.attachmentCount

    property QtObject internal : QtObject {
      id: internal

      property var displayFields: null

      property int fieldCount: 0

      property var attachments: null

      property int attachmentCount: 0

      property Connections attachmentConnection: Connections {
          target: attachments
          onRowsInserted: internal.attachmentCount = attachments.rowCount();
          onRowsRemoved: internal.attachmentCount = attachments.rowCount();
      }

      property Connections fieldsConnection: Connections {
          target: displayFields
          onRowsInserted: internal.fieldCount = displayFields.rowCount();
          onRowsRemoved: internal.fieldCount = displayFields.rowCount();
      }
    }

    onPopupManagerChanged: {
        if (popupManager) {
            if (popupManager.attachmentManager) {
                internal.attachments = popupManager.attachmentManager.attachmentsModel;
                internal.attachmentCount = attachments.rowCount();
            } else {
                internal.attachments = null;
                internal.attachmentCount = 0;
            }

            internal.displayFields = popupManager.displayedFields;
            internal.fieldCount = displayFields.rowCount();

        } else {
            internal.attachments = null;
            internal.attachmentCount = 0;

            internal.displayFields = null;
            internal.fieldCount = 0;
        }
    }
}
