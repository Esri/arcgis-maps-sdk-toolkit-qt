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
    \qmltype SslHandshakeView
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since Esri.ArcGISRutime 100.0
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
            id: gridLayout
            columns: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.column: 0
                color: "white"
                border {
                    color: "black"
                    width: 1
                }
                radius: 3
                smooth: true
                antialiasing: true
                clip:true
                Layout.minimumWidth: childrenRect.width
                Layout.minimumHeight: childrenRect.height

                Image {
                    anchors.fill: parent
                    source: "images/banner.png"
                }

                ColumnLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Untrusted Host")
                        horizontalAlignment: Qt.AlignHCenter
                        padding: 5
                        font {
                            pixelSize: 18
                            family: "sanserif"
                        }
                        color: "white"
                    }

                    Text {
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                        text: requestingHost
                        horizontalAlignment: Qt.AlignHCenter
                        font {
                            pixelSize: 12
                            family: "sanserif"
                        }
                        color: "white"
                    }
                }
            }

            Text {
                text: detailText
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                wrapMode: Text.Wrap
                font {
                    pixelSize: 10
                    family: "sanserif"
                }
            }

            CheckBox {
                id: rememberCheckbox
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr("Remember")
            }

            Button {
                Layout.margins: 10
                Layout.alignment: Qt.AlignLeft
                text: qsTr("Block")
                onClicked: {
                    // reject the challenge and let the resource fail to load
                    if (challenge)
                        challenge.continueWithSslHandshake(false, rememberCheckbox.checked);
                    root.visible = false;
                }
            }

            Button {
                id: continueButton
                Layout.margins: 10
                Layout.alignment: Qt.AlignRight
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
