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
    property var popupElement: null


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
        function onPopupElementChanged() {
            let component = textPopupElementView;
            if (component.status === Component.Ready) {
                let view = component.createObject(dynamicViewContainer);
                if (view) {
                    view.popupElement = controller.popupElement;
                }
            } else {
                console.error("Error loading component:", component.errorString());
            }
        }

        function onPopupChanged() {
            for ( let i = dynamicViewContainer.children.length; i > 0; i--) {
                dynamicViewContainer.children[i-1].destroy();
            }
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

    ScrollView {
        id: sv
        anchors.fill: parent
        clip: true
        Column {
            id: dynamicViewContainer
            anchors.fill: parent
            spacing: 5
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
