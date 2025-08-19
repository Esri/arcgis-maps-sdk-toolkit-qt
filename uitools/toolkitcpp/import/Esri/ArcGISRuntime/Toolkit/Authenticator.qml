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

/*!
    \qmltype Authenticator
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \ingroup ArcGISQtToolkitUiQmlViews
    \since Esri.ArcGISRuntime 200.8
    \brief A view for handling authentication challenges and automatically
           launching the appropriate UI for each type of authentication.

    The Authenticator provides a default experience for handling authentication challenges
    with the ArcGIS Maps SDK for Qt. The Authenticator will determine which type of authentication
    is in use and will initiate the proper backend controller and UI components.

    To use, declare the Authenticator in your QML UI.

    \code
    Authenticator {
        anchors.centerIn: parent
    }
    \endcode

    \image authenticator_qml.png authenticator

    For further details, consult the \l {https://github.com/Esri/arcgis-maps-sdk-toolkit-qt/blob/main/uitools/toolkitcpp/docs/Authenticator.md}{Authenticator readme}.

    \note OAuth 2.0 uses a WebView. To use a OAuth you must call
          \c{QtWebView::initialize()} before the \c{QGuiApplication}
          instance is created. See \l{https://doc.qt.io/qt-6/qtwebview-index.html}
          {Qt WebView}.
*/

Item {
    id: root

    property var controller: viewLoader.controller
    signal activeLoginViewReady_(var activeLoginView)

    Component.onCompleted: {
        viewLoader.source =  "Authenticator_internal.qml";
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        focus: true
    }

    Connections {
        target: viewLoader.item
        ignoreUnknownSignals: true
        function onActiveLoginViewReady_(activeLoginView) {
            activeLoginViewReady_(activeLoginView);
        }
    }
}
