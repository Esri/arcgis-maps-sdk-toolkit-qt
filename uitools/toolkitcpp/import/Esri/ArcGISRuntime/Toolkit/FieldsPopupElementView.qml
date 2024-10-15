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
import QtQuick.Layouts

ListView {
    id: fieldsPopupElementView

    height: contentHeight

    property var controller: null
    model: controller.values
    // model: ListModel {
    //     ListElement { label: "Test1" }
    //     ListElement { label: "Test2" }
    //     ListElement { label: "Test3" }
    //     ListElement { label: "Test4" }
    //     ListElement { label: "Test5" }
    // }
    clip: true
    focus: true

    spacing: 10

    header: Label {
        text: controller.title !== "" ? controller.title : "Fields"
        wrapMode: Text.WordWrap
        textFormat: Text.AutoText
        width: parent.width
        font.pixelSize: 20
        rightPadding: 10
        leftPadding: 10
    }

    delegate: Column {
        width: parent.width

            MenuSeparator {
                width: parent.width
                leftPadding: 20
            }
            Label {
                // id: testText
                text: modelData["labels"]
                wrapMode: Text.WordWrap
                textFormat: Text.AutoText
                width: parent.width
                color: "grey"
                font.pixelSize: 15
                rightPadding: 10
                leftPadding: 20
            }
            Label {
                // id: testText
                text: modelData["formattedValues"]
                wrapMode: Text.WordWrap
                textFormat: Text.AutoText
                width: parent.width
                rightPadding: 10
                leftPadding: 20
                Component.onCompleted: {
                    if (text.startsWith("http")) {
                        let link = text;
                        text = `<a href="${link}" target="_blank">View</a>`;
                        textFormat = Text.RichText;
                    }
                }
                onLinkActivated: (link) => {
                    if (link !== "" && link !== undefined) {
                        Qt.openUrlExternally(link);
                    }
                }
                    // print(parent);
                    // print("LabelPwidth: " + parent.width);
            }

    }

    Component.onCompleted: {
        // print(fieldsPopupElementView.height);
        // fieldsPopupElementView.height = 600;
    }

    onContentHeightChanged: {
        print("onContentHeightChanged: " + fieldsPopupElementView.contentHeight)
    }

    // MouseArea {
    //     anchors.fill: parent
    //     onWheel: (wheel) => {
    //         // console.log("Mouse wheel:", wheel.angleDelta.y)
    //         // Emit a signal or call a method on the outer ListView
    //         // if (wheel.angleDelta.y < 0) {
    //         //     // Scroll outer down
    //         //     elementsView.scroll(wheel.angleDelta.y);
    //         // } else {
    //         //     // Scroll outer up
    //         //     elementsView.scroll(wheel.angleDelta.y);
    //         // }
    //         elementsView.scroll((-1) * wheel.angleDelta.y);
    //     }
    // }
}
// }

// Item {
//     id: fieldsPopupElementView

//     /*!
//       \qmlproperty FieldsPopupElementView controller
//       \brief The Controller handles reading from the FieldsPopupElement.

//       The CPP controller is documented \l{Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController}{here}.
//     */



//     property var controller: null

//     // implicitHeight: lview.implicitHeight
//     // implicitHeight: childrenRect.height
//     // implicitHeight: 300
//     height: lview.height

//     ListView {
//         id: lview
//         // model: controller.fieldsLM ? controller.fieldsLM : {}
//         model: ListModel {
//                     ListElement { label: "Test1" }
//                     ListElement { label: "Test2" }
//                     ListElement { label: "Test3" }
//                     ListElement { label: "Test4" }
//                     ListElement { label: "Test5" }
//                 }
//         spacing: 10
//         height: contentHeight
//         clip: true
//         anchors.fill: parent
//         // contentHeight: fieldsLayout.height
//         // anchors.top: parent.top
//         // anchors.right: parent.right
//         // anchors.bottom: parent.bottom
//         // width: 300

//         onModelChanged: {
//             print("ModelChanged")
//             // print(controller.fieldsLM.size());
//         }

//         // clip: true
//         delegate: Item {
//             height: testText.implicitHeight
//             Label {
//                 id: testText
//                 text: model.label ? model.label : ""
//                 anchors.fill: parent
//                 padding: 10
//             }
//         }

//         Component.onCompleted: print(lview.height);
//     }

//     // Rectangle {
//     //     anchors.top: parent.top
//     //     anchors.right: parent.right
//     //     anchors.bottom: parent.bottom
//     //     width: 300
//     //     color: "black"
//     //     ListView {
//     //         id: lview
//     //         // model: controller.fieldsLM ? controller.fieldsLM : {}
//     //         anchors.fill: parent
//     //         model: testModel
//     //         spacing: 10
//     //         height: contentHeight
//     //         // clip: true
//     //         delegate: Item {
//     //             height: testText.implicitHeight
//     //             Label {
//     //                 id: testText
//     //                 text: model.label ? model.label : ""
//     //                 anchors.fill: parent
//     //                 padding: 10
//     //             }
//     //         }
//     //     }
//     // }
// }
