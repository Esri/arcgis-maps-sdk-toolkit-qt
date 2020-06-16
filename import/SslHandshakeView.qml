import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {

    property AuthenticationController controller: AuthenticationController {}

    title: controller.currentAuthenticatingHost

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
            text: qsTr("Block")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
        Button {
            text: qsTr("Trust")
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }

        onAccepted: controller.continueWithSslHandshake(true, rememberCheckbox.checked);

        onRejected: controller.continueWithSslHandshake(false, rememberCheckbox.checked);
    }

    height: contentHeight + implicitFooterHeight + implicitHeaderHeight
    width: contentWidth

    ColumnLayout {
        Label {
                text: qsTr("The server could not prove itself; its security certificate "
                         + "is not trusted by your OS. Would you like to continue anyway?")
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
            }
        CheckBox {
                id: rememberCheckbox
                text: qsTr("Remember")
                Layout.fillWidth: true
                checked: true
            }
    }
}
