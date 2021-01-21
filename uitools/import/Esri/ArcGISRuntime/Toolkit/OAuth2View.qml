/*******************************************************************************
 *  Copyright 2012-2020 Esri
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

import Esri.ArcGISRuntime.Toolkit.Controller 100.10

import QtQuick 2.11
import QtQuick.Controls 2.11
import QtWebView 1.1
import QtQuick.Window 2.12

Dialog {
    id: oAuthView

    property AuthenticationController controller: AuthenticationController {}

    title: webView.title

    closePolicy: Popup.NoAutoClose

    header: Label {
        text: `<h2>${webView.title}</h2>`
        horizontalAlignment: Text.AlignHCenter
    }
    
    footer: DialogButtonBox {
        standardButtons: +DialogButtonBox.Close
        onRejected: controller.cancel();
    }

    implicitWidth: Screen.width
    implicitHeight: Screen.height

    WebView {
        id: webView
        anchors {
            centerIn: parent
            fill: parent
        }
        
        url: controller.currentChallengeUrl

        onLoadingChanged: {
            if (loadRequest.status === WebView.LoadSucceededStatus) {
                forceActiveFocus();
            } else if (loadRequest.status === WebView.LoadFailedStatus) {
                controller.cancelWithError("Failed to load");
            }
        }

        onTitleChanged: {
            if (title.indexOf("SUCCESS code=") > -1) {
                const authCode = title.replace("SUCCESS code=", "");
                controller.continueWithOAuthAuthorizationCode(authCode);
            } else if (title.indexOf("Denied error=") > -1) {
                const errorString = title.replace("Denied error=", "");
                controller.cancelWithError(errorString);
            } else if (title.indexOf("Error: ") > -1) {
                const errorString = title.replace("Error: ", "");
                controller.cancelWithError(errorString);
            }
        }
    }
}

