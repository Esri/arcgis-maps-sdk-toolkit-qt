import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {

    property AuthenticationController controller: AuthenticationController {}

    title: "Untrusted Host"

    closePolicy: Popup.NoAutoClose

    header: Label {
        text: `<h1>${title}</h1>`
        horizontalAlignment: Text.AlignHCenter
    }

    footer: DialogButtonBox {
        Button {
            text: qsTr("Block")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
        Button {
            text: qsTr("Trust")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

        onAccepted: controller.continueWithSslHandshake(
            true, rememberCheckbox.checked);

        onRejected: controller.continueWithSslHandshake(
            false, rememberCheckbox.checked);
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
        CheckBox {
                id: rememberCheckbox
                text: qsTr("Remember")
                checked: true
                Layout.alignment: Qt.AlignHCenter
                Layout.margins: 5
            }
    }
}
