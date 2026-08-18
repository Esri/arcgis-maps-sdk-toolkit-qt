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
    readonly property int availableTextWidth: Math.max(1, richText.width - richText.leftPadding - richText.rightPadding)

    function sanitizePopupHtmlForWidth(rawHtml, maxWidth) {
        if (!rawHtml)
            return "";

        const safeWidth = Math.max(1, Math.floor(maxWidth || 1));
        let html = popupView.changeHyperlinkColor(rawHtml);

        // Strip width constraints from inline CSS and width attributes.
        html = html.replace(/\sstyle\s*=\s*(['\"])([\s\S]*?)\1/gi, function (_, quote, styleValue) {
            const sanitized = styleValue
                .replace(/(^|;)\s*width\s*:[^;]*/gi, "$1")
                .replace(/^\s*;|;\s*$/g, "")
                .trim();
            return sanitized ? " style=" + quote + sanitized + quote : "";
        });
        html = html.replace(/\swidth\s*=\s*(['\"])?[^'\"\s>]+\1?/gi, "");

        // Clamp only known overflow-prone tags.
        html = html.replace(/<img\b([^>]*)>/gi, "<img$1 width='" + safeWidth + "'>");
        html = html.replace(/<table\b([^>]*)>/gi, "<table$1 width='100%'>");

        return html;
    }

    implicitHeight: richText.implicitHeight

    Label {
        id: richText
        text: controller ? sanitizePopupHtmlForWidth(controller.text, availableTextWidth) : ""
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
