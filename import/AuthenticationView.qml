import QtQuick.Controls 2.11

Dialog {
    id: authenticationView
    contentItem: Loader {
        focus: true
        source: {
            if (+challenge.authenticationChallengeType === 1) {
                // ArcGIS token, HTTP Basic/Digest, IWA
                return "UserCredentialsView.qml";
            } else if (+challenge.authenticationChallengeType === 2) {
                // OAuth 2
                return "OAuth2View.qml";
            } else if (+challenge.authenticationChallengeType === 3) {
                // Client Certificate
                return "ClientCertificateView.qml";
            } else if (+challenge.authenticationChallengeType === 4) {
                // SSL Handshake - Self-signed certificate
                return "SslHandshakeView.qml";
            } else {
                // Load nothing, challenge not understood.
                return null;
            }
        }
    }

    QtObject {
        id: internal
        property var authenticationManager: null;
        property Connection authenticationConnection: Connection {
            target: authenticationManager
            onAuthenticationChallenge {

            }
        }
    }
}