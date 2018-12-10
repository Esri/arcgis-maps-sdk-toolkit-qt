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

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Window 2.0

/*!
    \qmltype UserCredentialsView
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since Esri.ArcGISRutime 100.0
    \brief A view for handling username and password authentication challenges.

    When a request is made to access a resource that requires a username and
    password, the AuthenticationView will automatically launch this view. This
    is applicable for:

    \list
      \li ArcGIS Token
      \li HTTP Digest
      \li HTTP Basic
      \li Integrated Windows Authentication (IWA)
    \endlist

    \note In the case of using an IWA secured resource on a Windows system, the
    OS will automatically handle the authentication, and no UI dialog will appear.
*/
Rectangle {
    id: root
    color: "transparent"

    /*!
        \brief The AuthenticationChallenge for ArcGIS Token, HTTP Basic, HTTP Digest, and IWA.

        \note If using the AuthenticationView, this is set automatically and
         requires no configuration.
    */
    property var challenge

    /*! \internal */
    property string requestingHost: challenge ? challenge.authenticatingHost : ""
    /*! \internal */
    property string detailText: qsTr("You need to sign in to access the resource at:")

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
            GradientStop { position: 1.0; color: "black" }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
        onWheel: wheel.accepted = true
    }


    Rectangle {
        color: "white"
        border {
            color: "black"
            width: 1
        }
        radius: 3
        smooth: true
        clip: true
        antialiasing: true
        anchors.centerIn: parent
        width: childrenRect.width
        height: childrenRect.height


        GridLayout {
            columns: 2

            Rectangle {
                id: banner
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.margins: 1
                Layout.minimumWidth: childrenRect.width
                Layout.minimumHeight: childrenRect.height

                color: "white"
                border {
                    color: "black"
                    width: 1
                }
                radius: 3
                smooth: true
                antialiasing: true

                Image {
                    anchors.fill: parent
                    source: "images/banner.png"
                }

                Text {
                    id: titleText
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("Authentication Required")
                    padding: 5
                    horizontalAlignment: Qt.AlignHCenter
                    font {
                        pixelSize: 18
                        family: "sanserif"
                    }
                    color: "white"
                }
            }

            Rectangle {
                color: "#FFCCCC"
                radius: 5
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.columnSpan: 2
                height: childrenRect.height
                visible: challenge ? challenge.failureCount > 1 : false

                Text {
                    text: qsTr("Invalid username or password.")
                    font {
                        pixelSize: 12
                        family: "sanserif"
                    }
                    color: "red"
                }
            }

            Text {
                text: detailText
                Layout.fillWidth: true
                Layout.rightMargin: 10
                Layout.leftMargin: 10
                Layout.topMargin: 10
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                wrapMode: Text.Wrap
                font {
                    pixelSize: 12
                    family: "sanserif"
                }
            }

            Text {
                text: requestingHost
                Layout.fillWidth: true
                Layout.rightMargin: 10
                Layout.leftMargin: 10
                Layout.bottomMargin: 10
                Layout.columnSpan: 2
                wrapMode: Text.Wrap
                font {
                    pixelSize: 12
                    family: "sanserif"
                }
            }

            TextField {
                id: usernameTextField
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.columnSpan: 2
                placeholderText: qsTr("username")
            }

            TextField {
                id: passwordTextField
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.columnSpan: 2
                placeholderText: qsTr("password")
                echoMode: TextInput.Password
            }

            Button {
                Layout.margins: 10
                Layout.alignment: Qt.AlignLeft
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
                Layout.margins: 10
                Layout.alignment: Qt.AlignRight
                text: qsTr("Continue")
                highlighted: true
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
