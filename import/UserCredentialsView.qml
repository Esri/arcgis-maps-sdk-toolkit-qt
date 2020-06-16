import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: userCredentialsView

    property AuthenticationController controller: AuthenticationController {}

    title: qsTr("Authentication required")

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
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

        onAccepted: userCredentialsView.accept();

        onRejected: userCredentialsView.reject();
    }

    height: contentHeight + implicitFooterHeight + implicitHeaderHeight
    width: contentWidth

    ColumnLayout {
        Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            visible: controller.currentChallengeFailureCount > 1
            text: qsTr("Invalid username or password.")
            color: palette.toolTipBase
            background:  Rectangle {
                color: palette.toolTipText
                radius: 5
            }
        }

        Label {
            text: qsTr("You need to sign in to access the resource at:")
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }

        Label {
            text: controller.currentAuthenticatingHost
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }

        TextField {
            id: usernameTextField
            Layout.fillWidth: true
            placeholderText: qsTr("username")
            onAccepted: userCredentialsView.accept();
        }

        TextField {
            id: passwordTextField
            Layout.fillWidth: true
            placeholderText: qsTr("password")
            onAccepted: userCredentialsView.accept();
            echoMode: TextInput.Password
        }
    }

    function accept() {
        controller.continueWithUsernamePassword(usernameTextField.text,
                                                passwordTextField.text);
    }

    function reject() {
        controller.cancel();
    }
}
