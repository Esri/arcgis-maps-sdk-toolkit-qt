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
import Qt.labs.platform as P

Dialog {
    id: clientCertificateView
    property ArcGISAuthenticationController controller: null
    property url currentCertificate: ""

    title: qsTr("Client certificate requested")

    closePolicy: Popup.NoAutoClose

    header: Label {
        id: headerLabel
        text: `<h1>${title}</h1>`
        horizontalAlignment: Text.AlignHCenter
    }

    footer: DialogButtonBox {
        Button {
            id: mainDialogCancelButton
            text: qsTr("Cancel")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }

        onRejected: {
            controller.cancel();
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        Label {
            text: qsTr("The service has requested a client certificate to authenticate you. "
                      + "The app has identified the requesting server as "
                      + `'${controller.currentAuthenticatingHost}', but you `
                      + "should only give the app access to the certificate if you trust it.")
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            Layout.preferredWidth: 400
            Layout.margins: 5
        }

        Button {
            text: qsTr("Choose certificate")
            Layout.fillWidth: true
            onClicked: fileDialog.open()
        }

        P.FileDialog {
            id: fileDialog
            folder: P.StandardPaths.writableLocation(P.StandardPaths.DocumentsLocation)
            onAccepted: {
                currentCertificate = file;
                // first attempt with no password in case none is required
                passwordDialog.firstAttemptWithNoPassword = true;
                passwordTextField.text = "";
                processCertificate_();
            }
        }

        Dialog {
            id: passwordDialog
            anchors.centerIn: parent
            property bool firstAttemptWithNoPassword: true
            header: Label {
                text: passwordDialog.firstAttemptWithNoPassword ? qsTr("Enter password") : qsTr("Invalid password - try again")
                color: passwordDialog.firstAttemptWithNoPassword ? headerLabel.color : "red"
                horizontalAlignment: Text.AlignLeft
                padding: parent.padding
            }
            property string fileName: currentCertificate.toString().split("/").pop();
            modal: true
            closePolicy: Popup.NoAutoClose

            footer: DialogButtonBox {
                id: buttonBox
                Button {
                    text: qsTr("Cancel")
                    onClicked: passwordDialog.reject()
                }
                Button {
                    id: passwordDialogOkButton
                    text: qsTr("Ok")
                    enabled: passwordTextField.text.length > 0
                    onClicked: passwordDialog.accept()
                }
            }

            onAccepted: {
                // Disallow empty password
                // We already attempt that when the file is selected.
                if (passwordTextField.text.length === 0) {
                    openPasswordInputDialog_();
                    return;
                }
                processCertificate_();
            }
            onRejected: {
                passwordDialog.close();
            }

            ColumnLayout {
                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: clientCertificateView.width
                    text: qsTr("The client certificate file "
                               + `'${passwordDialog.fileName}' `
                               + "requires a password to open.")
                    wrapMode: Text.Wrap
                }
                TextField {
                    id: passwordTextField
                    Layout.fillWidth: true
                    placeholderText: qsTr("password")
                    onAccepted: passwordDialog.accept();
                    echoMode: TextInput.Password
                }
            }
        }
    }

    function processCertificate_() {
        if (passwordDialog.firstAttemptWithNoPassword && passwordTextField.text.length > 0) {
            passwordDialog.firstAttemptWithNoPassword = false;
        }

        switch(controller.respondWithClientCertificate(currentCertificate, passwordTextField.text)) {
            case ArcGISAuthenticationController.CertificateResult.Accepted:
            case ArcGISAuthenticationController.CertificateResult.Error: {
                passwordDialog.close();
                clientCertificateView.close();
                break;
            }
            case ArcGISAuthenticationController.CertificateResult.PasswordRejected: {
                passwordTextField.text = "";
                openPasswordInputDialog_();
                break;
            }
        }
    }

    function openPasswordInputDialog_() {
        if (!passwordDialog.opened) {
            passwordDialog.open();
        } else {
            const openDialogFn = () => {
                passwordDialog.open();
                passwordDialog.closed.disconnect(openDialogFn);
            };
            passwordDialog.closed.connect(openDialogFn);
        }
    }

    // the functions below are automated test helpers
    function applyClientCertificate(path, password) {
        const inputPasswordFn = () => {
            if (!passwordDialog.opened)
                return;

            passwordDialog.openedChanged.disconnect(inputPasswordFn);
            passwordTextField.text = password;
            passwordDialogOkButton.click();
        }

        passwordDialog.openedChanged.connect(inputPasswordFn);

        fileDialog.file = path;

        // this will open the password dialog, which we are listening for it to become
        // opened above to input the password
        fileDialog.accept();
    }

    function clickCancel() {
        mainDialogCancelButton.click();
    }
}
