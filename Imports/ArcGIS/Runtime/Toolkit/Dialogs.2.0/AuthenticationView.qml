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

/*!
    \qmltype AuthenticationView
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Dialogs
    \since Esri::ArcGISRutime::Toolkit 2.0
    \brief A view for handling authentication challenges and automatically launching
    the appropriate UI for each type of authentication.

    Declare an AuthenticationView in your QML file, and set the
    authenticationManager property. The AuthenticationView will
    then be connected to all authentication challenges, and will
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
    
    \note OAuth 2.0 uses a WebView, and in Qt, certain platforms 
    fall back to use the WebEngine. Because of this, it is advised 
    that you add the following lines to your project's .pro file
    and main.cpp when using the AuthenticationView:
    
    *.pro:
    \code
    qtHaveModule(webengine) {
      QT += webengine
      DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
    }
    \endcode
    
    main.cpp:
    \code
    #ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    #include <QtWebEngine>
    #endif // QT_WEBVIEW_WEBENGINE_BACKEND
    \endcode
    
    main.cpp:
    \code
    #ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
    #endif // QT_WEBVIEW_WEBENGINE_BACKEND
    \endcode
*/
Item {
    id: authView
    anchors.fill: parent

    Component.onCompleted: {
        var parentsChildren = parent.children;
        var maxZ = 0;
        for (var i = 0; i < parentsChildren.length; i++){
            if (parentsChildren[i] === authView)
                continue;
            maxZ = Math.max(maxZ, parentsChildren[i].z);
        }
        authView.z = maxZ + 1; // the view should be shown on top
    }

    /*!
        \brief The AuthenticationManager instance.

        This property is required so that the view can receive the authenticationChallenge
        signals. In the case of the QML API, set this to the AuthenticationManager
        singleton. In the case of the C++ API, you must register the AuthenticationManager
        as a QML type before setting it in your QML UI.
    */
    property var authenticationManager

    /*! \internal */
    property var authChallenge
    /*! \internal */
    property var currentView

    // Connect to the target (AuthenticationManager)
    // authenticationChallenge signal
    Connections {
        target: authenticationManager

        onAuthenticationChallenge: {
            authChallenge = challenge;

            if (Number(challenge.authenticationChallengeType) === 1) {
                // ArcGIS token, HTTP Basic/Digest, IWA
                createView("UserCredentialsView.qml");
            } else if (Number(challenge.authenticationChallengeType) === 2) {
                // OAuth 2
                createView("OAuth2View.qml");
            } else if (Number(challenge.authenticationChallengeType) === 3) {
                // Client Certificate
                createView("ClientCertificateView.qml");
            } else if (Number(challenge.authenticationChallengeType) === 4) {
                // SSL Handshake - Self-signed certificate
                createView("SslHandshakeView.qml");
            }
        }

        function createView(fileUrl) {
            if (currentView) {
                // destroy current active view
                currentView.destroy();
            }

            // create and display new view
            var component = Qt.createComponent(fileUrl);
            if (component.status === Component.Ready) {
                currentView = component.createObject(authView);
                currentView.challenge = authChallenge;
                currentView.anchors.fill = authView;
            }
        }
    }
}
