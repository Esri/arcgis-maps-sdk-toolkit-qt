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

Item {
    id: authView
    anchors.fill: parent
    property var authenticationManager
    property var authChallenge

    // Connect to the target (AuthenticationManager)
    // authenticationChallenge signal
    Connections {
        target: authenticationManager

        onAuthenticationChallenge: {
            authChallenge = challenge;

            if (Number(challenge.authenticationChallengeType) === 1) {
                // ArcGIS token, HTTP Basic/Digest, IWA
                loader.source = "UserCredentialsView.qml";
            } else if (Number(challenge.authenticationChallengeType) === 2) {
                // OAuth 2
                loader.source = "OAuth2View.qml";
            } else if (Number(challenge.authenticationChallengeType) === 3) {
                // Client Certificate
                loader.source = "ClientCertificateView.qml";
            } else if (Number(challenge.authenticationChallengeType) === 4) {
                // SSL Handshake - Self-signed certificate
                loader.source = "SslHandshakeView.qml";
            }

            if (loader.item)
                loader.item.visible = true;
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
    }
}
