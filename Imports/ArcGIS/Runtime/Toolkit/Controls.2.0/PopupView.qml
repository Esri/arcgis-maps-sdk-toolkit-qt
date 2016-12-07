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

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import Esri.ArcGISExtras 1.1

/*!
    \qmltype PopupView
    \ingroup ArcGISQtToolkit
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since 2.0
    \brief
*/

Item {
    id: popupView

    /* public properties */
    property var popupManager: null
    property real popupWidth: 275 * scaleFactor
    property real popupHeight: 350 * scaleFactor

    /* public functions */
    function show() {
        popupView.visible = true;
    }

    function dismiss() {
        popupView.visible = false;
    }

    /* internal */
    visible: false
    width: popupWidth
    height: popupHeight
    property real scaleFactor: System.displayScaleFactor

    PopupViewBase {
        anchors.fill: parent
        popupManagerInternal: popupManager
    }
}
