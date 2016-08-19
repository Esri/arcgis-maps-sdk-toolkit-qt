import QtQuick 2.5
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import Esri.ArcGISExtras 1.1

Rectangle {
    id: root
    color: "transparent"

    property real scaleFactor: System.displayScaleFactor
    property string requestingHost: authView.authChallenge.authenticatingHost
    property string detailText: qsTr("The server could not prove itself; its security certificate is not trusted by your OS. Would you like to continue anyway?")

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

        Column {
            anchors {
                fill: parent
                margins: 5 * scaleFactor
            }

            spacing: 2 * scaleFactor

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Untrusted Host")
                font {
                    pixelSize: 18 * scaleFactor
                    family: "sanserif"
                }
                renderType: Text.NativeRendering
                color: "white"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                elide: Text.ElideRight
                text: requestingHost
                font {
                    pixelSize: 12 * scaleFactor
                    family: "sanserif"
                }
                color: "white"
            }
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
        spacing: 8 * scaleFactor

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: detailText
            width: parent.width
            wrapMode: Text.Wrap
            font {
                pixelSize: 10 * scaleFactor
                family: "sanserif"
            }
        }

        CheckBox {
            id: rememberCheckbox
            text: qsTr("Remember")
        }

        Row {
            width: parent.width
            spacing: 4 * scaleFactor

            Button {
                width: ((parent.width / 2) - 2 * scaleFactor)
                text: qsTr("Block")
                onClicked: {
                    // reject the challenge and let the resource fail to load
                    authView.authChallenge.continueWithSslHandshake(false, rememberCheckbox.checked);
                    root.visible = false;
                }
            }

            Button {
                width: ((parent.width / 2) - 2 * scaleFactor)
                text: qsTr("Trust")
                onClicked: {
                    // continue SSL handshake and trust host
                    authView.authChallenge.continueWithSslHandshake(true, rememberCheckbox.checked);
                    root.visible = false;
                }
            }
        }
    }
}
