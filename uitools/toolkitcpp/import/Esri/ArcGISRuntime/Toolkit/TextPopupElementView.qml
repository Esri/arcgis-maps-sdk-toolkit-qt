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

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window
import QtQuick.Layouts
import QtWebView

Page {
    id: textPopupElementView
    property string htmlString: ""

    anchors.fill: parent
//    implicitWidth: 300 + padding

//    implicitHeight: 300 + padding

//    spacing: 5
//    leftPadding: textPopupElementView.spacing
//    rightPadding: textPopupElementView.spacing

//    title: controller.title
    WebView {
        id: webView
        anchors.fill: parent

        Component.onCompleted: {
//            var htmlString = "<p><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>White Mountain Peak</strong></span><span style='font-family:Verdana;font-size:14px;'> is a peak in California's White Mountains range. It ranks </span><span style='color:#aa3427;font-family:Verdana;font-size:14px;'><strong>#3</strong></span><span style='font-family:Verdana;font-size:14px;'> among the California Fourteeners.</span></p><p><span style='font-family:Verdana;font-size:14px;'>The summit is </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>14,252</strong></span><span style='font-family:Verdana;font-size:14px;'> feet high (4,344 meters) and has a prominence of </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>7,196</strong></span><span style='font-family:Verdana;font-size:14px;'> feet (2,193 meters).</span></p><p><a href='https://en.wikipedia.org/wiki/List_of_California_fourteeners' rel='nofollow ugc'><span style='font-family:Verdana;font-size:14px;'>More info</span></a></p>"
             webView.loadHtml(htmlString);
        }

        onUrlChanged: {
            print("");
            print(webView.url);
            print("");
            if (webView.url.toString() !== "") {
//                Qt.openUrlExternally(webView.url.toString());
                //load external url via C++
                var htmlString2 = "<p><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>White Mountain Peak</strong></span><span style='font-family:Verdana;font-size:14px;'> is a peak in California's White Mountains range. It ranks </span><span style='color:#aa3427;font-family:Verdana;font-size:14px;'><strong>#3</strong></span><span style='font-family:Verdana;font-size:14px;'> among the California Fourteeners.</span></p><p><span style='font-family:Verdana;font-size:14px;'>The summit is </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>14,252</strong></span><span style='font-family:Verdana;font-size:14px;'> feet high (4,344 meters) and has a prominence of </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>7,196</strong></span><span style='font-family:Verdana;font-size:14px;'> feet (2,193 meters).</span></p><p><a href='https://en.wikipedia.org/wiki/List_of_California_fourteeners' rel='nofollow ugc'><span style='font-family:Verdana;font-size:14px;'>More info</span></a></p>"
                webView.loadHtml(htmlString2); // Reload the original HTML
            }
        }

//        url: "data:text/html,<p><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>White Mountain Peak</strong></span><span style='font-family:Verdana;font-size:14px;'> is a peak in California's White Mountains range. It ranks </span><span style='color:#aa3427;font-family:Verdana;font-size:14px;'><strong>#3</strong></span><span style='font-family:Verdana;font-size:14px;'> among the California Fourteeners.</span></p><p><span style='font-family:Verdana;font-size:14px;'>The summit is </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>14,252</strong></span><span style='font-family:Verdana;font-size:14px;'> feet high (4,344 meters) and has a prominence of </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>7,196</strong></span><span style='font-family:Verdana;font-size:14px;'> feet (2,193 meters).</span></p><p><a href='https://en.wikipedia.org/wiki/List_of_California_fourteeners' rel='nofollow ugc'><span style='font-family:Verdana;font-size:14px;'>More info</span></a></p>"
    }
//    Text {
//        id: textView
//        textFormat: Text.RichText
//        anchors.centerIn: parent
//        text: "<p><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>White Mountain Peak</strong></span><span style='font-family:Verdana;font-size:14px;'> is a peak in California's White Mountains range. It ranks </span><span style='color:#aa3427;font-family:Verdana;font-size:14px;'><strong>#3</strong></span><span style='font-family:Verdana;font-size:14px;'> among the California Fourteeners.</span></p><p><span style='font-family:Verdana;font-size:14px;'>The summit is </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>14,252</strong></span><span style='font-family:Verdana;font-size:14px;'> feet high (4,344 meters) and has a prominence of </span><span style='color:#287fb8;font-family:Verdana;font-size:14px;'><strong>7,196</strong></span><span style='font-family:Verdana;font-size:14px;'> feet (2,193 meters).</span></p><p><a href='https://en.wikipedia.org/wiki/List_of_California_fourteeners' rel='nofollow ugc'><span style='font-family:Verdana;font-size:14px;'>More info</span></a></p>"
//    }

}
