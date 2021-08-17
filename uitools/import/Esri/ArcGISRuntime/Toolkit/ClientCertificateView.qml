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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1 as P

Dialog {
    id: clientCertificateView
    property AuthenticationController controller: AuthenticationController {}
    
    title: qsTr("Client certificate requested")

    closePolicy: Popup.NoAutoClose

    header: Label {
        text: `<h1>${title}</h1>`
        horizontalAlignment: Text.AlignHCenter
    }

    footer: DialogButtonBox {
        Button {
            text: qsTr("Skip")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }

        Button {
            text: qsTr("Continue")
            enabled: certificateView.currentIndex >= 0
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

        onAccepted: {
            controller.continueWithClientCertificate(certificateView.currentIndex);
        }

        onRejected: {
            controller.cancel();
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        Label {
            text:qsTr("The service has requested a client certificate to authenticate you. "
                      + "The app has identified the requesting server as "
                      + `'${controller.currentAuthenticatingHost}', but you `
                      + "should only give the app access to the certificate if you trust it.")
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            Layout.preferredWidth: 400
            Layout.margins: 5
        }

        ListView {
            id: certificateView
            clip: true
            model: controller.clientCertificateInfos

            header: Label {
                text: `<h2>${qsTr("Available certificates")}</h2>`
                horizontalAlignment: Qt.AlignHCenter
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            footer: Button {
                text: qsTr("Add certificate")
                anchors {
                    left: parent.left
                    right: parent.right
                }
                onClicked: fileDialog.open()
                P.FileDialog {
                    id: fileDialog
                    folder: P.StandardPaths.writableLocation(P.StandardPaths.DocumentsLocation)
                    onFileChanged: {
                        controller.addClientCertificate(file);
                    }
                }
            }

            delegate: ItemDelegate {
                text: modelData
                anchors {
                    left: parent.left
                    right: parent.right
                }
                clip: true
                highlighted: ListView.isCurrentItem
            }

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 200
            Layout.margins: 5
        }
    }


    Connections {
        target: controller

        function onClientCertificatePasswordRequired(certificate) {
            const dialog = passwordDialog.createObject(
                             clientCertificateView.parent, { certificate: certificate});
            dialog.open();
        }
    }

    Component {
        id: passwordDialog
        Dialog {
            id: pD
            anchors.centerIn: parent
            title: qsTr("Enter password")
            property url certificate: ""
            property string fileName: certificate.toString().split("/").pop();
            modal: true
            closePolicy: Popup.NoAutoClose
            standardButtons: Dialog.Ok | Dialog.Cancel
            onAccepted: {
                controller.addClientCertificate(
                            certificate, passwordTextField.text);
            }
            ColumnLayout {
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: clientCertificateView.width
                    text: qsTr("The client certificate file "
                               + `'${pD.fileName}' `
                               + "requires a password to open.")
                    wrapMode: Text.Wrap
                }
                TextField {
                    id: passwordTextField
                    Layout.fillWidth: true
                    placeholderText: qsTr("password")
                    onAccepted: pD.accept();
                    echoMode: TextInput.Password
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }
}
