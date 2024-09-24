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

Item {
    id: textPopupElementView
    property var popupElement: null

    property var controller: TextPopupElementViewController {}


    Binding {
        target: controller
        property: "popupElement"
        value: textPopupElementView.popupElement
    }

//    implicitWidth: richText.width
//    implicitHeight: richText.height

    WebView {
        id: webView
        width: parent.width
        implicitHeight: 100 + padding

        // Property to get HTML content when necessary.(Remove?)
        property string html: ""

        onLoadingChanged: (loadRequest) => {
        if (loadRequest.status === WebView.LoadSucceededStatus) {
            webView.runJavaScript("document.body.scrollHeight", function(result) {
                // Set the height of the WebView based on the content height
                // this works for Switch but seems problematic in WebView
                webView.height = result;
            });
                readHtmlContent();
            } else if (loadRequest.status === WebView.LoadFailedStatus) {
                print("Failed to load");
            }
        }

        onUrlChanged: {
            // If a link is clicked in the html content, open the link in the default browser
            // then reload the original html content
            if (webView.url.toString() !== "" && webView.url.toString().startsWith("http")) {
                Qt.openUrlExternally(webView.url.toString());
                webView.loadHtml(controller.text);
            }
        }

        function readHtmlContent() {
            const js = "document.documentElement.outerHTML";
            webView.runJavaScript(js, function(result) { html = result; });
        }

        Component.onCompleted: {
            loadHtml(controller.text);
        }
    }

//    // WebView backup
//        Text {
//            id: richText
//            text: controller.text
//            textFormat: Text.RichText
//            wrapMode: Text.WordWrap
//            width: parent.width

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    // Regular expression to find links
//                    var regex = /href='(https?:\/\/[^\s']+)'/g;
//                    var match = regex.exec(richText.text);

//                    // Check if a link was clicked
//                    if (match) {
//                        Qt.openUrlExternally(match[1]); // Open the first matched link
//                    }
//                }
//            }
//        }

//        implicitHeight: richText.implicitHeight
//        Rectangle {
//            color: "white" // Set the background color to white
//            anchors.fill: parent
//            //    WebView backup
//            Text {
//                id: richText
//                text: controller.text
//                textFormat: Text.RichText
//                wrapMode: Text.WordWrap
//                //            width: parent.width
//                anchors.fill: parent

//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        // Regular expression to find links
//                        let link = richText.hoveredLink;

//                        // Check if a link was clicked
//                        if (link !== "" && link !== undefined) {
//                            Qt.openUrlExternally(link); // Open the first matched link
//                        }
//                    }
//                }
//            }
//        }
}
