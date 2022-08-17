/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit 100.15
import Calcite 1.0 as C

Control {
    id: demoPage
    property bool handlesOwnAuthentication : false
    enum ViewType {
        Scene,
        Map
    }

    property int viewType: {
        if (mapViewContents === null && sceneViewContents !== null) {
            return DemoPage.ViewType.Scene;
        } else {
            return DemoPage.ViewType.Map;
        }
    }

    readonly property var geoView: geoViewLoader.item

    readonly property var geoModel: {
        if (geoView instanceof MapView)
            return geoView.map
        else if (geoView instanceof SceneView)
            return geoView.scene;
        else
            return null;
    }

    property Component sceneViewContents: null

    property Component mapViewContents: null

    signal showToolsButtonPressed()

    Component.onCompleted: {
        if (ArcGISRuntimeEnvironment.apiKey === "") {
            apiKeyPopup.open();
        } else {
            apiKeyPopup.resetLoader();
        }
    }

    Keys.forwardTo: [geoView]

    BusyIndicator {
        anchors.centerIn: demoPage
        running: geoModel && geoModel.loadStatus === Enums.LoadStatusLoading
        visible: running && ArcGISRuntimeEnvironment.apiKey !== ""
    }

    Dialog {
        id: apiKeyPopup
        title: "Set API Key"
        implicitWidth: 300
        contentItem: TextField {
            id: apiKeyInput
            placeholderText: "Enter your API Key here."
            Keys.onReturnPressed: apiKeyPopup.accept()
            Component.onCompleted: forceActiveFocus()
        }
        anchors.centerIn: demoPage
        standardButtons: Dialog.Ok | Dialog.Cancel
        closePolicy: Popup.NoAutoClose
        onOpened: {
            apiKeyInput.text = ArcGISRuntimeEnvironment.apiKey;
        }
        onAccepted: {
            const oldKey = ArcGISRuntimeEnvironment.apiKey;
            ArcGISRuntimeEnvironment.apiKey = apiKeyInput.text.trim();
            if (oldKey !== ArcGISRuntimeEnvironment.apiKey) {
                resetLoader();
            }
        }
        function resetLoader() {
            geoViewLoader.sourceComponent = undefined;
            geoViewLoader.sourceComponent = Qt.binding(() => viewType === DemoPage.ViewType.Scene ? sceneViewContents : mapViewContents);
        }
    }

    Loader {
        anchors.centerIn: demoPage
        active: !handlesOwnAuthentication
        sourceComponent: Component {
            AuthenticationView { }
        }
    }

    contentItem: GridLayout {
        id: gridLayout
        columns: 6
        clip: true
        Button {
            Layout.leftMargin: 5
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignLeft
            text: "Select a tool"
            onClicked: showToolsButtonPressed()
            enabled: !apiKeyPopup.visible
        }
        Switch {
            text: "Dark Mode"
            checked: C.Calcite.theme === C.Calcite.Theme.Dark
            onCheckedChanged: C.Calcite.theme = checked ? C.Calcite.Theme.Dark : C.Calcite.Theme.Light
        }

        Item {
            Layout.fillWidth: true
        }
        RadioButton {
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignRight
            text: "Map"
            checkable: true
            autoExclusive: true
            checked: viewType === DemoPage.ViewType.Map
            onClicked: viewType = DemoPage.ViewType.Map
            enabled: ArcGISRuntimeEnvironment.apiKey !== "" && mapViewContents !== null
        }
        RadioButton {
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignRight
            text: "Scene"
            checkable: true
            autoExclusive: true
            checked: viewType === DemoPage.ViewType.Scene
            onClicked: viewType = DemoPage.ViewType.Scene;
            enabled: ArcGISRuntimeEnvironment.apiKey !== "" && sceneViewContents !== null
        }
        Button {
            Layout.topMargin: 5
            Layout.rightMargin: 5
            text: "Set API Key"
            Layout.alignment: Qt.AlignRight
            onClicked: apiKeyPopup.open()
            enabled: !apiKeyPopup.visible
        }
        Loader {
            id: geoViewLoader;
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 6
            Layout.maximumWidth: parent.width
            focus: true
            sourceComponent: Component {
                Label {
                    text: "No API key set. Please set an API Key."
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }
        }
    }
}
