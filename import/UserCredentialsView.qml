import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    id: userCredentialsView

    property AuthenticationController controller: AuthenticationController {}

    title: qsTr("Authentication required")

    header: Label {
        text: `<h1>${title}</h1>`
        horizontalAlignment: Text.AlignHCenter
        padding: 5
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

    ColumnLayout {
        anchors.centerIn: parent
        Label {
            text: qsTr("Invalid username or password.")
            horizontalAlignment: Qt.AlignHCenter
            visible: controller.currentChallengeFailureCount > 1
            color: "red"
            background:  Rectangle {
                color: "#FFCCCC"
                radius: 5
            }
            Layout.fillWidth: true
            Layout.margins: 5
        }

        Label {
            text: controller.currentAuthenticatingHost
            horizontalAlignment: Qt.AlignHCenter
            font.weight: Font.Bold
            Layout.fillWidth: true
            Layout.margins: 5
        }

        Label {
            text: "You need to sign in to access this resource."
            horizontalAlignment: Qt.AlignHCenter
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            Layout.margins: 5
        }

        TextField {
            id: usernameTextField
            placeholderText: qsTr("username")
            onAccepted: userCredentialsView.accept();
            Layout.fillWidth: true
            Layout.margins: 5
        }

        TextField {
            id: passwordTextField
            placeholderText: qsTr("password")
            onAccepted: userCredentialsView.accept();
            echoMode: TextInput.Password
            Layout.fillWidth: true
            Layout.margins: 5
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
