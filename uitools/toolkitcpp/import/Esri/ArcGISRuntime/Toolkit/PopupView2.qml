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
    id: popupView2

    /*!
       \brief The PopupManager that controls the information being displayed in
       the view.

       The PopupManager should be created from a Popup.
       \qmlproperty PopupManager popupManager
     */
    property var popup: null
//    property var popupElement: null


    property var controller: PopupViewController2 {}

    /*!
       \brief Callback function called when the close button is clicked. When
       this property is set to null the close button does not render. When
       the close button is clicked the function in this property is called.
       Defaults to setting visible to false.
     */
    property var closeCallback: function() {
        popupView.visible = false;
    }

    Connections {
        target: controller
        function onPopupChanged() {
            print("controller.popupElements.count: " + controller.popupElements.count);
        }
    }

    Binding {
        target: controller
        property: "popup"
        value: popupView2.popup
    }

    implicitWidth: 300 + padding

    implicitHeight: 300 + padding

    spacing: 5
    leftPadding: popupView2.spacing
    rightPadding: popupView2.spacing

    title: controller.title

    // Title Header
    header: Label {
        textFormat: Text.StyledText
        text: `<h2>${popupView.title}</h2>`
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        font: popupView.font
        leftPadding: popupView.spacing
        rightPadding: popupView.spacing
    }


    ListView {
        id: elementsView
        width: parent.width
        height: parent.height
        model: controller.popupElements
        spacing: 10

//        delegate: ItemDelegate {
        delegate: Item {
            width: ListView.view.width
            height: 210
                TextPopupElementView
                {
                    anchors.fill: parent
                    popupElement: listModelData.popupElement
//                    popupElement: listModelData.popupElementType === 0 ? listModelData.popupElement : null
                    width: parent.width
                }
                Component.onCompleted: {
                    print("height: " + height);
                }

//                MouseArea {
//                    anchors.fill: parent
//                    drag.target: parent

//                    onWheel: (wheel) => {
//                        // Calculate new contentY
//                        var newContentY = elementsView.contentY - wheel.angleDelta.y;

//                        // Prevent overscrolling
//                        if (newContentY < 0) {
//                            newContentY = 0; // Prevent scrolling above the top
//                        } else if (newContentY > elementsView.contentHeight - elementsView.height) {
//                            newContentY = elementsView.contentHeight - elementsView.height; // Prevent scrolling below the bottom
//                        }

//                        // Set the new contentY
//                        elementsView.contentY = newContentY;

//                        // Prevent default handling to allow ListView to scroll
//                        wheel.accepted = true;
//                    }

//                    onClicked: (mouse) => {
//                        // Allow click events to propagate
//                        mouse.accepted = false;
//                    }
//                }
            }
    }


    footer: ColumnLayout {
        visible: popupView.closeCallback
        Button {
            text: "Close"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                if (popupView.closeCallback)
                    popupView.closeCallback()
            }
            Layout.bottomMargin: popupView.spacing
        }
    }

    Component {
        id: textPopupElementView
        TextPopupElementView {
            width: parent.width
        }
    }
}
