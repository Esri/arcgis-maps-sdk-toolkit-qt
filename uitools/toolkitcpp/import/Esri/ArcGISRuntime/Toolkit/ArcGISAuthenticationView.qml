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

import QtQuick
import QtQuick.Controls

Item {
    id: root
    Component.onCompleted: {
        viewLoader.source = ArcGISAuthenticationController.canBeUsed ? "ArcGISAuthenticationView_internal.qml" : "AuthenticationView.qml";
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
    }

    Connections {
        target: viewLoader.source
        function onActiveLoginViewReady(activeLoginView) {
            activeLoginViewReady_(activeLoginView);
        }
    }

    signal activeLoginViewReady_(var activeLoginView)
}
