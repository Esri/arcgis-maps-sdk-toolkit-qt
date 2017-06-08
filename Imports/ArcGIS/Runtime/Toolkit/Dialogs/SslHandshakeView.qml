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
import Esri.ArcGISExtras 1.1

/*!
    \qmltype SslHandshakeView
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since 2.0
    \brief A view for handling SSL Handshake authentication challenges.

    When a request is made to access a resource and there is an SSL
    handshake challenge (such as in the case of a self-signed certificate),
    the AuthenticationView will automatically launch this view. The
    checkbox allows for you to either be prompted each time a SSL handshake
    error occurs from the same host, or only the first time for each host.
*/
Rectangle {
    id: root
    color: "transparent"

    /*!
        \brief The AuthenticationChallenge for SSL Handshake errors.

        \note If using the AuthenticationView, this is set automatically and
         requires no configuration.
    */
    property var challenge

    /*! \internal */
    property string requestingHost: challenge ? challenge.authenticatingHost : ""
    /*! \internal */
    property string detailText: qsTr("The server could not prove itself; its security certificate is not trusted by your OS. Would you like to continue anyway?")
    /*! \internal */
    property real scaleFactor: System.displayScaleFactor

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
                    if (challenge)
                        challenge.continueWithSslHandshake(false, rememberCheckbox.checked);
                    root.visible = false;
                }
            }

            Button {
                width: ((parent.width / 2) - 2 * scaleFactor)
                text: qsTr("Trust")
                onClicked: {
                    // continue SSL handshake and trust host
                    if (challenge)
                        challenge.continueWithSslHandshake(true, rememberCheckbox.checked);
                    root.visible = false;
                }
            }
        }
    }
}
