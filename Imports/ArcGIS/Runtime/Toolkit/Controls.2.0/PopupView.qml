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
    property color backgroundColor: "#f2f3f4"
    property color borderColor: "#4f4f4f"
    property real borderWidth: 2 * scaleFactor
    property real radius: 2 * scaleFactor
    property color titleTextColor: "black"
    property real titleTextSize: 13 * scaleFactor
    property color attributeNameTextColor: "gray"
    property color attributeValueTextColor: "#4f4f4f"
    visible: false
    width: 275 * scaleFactor
    height: 350 * scaleFactor

    /* public functions */
    function show() {
        visible = true;
    }

    function dismiss() {
        visible = false;
    }

    /* public signals */
    signal attachmentThumbnailClicked(var index)

    /* internal */
    property real scaleFactor: System.displayScaleFactor   

    PopupViewBase {
        anchors.fill: parent
        popupManagerInternal: popupManager
        backgroundColorInternal: backgroundColor
        borderColorInternal: borderColor
        borderWidthInternal: borderWidth
        radiusInternal: radius
        titleTextSizeInternal: titleTextSize
        attributeNameTextColorInternal: attributeNameTextColor
        attributeValueTextColorInternal: attributeValueTextColor
        titleTextColorInternal: titleTextColor

        onAttachmentThumbnailClickedInternal: attachmentThumbnailClicked(index)
    }
}
