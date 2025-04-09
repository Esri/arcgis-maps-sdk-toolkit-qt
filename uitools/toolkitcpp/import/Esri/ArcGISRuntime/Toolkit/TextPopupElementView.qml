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
    id: textPopupElementView

    /*!
      \internal
    This class is an internal implementation detail and is subject to change.
    */
    property var controller: null

    implicitHeight: richText.implicitHeight

    Label {
        id: richText
        text: controller ? popupView.changeHyperlinkColor(controller.text) : ""
        textFormat: Text.RichText
        wrapMode: Text.WordWrap
        anchors.fill: parent
        leftPadding: 10
        rightPadding: 10

        onLinkActivated: (link) => {
            // emit signal to bubble up link to PopupViewController
            controller.clickedUrl(link);
            if (link !== "" && link !== undefined && popupView.openUrlsWithSystemDefaultApplication) {
                Qt.openUrlExternally(link);
            }
        }

        HoverHandler {
            enabled: parent.hoveredLink
            cursorShape: Qt.PointingHandCursor
        }
    }
}
