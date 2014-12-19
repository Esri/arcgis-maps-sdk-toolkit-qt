/*******************************************************************************
 * Copyright 2012-2015 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

Dialog {    
    id: dialog
    modality: Qt.ApplicationModal
    title: "User Credentials"
    height: rootRectangle.implicitHeight
    width: rootRectangle.implicitWidth

    property bool busy: false
    property alias bannerTitle: titleText.text
    property alias bannerImage: bannerImage.source
    property alias usernameLabel: usernameText.text
    property alias username: usernameField.text
    property alias passwordLabel: passwordText.text
    property alias password: passwordField.text
    property string message : ""
    property alias dialogTitle: dialog.title

    contentItem: Rectangle {
        id: rootRectangle
        enabled: !busy

        Component.onCompleted: {
            // workaround for Qt bug - Desktop platforms need anchors.fill set, and Mobile platforms cannot have it set
            if (Qt.platform.os !== "ios" || Qt.platform.os != "android") {
                anchors.fill = parent;
            }
        }
        property real displayScaleFactor: ((Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72))
        implicitHeight: Math.min(265 * displayScaleFactor, Screen.desktopAvailableHeight * .95)
        implicitWidth: Math.min(325 * displayScaleFactor, Screen.desktopAvailableWidth * .95)

        Keys.onReturnPressed: {
            if (username.length > 0 && password.length > 0)
                signIn();
        }

        Image {
            id: bannerImage
            height: 40 * rootRectangle.displayScaleFactor
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            fillMode: Image.PreserveAspectCrop
            source: "images/userCredentialsBanner.png"
        }

        Text {
            id: titleText
            text: qsTr("Sign In")
            color: "white"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            font {
                pixelSize: 30 * rootRectangle.displayScaleFactor
                bold: true
            }
            anchors {
                top: parent.top
                left: parent.left
                leftMargin: 10 * rootRectangle.displayScaleFactor
                right: parent.right
                bottom: bannerImage.bottom
            }
        }
        Column {
            id: baseColumn
            anchors {
                left: parent.left
                right: parent.right
                top: bannerImage.bottom
                margins: 10 * rootRectangle.displayScaleFactor
            }
            spacing: 25 * rootRectangle.displayScaleFactor

            Column {
                spacing: 10 * rootRectangle.displayScaleFactor
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Text {
                    id: usernameText
                    text: qsTr("Username")
                    horizontalAlignment: Text.AlignLeft
                    font {
                        pixelSize: 16 * rootRectangle.displayScaleFactor
                        bold: true
                    }
                }

                TextField {
                    id: usernameField
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: usernameText.paintedHeight * 1.5
                    width: rootRectangle.width * .95
                    placeholderText: "Username"
                    font.pixelSize: 16 * rootRectangle.displayScaleFactor
                    style: TextFieldStyle {
                        renderType: Text.QtRendering
                    }
                    activeFocusOnTab: true
                    focus: true
                    inputMethodHints: Qt.ImhNoAutoUppercase + Qt.ImhNoPredictiveText + Qt.ImhSensitiveData
                }

                Text {
                    id: passwordText
                    text: qsTr("Password")
                    horizontalAlignment: Text.AlignLeft
                    font: usernameText.font
                }

                TextField {
                    id: passwordField
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: usernameField.height
                    width: rootRectangle.width * .95
                    echoMode: TextInput.Password
                    placeholderText: "Password"
                    font: usernameField.font
                    style: usernameField.style
                    activeFocusOnTab: true
                }
            }

            Column {
                spacing: 20 * rootRectangle.displayScaleFactor
                anchors.horizontalCenter: parent.horizontalCenter

                Row {
                    spacing: 10 * rootRectangle.displayScaleFactor
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: signInButton
                        text: "SIGN IN"
                        style: ButtonStyle {
                            background: Rectangle {
                                id: styleRectSignIn
                                implicitWidth: 100 * rootRectangle.displayScaleFactor
                                implicitHeight: 25 * rootRectangle.displayScaleFactor
                                border {
                                    width: control.activeFocus ? 2 * rootRectangle.displayScaleFactor : 1 * rootRectangle.displayScaleFactor
                                    color: "#888"
                                }
                                radius: 4
                                color: control.pressed ? "#E68A2E" : "#FFA319"
                            }
                            label: Text {
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: "white"
                                font {
                                    bold: true
                                    pointSize: 14
                                }
                                text: signInButton.text
                                renderType: Text.QtRendering
                            }
                        }

                        onClicked: {
                            signIn();
                        }
                    }

                    Button {
                        id: cancelButton
                        text: "CANCEL"
                        style: ButtonStyle {
                            background: Rectangle {
                                id: styleRectCancel
                                implicitWidth: 100 * rootRectangle.displayScaleFactor
                                implicitHeight: 25 * rootRectangle.displayScaleFactor
                                border {
                                    width: control.activeFocus ? 2 * rootRectangle.displayScaleFactor : 1 * rootRectangle.displayScaleFactor
                                    color: "#888"
                                }
                                radius: 4
                                gradient: Gradient {
                                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                                }
                            }
                            label: Text {
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: "black"
                                text: cancelButton.text
                                font.pointSize: 14
                                renderType: Text.QtRendering
                            }
                        }

                        onClicked: {
                            cancel();
                        }
                    }
                }
            }
        }

        Rectangle {
            id: busyScreen
            anchors.fill: parent
            color: "darkgrey"
            visible: busy
            opacity: 0.4
        }

        BusyIndicator {
            id: busyIndicator
            running: busy
            anchors.centerIn: parent
        }
    }

    function signIn() {
        accepted();
        console.log("signin")
    }

    function cancel() {
        rejected();
        close();
    }
}
