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
import QtWebView 1.1

WebView {
    id: webView
    anchors.fill: parent
    property var challenge: authView.authChallenge
    url: challenge ? challenge.authorizationUrl : ""

    onLoadingChanged: {
        if (loadRequest.status === WebView.LoadSucceededStatus) {
            if (title.indexOf("SUCCESS code=") > -1) {
                var authCode = title.replace("SUCCESS code=", "");
                if (challenge)
                    challenge.continueWithOAuthAuthorizationCode(authCode);
                webView.visible = false;
            } else if (title.indexOf("Denied error=") > -1) {
                if (challenge)
                    challenge.cancel();
                webView.visible = false;
            }
        }
    }
}
