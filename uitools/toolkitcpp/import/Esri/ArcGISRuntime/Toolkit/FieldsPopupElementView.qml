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

// Item {
//     property var controller: null
//     Flickable {
//         id: flickable
//         clip: true
//         // contentHeight: fieldsLayout.height
//         anchors.top: parent.top
//         anchors.right: parent.right
//         anchors.bottom: parent.bottom
//         width: 300


//         ColumnLayout {
//             id: fieldsLayout
//             anchors {
//                 left: parent.left
//                 right: parent.right
//             }

//             // We must account for what is visible, including title headers as rows.
//             // Field names
//             Repeater {
//                 model: ListModel {
//                     ListElement { label: "Test1" }
//                     ListElement { label: "Test2" }
//                     ListElement { label: "Test3" }
//                     ListElement { label: "Test4" }
//                     ListElement { label: "Test5" }
//                 }
//                 Label {
//                     // text: label ?? fieldName ?? ""
//                     // Layout.maximumWidth: flickable.width / 2
//                     // wrapMode: Text.Wrap
//                     // font: popupView.font
//                     Label {
//                         id: testText
//                         text: model.label ? model.label : ""
//                         Layout.maximumWidth: parent.width
//                         padding: 10
//                     }
//                 }
//             }
//         }
//     }
// }

// Item {
//     property var controller: null
//     // height: fieldsPopupElementView.height
//     // width: parent.width
//     anchors.fill: parent

// works with generic LM
ListView {
    // id: lview
    id: fieldsPopupElementView

    // height: 600
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
    }

    delegate: Column {
        width: parent.width
        // anchors.margins: 50
        // Layout.maximumWidth: parent.width
            Label {
                // id: testText
                text: modelData["labels"]/* ? model.label : ""*/
                wrapMode: Text.WordWrap
                textFormat: Text.AutoText
                width: parent.width
                // anchors.fill: parent
                // padding: 5
            }
            Label {
                // id: testText
                text: modelData["formattedValues"]/* ? model.label : ""*/
                wrapMode: Text.WordWrap
                textFormat: Text.AutoText
                width: parent.width
                Component.onCompleted: {
                    print(parent);
                    print("LabelPwidth: " + parent.width);
                    // print("LabelPheight: " + parent.height);
                }
                // anchors.fill: parent
                // padding: 5
            }
            Component.onCompleted: {
                // print("delegate - Pwidth: " + parent.width);
                print(parent);
            }
    }

    Component.onCompleted: {
        // print(fieldsPopupElementView.height);
        // fieldsPopupElementView.height = 600;
    }

    onContentHeightChanged: {
        print("onContentHeightChanged: " + fieldsPopupElementView.contentHeight)
    }
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
