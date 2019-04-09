/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0

/*!
    \qmltype ClientCertificateView
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since Esri.ArcGISRutime 100.0
    \brief A view for handling PKI authentication challenges.

    When a request is made to access a resource that is secured with PKI
    security, the AuthenticationView will automatically launch this view.
    The view allows you to browse to browse to different certificate files,
    and will automatically prompt for a password if one is required.
*/
Rectangle {
    id: root
    color: "transparent"

    /*!
        \brief The AuthenticationChallenge for PKI security.

        \note If using the AuthenticationView, this is set automatically and
         requires no configuration.
    */
    property var challenge

    /*! \internal */
    property string detailText: qsTr("The service has requested a client certificate to authenticate you. The app has identified the requesting server as '%1', but you should only give the app access to the certificate if you trust it.").arg(requestingHost)
    /*! \internal */
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" || Qt.platform.os === "linux" ? 96 : 72)
    /*! \internal */
    property string requestingHost: challenge ? challenge.authenticatingHost : ""
    /*! \internal */
    property string certificateFilename
    /*! \internal */
    property string passwordDetailText: qsTr("The client certificate file '%1' requires a password to open.").arg(certificateFilename)

    RadialGradient {
        anchors.fill: parent
        opacity: 0.7
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightgrey" }
            GradientStop { position: 1.0; color: "black" }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
        onWheel: wheel.accepted = true
    }

    Rectangle {
        id: certificatePage
        anchors.centerIn: parent
        height: banner.height + controlsColumn.height

        Rectangle {
            anchors {
                fill: banner
                margins: -1 * displayScaleFactor
            }
            color: "white"
            border {
                color: "black"
                width: 1 * displayScaleFactor
            }
            radius: 3
            smooth: true
            clip: true
            antialiasing: true
        }

        Image {
            id: banner
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            width: 224 * displayScaleFactor
            height: 50 * displayScaleFactor
            clip: true
            source: "images/banner.png"

            Column {
                anchors {
                    fill: parent
                    margins: 5 * displayScaleFactor
                }

                spacing: 2 * displayScaleFactor

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Client Certificate Requested")
                    font {
                        pixelSize: 16 * displayScaleFactor
                        family: "sanserif"
                    }
                    color: "white"
                    renderType: Text.NativeRendering
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    elide: Text.ElideRight
                    text: requestingHost
                    font {
                        pixelSize: 12 * displayScaleFactor
                        family: "sanserif"
                    }
                    color: "white"
                    renderType: Text.NativeRendering
                }
            }
        }

        Rectangle {
            anchors {
                fill: controlsColumn
                margins: -5 * displayScaleFactor
            }
            color: "white"
            border {
                color: "black"
                width: 1 * displayScaleFactor
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
                topMargin: 5 * displayScaleFactor
                horizontalCenter: parent.horizontalCenter
            }
            width: 215 * displayScaleFactor
            spacing: 5 * displayScaleFactor

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: detailText
                width: parent.width
                wrapMode: Text.Wrap
                font {
                    pixelSize: 10 * displayScaleFactor
                    family: "sanserif"
                }
                renderType: Text.NativeRendering
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Available Certificates:")
                width: parent.width
                wrapMode: Text.Wrap
                font {
                    pixelSize: 12 * displayScaleFactor
                    family: "sanserif"
                }
                renderType: Text.NativeRendering
            }


            Rectangle {
                width: parent.width
                height: 80 * displayScaleFactor
                anchors.margins: 10 * displayScaleFactor
                color: "white"
                radius: 5
                border {
                    color: "black"
                    width: 1 * displayScaleFactor
                }

                ListView {
                    id: certificateList
                    anchors {
                        fill: parent
                        margins: 5 * displayScaleFactor
                    }

                    spacing: 10 * displayScaleFactor
                    clip: true
                    highlightFollowsCurrentItem: true
                    highlightResizeVelocity: 10000
                    highlight: Rectangle { color: "lightsteelblue"; radius: 1 }

                    // hook up to AuthenticationManagers's clientCertificateInfos list
                    model: authView.authenticationManager.clientCertificateInfos

                    delegate: Item {
                        width: parent.width
                        height: 25 * displayScaleFactor
                        anchors {
                            leftMargin: 5 * displayScaleFactor
                            rightMargin: 5 * displayScaleFactor
                        }

                        Row {
                            Text {
                                text: modelData
                                width: certificateList.width
                                clip: true
                                elide: Text.ElideRight
                                renderType: Text.NativeRendering
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                certificateList.currentIndex = index;
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: addButton
                property bool pressed: false
                anchors.right: parent.right
                width: 25 * displayScaleFactor
                height: 25 * displayScaleFactor
                radius: 25
                smooth: true
                antialiasing: true
                color: pressed ? "#959595" : "#D6D6D6"
                border {
                    color: "#585858"
                    width: 1 * displayScaleFactor
                }

                Image {
                    anchors.centerIn: parent
                    rotation: 45
                    width: 20 * displayScaleFactor
                    height: width
                    source: "images/add.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: addButton.pressed = true
                    onReleased: addButton.pressed = false
                    onClicked: {
                        fileDialog.open();
                    }
                }
            }

            Row {
                width: parent.width
                spacing: 4

                Button {
                    width: ((parent.width / 2) - 2 * displayScaleFactor)
                    text: qsTr("Skip")
                    onClicked: {
                        // cancel the challenge and let the resource fail to load
                        if (challenge)
                            challenge.cancel();
                        root.visible = false;
                    }
                }

                Button {
                    width: ((parent.width / 2) - 2 * displayScaleFactor)
                    text: qsTr("Continue")
                    enabled: certificateList.currentIndex != -1
                    onClicked: {
                        // continue with client certificate file
                        if (challenge)
                            challenge.continueWithClientCertificate(certificateList.currentIndex);
                        root.visible = false;
                    }
                }
            }
        }
    }

    Rectangle {
        id: certificatePasswordPage
        property url certificateFile
        anchors.centerIn: parent
        height: banner2.height + controlsColumn2.height
        visible: false

        Rectangle {
            anchors {
                fill: banner2
                margins: -1 * displayScaleFactor
            }
            color: "white"
            border {
                color: "black"
                width: 1 * displayScaleFactor
            }
            radius: 3
            smooth: true
            clip: true
            antialiasing: true
        }

        Image {
            id: banner2
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            width: 224 * displayScaleFactor
            height: 50 * displayScaleFactor
            clip: true
            source: "images/banner.png"

            Text {
                anchors.centerIn: parent
                text: qsTr("Enter Password")
                font {
                    pixelSize: 18 * displayScaleFactor
                    family: "sanserif"
                }
                color: "white"
                renderType: Text.NativeRendering
            }
        }

        Rectangle {
            anchors {
                fill: controlsColumn2
                margins: -5 * displayScaleFactor
            }
            color: "white"
            border {
                color: "black"
                width: 1 * displayScaleFactor
            }
            radius: 3
            smooth: true
            clip: true
            antialiasing: true
        }

        Column {
            id: controlsColumn2
            anchors {
                top: banner2.bottom
                topMargin: 5 * displayScaleFactor
                horizontalCenter: parent.horizontalCenter
            }
            width: 215 * displayScaleFactor
            spacing: 5 * displayScaleFactor

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: passwordDetailText
                width: parent.width
                wrapMode: Text.Wrap
                font {
                    pixelSize: 10 * displayScaleFactor
                    family: "sanserif"
                }
                renderType: Text.NativeRendering
            }

            TextField {
                id: passwordTextField
                width: parent.width
                placeholderText: qsTr("password")
                echoMode: TextInput.Password
            }

            Row {
                width: parent.width
                spacing: 4
                Button {
                    width: ((parent.width / 2) - 2 * displayScaleFactor)
                    text: qsTr("Skip")
                    onClicked: {
                        certificatePasswordPage.visible = false;
                        certificatePage.visible = true;
                        passwordTextField.text = "";
                    }
                }

                Button {
                    width: ((parent.width / 2) - 2 * displayScaleFactor)
                    text: qsTr("Continue")
                    onClicked: {
                        certificatePasswordPage.visible = false;
                        certificatePage.visible = true;

                        // attempt to add client certificate with the provided password
                        authView.authenticationManager.addClientCertificate(certificatePasswordPage.certificateFile, passwordTextField.text);
                        passwordTextField.text = "";
                    }
                }
            }
        }
    }

    Connections {
        target: authView.authenticationManager

        onClientCertificatePasswordRequired: {
            certificatePasswordPage.certificateFile = certificate;

            certificateFilename = certificatePasswordPage.certificateFile.toString().split("/").pop();

            // the certificate requires password
            certificatePage.visible = false;
            certificatePasswordPage.visible = true;
        }
    }

    FileDialog {
        id: fileDialog

        onAccepted: {
            // attempt to add client certificate without password
            authView.authenticationManager.addClientCertificate(fileDialog.fileUrl);
        }
    }
}
