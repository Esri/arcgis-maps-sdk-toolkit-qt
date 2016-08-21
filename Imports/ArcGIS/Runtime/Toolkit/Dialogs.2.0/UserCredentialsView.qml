import QtQuick 2.5
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import Esri.ArcGISExtras 1.1

Rectangle {
    id: root
    color: "transparent"

    property real scaleFactor: System.displayScaleFactor
    property var challenge: authView.authChallenge
    property string requestingHost: challenge ? challenge.authenticatingHost : ""
    property string detailText: qsTr("You need to sign in to access the resource at '%1'").arg(requestingHost)

    Keys.onEnterPressed: {
        if (Qt.platform.os !== "android" && Qt.platform.os !== "ios") {
            continueButton.clicked();
        }
    }

    Keys.onReturnPressed: {
        if (Qt.platform.os !== "android" && Qt.platform.os !== "ios") {
            continueButton.clicked();
        }
    }

    RadialGradient {
        anchors.fill: parent
        opacity: 0.7
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightgrey" }
            GradientStop { position: 0.5; color: "black" }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
        onWheel: wheel.accepted = true
    }

    Rectangle {
        anchors {
            fill: banner
            margins: -1 * scaleFactor
        }
        color: "white"
        border {
            color: "black"
            width: 1 * scaleFactor
        }
        radius: 3
        smooth: true
        clip: true
        antialiasing: true
    }

    Image {
        id: banner
        anchors {
            centerIn: parent
            verticalCenterOffset: -50 * scaleFactor
        }
        width: 224 * scaleFactor
        height: 50 * scaleFactor
        clip: true
        source: "images/banner.png"

        Text {
            anchors.centerIn: parent
            text: qsTr("Authentication Required")
            font {
                pixelSize: 18 * scaleFactor
                family: "sanserif"
            }
            color: "white"
            renderType: Text.NativeRendering
        }
    }

    Rectangle {
        anchors {
            fill: controlsColumn
            margins: -5 * scaleFactor
        }
        color: "white"
        border {
            color: "black"
            width: 1 * scaleFactor
        }
        radius: 3
        smooth: true
        clip: true
        antialiasing: true
    }

    Column {
        id: controlsColumn
        anchors {
            top: banner.bottom
            topMargin: 5 * scaleFactor
            horizontalCenter: banner.horizontalCenter
        }
        width: 215 * scaleFactor
        spacing: 10 * scaleFactor

        Rectangle {
            color: "#FFCCCC"
            radius: 5
            width: parent.width
            anchors.margins: 10 * scaleFactor
            height: 20 * scaleFactor
            visible: challenge ? challenge.failureCount > 1 : false

            Text {
                anchors.centerIn: parent
                text: qsTr("Invalid username or password.")
                font {
                    pixelSize: 12 * scaleFactor
                    family: "sanserif"
                }
                color: "red"
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: detailText
            width: parent.width
            wrapMode: Text.Wrap
            font {
                pixelSize: 12 * scaleFactor
                family: "sanserif"
            }
            renderType: Text.NativeRendering
        }

        TextField {
            id: usernameTextField
            width: parent.width
            placeholderText: qsTr("username")
        }

        TextField {
            id: passwordTextField
            width: parent.width
            placeholderText: qsTr("password")
            echoMode: TextInput.Password
        }

        Row {
            width: parent.width
            spacing: 4 * scaleFactor
            Button {
                width: ((parent.width / 2) - 2 * scaleFactor)
                text: qsTr("Skip")
                onClicked: {
                    // cancel the challenge and let the resource fail to load
                    if (challenge)
                        challenge.cancel();
                    root.visible = false;
                }
            }

            Button {
                id: continueButton
                width: ((parent.width / 2) - 2 * scaleFactor)
                text: qsTr("Continue")
                isDefault: true
                onClicked: {
                    // continue with the username and password
                    if (challenge)
                        challenge.continueWithUsernamePassword(usernameTextField.text, passwordTextField.text);
                    root.visible = false;
                }
            }
        }
    }
}

