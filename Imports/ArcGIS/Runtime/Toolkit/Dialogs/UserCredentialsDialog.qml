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

    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property bool busy: false

    modality: Qt.WindowModal

    contentItem: Rectangle {
        id: rootRectangle
        anchors.fill: parent
        implicitHeight: Math.min(250, Screen.desktopAvailableHeight * .95)
        implicitWidth: Math.min(350, Screen.desktopAvailableWidth * .95)

        Keys.onPressed: {
            signIn();
        }

        Image {
            id: image
            height: titleText.paintedHeight + 20
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
                pointSize: 30
                bold: true
            }
            anchors {
                top: parent.top
                left: parent.left
                leftMargin: 10
                right: parent.right
                bottom: image.bottom
            }
        }
        Column {
            id: baseColumn
            anchors {
                left: parent.left
                right: parent.right
                top: image.bottom
                margins: 10
            }
            spacing: 25

            Column {
                spacing: 10
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Text {
                    id: usernameText
                    text: qsTr("Username")
                    horizontalAlignment: Text.AlignLeft
                    font {
                        pointSize: 14
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
                    font.pointSize: 16
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
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Row {
                    spacing: 10
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: signInButton
                        text: "SIGN IN"
                        style: ButtonStyle {
                            background: Rectangle {
                                id: styleRectSignIn
                                implicitWidth: 100
                                implicitHeight: 25
                                border {
                                    width: control.activeFocus ? 2 : 1
                                    color: "#888"
                                }
                                radius: 3
                                color: control.pressed ? "#E68A2E" : "#FFA319"
                            }
                            label: Text {
                                horizontalAlignment: Text.AlignHCenter
                                color: "white"
                                font.bold: true
                                text: signInButton.text
                                font.pixelSize: 18 * displayScaleFactor
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
                                implicitWidth: 100
                                implicitHeight: 25
                                border {
                                    width: control.activeFocus ? 2 : 1
                                    color: "#888"
                                }
                                radius: 3
                                gradient: Gradient {
                                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                                }
                            }
                            label: Text {
                                horizontalAlignment: Text.AlignHCenter
                                color: "black"
                                text: cancelButton.text
                                font.pixelSize: 18 * displayScaleFactor
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

        BusyIndicator {
            id: busyIndicator
            running: busy
            anchors.centerIn: parent
        }
    }

    function signIn() {
        console.log("signin")
        busy = true;
    }

    function cancel() {
        close();
    }
}
