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
import QtWebView 1.1

/*!
    \qmltype OAuth2View
    \ingroup ArcGISQtToolkit
    \ingroup ArcGISQtToolkitCppApi
    \ingroup ArcGISQtToolkitQmlApi
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since Esri.ArcGISRutime 100.0
    \brief A view for handling OAuth 2.0 authentication challenges.

    When a request is made to access a resource via OAuth 2.0, the
    AuthenticationView will automatically launch this view. The
    OAuth2View is used for standard ArcGIS Online/Portal OAuth logins,
    SAML, and Enterprise logins.
*/
WebView {
    id: webView
    anchors.fill: parent

    /*!
        \brief The AuthenticationChallenge for PKI security.

        \note If using the AuthenticationView, this is set automatically and
         requires no configuration.
    */
    property var challenge

    /*! \internal */
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
