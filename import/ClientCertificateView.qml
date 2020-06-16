import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1 as P

Page {
    id: clientCertificateView
    property AuthenticationController controller: AuthenticationController {}

    title: qsTr("Client certificate requested")

    header: Label {
        text: title
        horizontalAlignment: Text.AlignHCenter
        color: palette.windowText
        background: Rectangle {
            color: palette.window
        }
    }

    footer: DialogButtonBox {
        Button {
            id: bBox
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

    height: contentHeight + implicitFooterHeight + implicitHeaderHeight
    width: contentWidth

    ColumnLayout {
        Label {
            text:qsTr("The service has requested a client certificate to authenticate you. "
                    + "The app has identified the requesting server as "
                    + `'${controller.currentAuthenticatingHost}', but you `
                    + "should only give the app access to the certificate if you trust it.")
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            Layout.preferredWidth: 400
        }

        ListView {
            id: certificateView
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 200
            model: controller.clientCertificateInfos
            header: Label {
                text: qsTr("Available certificates")
                horizontalAlignment: Qt.AlignHCenter
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            delegate: Label {
                id: cCI
                text: modelData
                clip: true
                elide: Text.ElideRight
                color: ListView.isCurrentItem ? palette.highlightedText 
                                              : palette.base
                background: Rectangle {
                    color: cCI.ListView.isCurrentItem ? palette.highlight
                                                      : palette.text
                }
            }
        }

        Button {
            text: qsTr("Open")
            Layout.alignment: Qt.AlignRight
            onClicked: fileDialog.open()
            P.FileDialog {
                id: fileDialog
                folder: P.StandardPaths.writableLocation(P.StandardPaths.DocumentsLocation)
                onFileChanged: {
                    controller.addClientCertificate(file);
                }
            }
        }
    }

    Connections {
        target: controller

        onClientCertificatePasswordRequired: {
            const dialog = passwordDialog.createObject(
                clientCertificateView, { certificate: certificate});
            dialog.open();
            dialog.closed.connect(() => dialog.destroy());
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
        }
    }
}
