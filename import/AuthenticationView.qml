import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: authenticationView

    property var controller: AuthenticationController { }

    contentItem: Loader {
        id: challengeViewLoader
    }

    closePolicy: Popup.NoAutoClose

    Connections {
        target: controller
        onCurrentChallengeTypeChanged: {
            challengeViewLoader.setSource(
                viewNameForChallengeType(controller.currentChallengeType),
                { controller: controller });
        }

        onChallengeChanged: {
            if (controller.currentChallengeType === 0) {
                authenticationView.close();
            } else {
                authenticationView.open();
            }
        }
    }

    function viewNameForChallengeType(type) {
        if (controller.currentChallengeType === 1) {
            // ArcGIS token, HTTP Basic/Digest, IWA
            return "UserCredentialsView.qml";
        } else if (controller.currentChallengeType === 2) {
            // OAuth 2
            return "OAuth2View.qml";
        } else if (controller.currentChallengeType === 3) {
            // Client Certificate
            return "ClientCertificateView.qml";
        } else if (controller.currentChallengeType === 4) {
            // SSL Handshake - Self-signed certificate
            return "SslHandshakeView.qml";
        } else {
            // Load nothing, challenge not understood.
            return "";
        }
    }
}
