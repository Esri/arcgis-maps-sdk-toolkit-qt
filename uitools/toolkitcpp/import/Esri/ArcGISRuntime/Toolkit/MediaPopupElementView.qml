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


Column {
    height: childrenRect.height + 10
    width: elementsView.width
    leftPadding: 10
    rightPadding: 10
    topPadding: 10
    bottomPadding: 10
    clip: true
    focus: true

    /*!
      \qmlproperty MediaPopupElementView controller
      \brief The Controller handles reading from the MediaPopupElement.

      \sa Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController
    */
    property var controller: null

    MenuSeparator {
        width: parent.width
        rightPadding: 20
    }

    Column {
        height: childrenRect.height
        width: elementsView.width - 40
        clip: true
        focus: true

        Label {
            text: controller.title !== "" ? controller.title : "Media"
            wrapMode: Text.WordWrap
            width: elementsView.width
            font.pixelSize: 20
            font.weight: Font.Black
        }
        Label {
            text: controller.description
            visible: controller.description !== ""
            wrapMode: Text.WordWrap
            width: elementsView.width
        }
    }

    MenuSeparator {
        width: parent.width
        rightPadding: 20
    }

    ListView {
        id: lv
        orientation: ListView.Horizontal
        clip: true
        focus: true
        height: 300
        width: parent.width - 20

        spacing: 10

        model: controller.popupMediaItems

        delegate: Item {
            height: 300
            width: elementsView.width - 40

            Image {
                source: model.sourceUrl
                fillMode: Image.PreserveAspectCrop
                height: 300
                width: elementsView.width - 40
                // width: popupView.width - 40
                asynchronous: true
                cache: true

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.openUrlExternally(model.linkUrl);
                    }
                }

                Component.onCompleted: print("this.height:", this, this.height)
            }

            // Overlay text
            Rectangle {
                visible: model.title !== ""
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: childrenRect.height
                color: "white"
                opacity: 0.7

                Column {
                    height: childrenRect.height
                    width: elementsView.width - 40
                    clip: true
                    focus: true
                    anchors.bottom: parent.bottom
                    leftPadding: 10
                    // rightPadding: 10

                    Label {
                        visible: model.title !== ""
                        text: model.title
                        font.weight: Font.Bold
                        // font.pointSize: 20
                        // width: elementsView.width - 40 - 20
                        width: parent.width - 20
                        // wrapMode: Text.WordWrap
                        color: "black"
                        elide: Text.ElideRight
                        // anchors.horizontalCenter: parent.horizontalCenter
                        // anchors.bottom: parent.bottom
                        // anchors.bottomMargin: 20 // Adjust margin as needed
                        // Component.onCompleted: {
                        //     print("model.title: ", model.title);
                        //     print("label.parent:" , parent.width);
                        // }
                    }

                    Label {
                        visible: model.caption !== ""
                        // visible: false
                        text: model.caption
                        font.pointSize: 15
                        // width: elementsView.width - 40 - 2
                        width: parent.width - 20
                        // wrapMode: Text.WordWrap
                        color: "black"
                        elide: Text.ElideRight
                        // anchors.horizontalCenter: parent.horizontalCenter
                        // anchors.bottom: parent.bottom
                        // anchors.bottomMargin: 20 // Adjust margin as needed
                        Component.onCompleted: {
                            print("model.caption: ", model.caption);
                        }
                    }
                }
            }
        }
    }
}

