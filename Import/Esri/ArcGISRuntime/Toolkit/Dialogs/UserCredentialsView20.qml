/*******************************************************************************
 * Copyright 2012-2016 Esri
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

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import Esri.ArcGISExtras 1.2

/*!
    \internal
    \obsolete Use version 100.x instead.
    \qmltype UserCredentialsView
    \ingroup ArcGISQtToolkit
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
    property real scaleFactor: System.displayScaleFactor
    /*! \internal */
    property string requestingHost: challenge ? challenge.authenticatingHost : ""
    /*! \internal */
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
            GradientStop { position: 1.0; color: "black" }
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

