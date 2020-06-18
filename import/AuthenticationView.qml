import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: authenticationView

    property var controller: AuthenticationController { }

    Connections {
        target: controller
        onCurrentChallengeTypeChanged: {
            const component = internal.viewNameForChallengeType(controller.currentChallengeType);
            if (component) {
                const incubator = component.incubateObject(authenticationView);
                incubator.onStatusChanged = function(status) {
                    if (status === Component.Ready) {
                        this.object.open();
                    }
                }
            }
        }
    }

    Component {
        id: userCredentialsView
        UserCredentialsView {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                onCurrentChallengeTypeChanged: {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }

    Component {
        id: oAuth2View
        OAuth2View {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                onCurrentChallengeTypeChanged: {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }

    Component {
        id: clientCertificateView
        ClientCertificateView {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                onCurrentChallengeTypeChanged: {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }

    Component {
        id: sslHandshakeView
        SslHandshakeView {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                onCurrentChallengeTypeChanged: {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }

    QtObject {
        id: internal
        function viewNameForChallengeType(type) {
            if (controller.currentChallengeType === 1) {
                // ArcGIS token, HTTP Basic/Digest, IWA
                return userCredentialsView;
            } else if (controller.currentChallengeType === 2) {
                // OAuth 2
                return oAuth2View;
            } else if (controller.currentChallengeType === 3) {
                // Client Certificate
                return clientCertificateView;
            } else if (controller.currentChallengeType === 4) {
                // SSL Handshake - Self-signed certificate
                return sslHandshakeView;
            } else {
                // Load nothing, challenge not understood.
                return null;
            }
        }
    }
}
