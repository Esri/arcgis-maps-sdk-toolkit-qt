/*******************************************************************************
 *  Copyright 2012-2024 Esri
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
  \qmltype ArcGISAuthenticationView_internal
  \internal
 */

Item {
    id: arcgisAuthenticationView_internal

    property AuthenticatorController controller: AuthenticatorController
    signal activeLoginViewReady_(var activeLoginView)

    Connections {
        target: controller

        function onDisplayUsernamePasswordSignInView() {
            displayView(userCredentialsViewComponent);
        }

        function onDisplayOAuthSignInView() {
            displayView(oAuth2ViewComponent);
        }

        function onDisplayServerTrustView() {
            displayView(serverTrustViewComponent);
        }

        function onDisplayClientCertificateView() {
            displayView(clientCertificateViewComponent);
        }
    }

    Component {
        id: clientCertificateViewComponent
        ArcGISClientCertificateView {
            anchors.centerIn: arcgisAuthenticationView_internal
            controller: arcgisAuthenticationView_internal.controller
            onClosed: {
                this.destroy();
            }
            Component.onCompleted: activeLoginViewReady_(this)
        }
    }

    Component {
        id: userCredentialsViewComponent
        ArcGISUserCredentialsView {
            anchors.centerIn: arcgisAuthenticationView_internal
            controller: arcgisAuthenticationView_internal.controller
            onClosed: {
                this.destroy();
            }
            Component.onCompleted: activeLoginViewReady_(this)
        }
    }

    Component {
        id: oAuth2ViewComponent
        ArcGISOAuth2View {
            anchors.centerIn: arcgisAuthenticationView_internal
            controller: arcgisAuthenticationView_internal.controller
            onClosed: {
                this.destroy();
            }
            onWebViewLoaded_: activeLoginViewReady_(this)
        }
    }

    Component {
        id: serverTrustViewComponent
        ServerTrustView {
            anchors.centerIn: arcgisAuthenticationView_internal
            controller: arcgisAuthenticationView_internal.controller
            onClosed: {
                this.destroy();
            }
            Component.onCompleted: activeLoginViewReady_(this)
        }
    }

    function displayView(componentToDisplay) {
        if (componentToDisplay) {
            const incubator = componentToDisplay.incubateObject(arcgisAuthenticationView_internal);
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
