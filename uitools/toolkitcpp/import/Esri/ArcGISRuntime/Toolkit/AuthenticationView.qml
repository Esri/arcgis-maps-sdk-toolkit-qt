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
import Esri.ArcGISRuntime.Toolkit.Controller

import QtQml
import QtQuick
import QtQuick.Controls

/*!
  \qmltype AuthenticationView
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \ingroup ArcGISQtToolkitUiQmlViews
  \since Esri.ArcGISRuntime 100.10
  \brief A view for handling authentication challenges and automatically
  launching the appropriate UI for each type of authentication.
  \deprecated [200.8] Use Authenticator instead.

  \note This component is deprecated and will be removed in an upcoming release. Use Authenticator instead.

  To migrate to `Authenticator`, change 

  \code
  AuthenticationView {
      anchors.centerIn: parent
  }
  \endcode

  to:

  \code
  Authenticator {
      anchors.centerIn: parent
  }
  \endcode

  Declare an AuthenticationView in your QML file. The AuthenticationView
  will then be connected to all authentication challenges, and will
  automatically launch the appropriate view for the type of
  challenge. Supported security formats include:

  \list
    \li ArcGIS Token (UserCredentialsView)
    \li HTTP Basic (UserCredentialsView)
    \li HTTP Digest (UserCredentialsView)
    \li IWA (UserCredentialsView)
    \li OAuth 2.0 (OAuth2View)
    \li SAML (OAuth2View)
    \li PKI (ClientCertificateView)
    \li SSL Handshake Warnings (SslHandshakeView)
  \endlist

  \note OAuth 2.0 uses a WebView. To use an OAuthView you must call
        \c{QtWebView::initialize()} before the \c{QGuiApplication}
        instance is created. See \l{https://doc.qt.io/qt-6/qtwebview-index.html}
        {Qt WebView}.

  \image authenticationview_qml.png authentication_view
  \code
  AuthenticationView {
    anchors.centerIn: parent
  }
  \endcode
 */

Item {
    id: authenticationView

    /*!
      \internal
    */
    property var controller: AuthenticationController { }

    Connections {
        target: controller
        function onCurrentChallengeTypeChanged() {
            const component = internal.viewNameForChallengeType(controller.currentChallengeType);
            if (component) {
                const incubator = component.incubateObject(authenticationView);
                if (incubator.status === Component.Ready) {
                    incubator.object.open();
                } else {
                    incubator.onStatusChanged = function(status) {
                        if (status === Component.Ready) {
                            this.object.open();
                        }
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
                function onCurrentChallengeTypeChanged() {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
            Component.onCompleted: activeLoginViewReady_(this)
        }
    }

    Component {
        id: oAuth2View
        OAuth2View {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                function onCurrentChallengeTypeChanged() {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
            onWebViewLoaded_: activeLoginViewReady_(this)
        }
    }

    Component {
        id: clientCertificateView
        ClientCertificateView {
            anchors.centerIn: authenticationView
            controller: authenticationView.controller
            Connections {
                target: controller
                function onCurrentChallengeTypeChanged() {
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
                function onCurrentChallengeTypeChanged() {
                    reject();
                }
            }
            onClosed: {
                this.destroy();
            }
        }
    }

    /*!
        \internal
        \brief signal emitted when the login view is in the Ready state
    */
    signal activeLoginViewReady_(var activeLoginView)

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
