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

import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import ArcGIS.Runtime 10.26
import ArcGIS.Runtime.Toolkit.Controls 1.0
import ArcGIS.Runtime.Toolkit.Dialogs 1.0

Rectangle {
    id: searchBoxExample
    clip: true

    property string errorString
    property bool signedIn: false
    property string portalUrl: "http://arcgis.com"
    property Portal portal
    signal signInCompleted
    signal signInErrored(var error)

    UserCredentialsDialog {
        id: userCredentialsDialog
        visible: false

        onAccepted: {
            busy = true;
            portal = ArcGISRuntime.createObject("Portal", {url: portalUrl});
            portal.signInComplete.connect(function () {
                signedIn = true;
                signInCompleted();
                busy = false;
                visible = false;
            });
            portal.signInError.connect(function (error) {
                visible = false;
                signInErrored(error);
                errorString = "Error during sign in.\n" + error.code + ": " + error.message + "\n" + error.details;
                busy = false;
                errorDialog.open();
            });
            userCredentials.userName = username;
            userCredentials.password = password;
            portal.credentials = userCredentials;
            portal.signIn();
        }

        UserCredentials {
            id: userCredentials
        }

        MessageDialog {
            id: errorDialog
            text: errorString
            title: "Sign In Error"
            standardButtons: StandardButton.Ok
            onAccepted: {
                userCredentialsDialog.visible = true;
                if (Qt.platform.os !== "ios" && Qt.platform.os != "android") {
                   userCredentialsDialog.height = userCredentialsDialog.internalContent.height
                   userCredentialsDialog.width = userCredentialsDialog.internalContent.width
                }
            }
        }
    }

    Column {
        anchors {
            fill: parent
            margins: 15
        }

        Text {
            width: searchBoxExample.width
            clip: true
            font.pointSize: 14
            wrapMode: Text.WordWrap
            text: signedIn ? "Full name: " + portal.user.fullName : "";
        }

        Text {
            width: searchBoxExample.width
            clip: true
            font.pointSize: 14
            wrapMode: Text.WordWrap
            text: signedIn ? "\nCreated on: " + portal.user.created : "";
        }

        Text {
            width: searchBoxExample.width
            clip: true
            font.pointSize: 14
            wrapMode: Text.WordWrap
            text: signedIn ? "\nModified on: " + portal.user.modified : "";
        }

        Text {
            width: searchBoxExample.width
            clip: true
            font.pointSize: 14
            wrapMode: Text.WordWrap
            text: signedIn ? "\nOrganization Id: " + portal.portalInfo.organizationId : "";
        }

        Text {
            width: searchBoxExample.width
            clip: true
            wrapMode: Text.WrapAnywhere
            font.pointSize: 14
            text: signedIn ? "\nLicense string: " + portal.portalInfo.licenseInfo.json["licenseString"] : "";
        }
    }

    Button {
        id: button
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 15
        }
        text: "Sign In"
        onClicked: {
            userCredentialsDialog.visible = true;
            if (Qt.platform.os !== "ios" && Qt.platform.os != "android") {
               userCredentialsDialog.height = userCredentialsDialog.internalContent.height
               userCredentialsDialog.width = userCredentialsDialog.internalContent.width
            }
        }
    }

    Component.onCompleted: userCredentialsDialog.visible = false;
}
