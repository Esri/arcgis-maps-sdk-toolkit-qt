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
