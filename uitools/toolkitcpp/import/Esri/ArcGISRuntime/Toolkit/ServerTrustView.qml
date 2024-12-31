/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
import QtQuick.Layouts

Dialog {

    property ArcGISAuthenticationController controller: null

    title: qsTr("Untrusted Host")

    closePolicy: Popup.NoAutoClose

    header: Label {
        text: `<h1>${title}</h1>`
        horizontalAlignment: Text.AlignHCenter
    }

    footer: DialogButtonBox {
        Button {
            id: blockButton
            text: qsTr("Block")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
        Button {
            id: trustButton
            text: qsTr("Trust")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

        onRejected: controller.continueWithServerTrust(false);
        onAccepted: controller.continueWithServerTrust(true);
    }

    ColumnLayout {
        anchors.centerIn: parent
        Label {
            text: controller.currentAuthenticatingHost
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.margins: 5
            font.weight: Font.Bold
        }
        Label {
            text: qsTr("The server could not prove itself; its security "
                     + "certificate is not trusted by your OS. Would you "
                     + "like to continue anyway?")
            horizontalAlignment: Qt.AlignHCenter
            wrapMode: Text.Wrap
            Layout.preferredWidth: 400
            Layout.fillWidth: true
            Layout.margins: 5
        }
    }

    /*!
      \internal
      \brief automated testing helper
     */
    function block() {
        blockButton.clicked();
    }

    /*!
      \internal
      \brief automated testing helper
     */
    function trust() {
        trustButton.clicked();
    }
}
