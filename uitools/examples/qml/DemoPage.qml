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

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Esri.Examples
import Esri.ArcGISRuntime.Toolkit
import Calcite as C

Control {
    id: demoPage
    property bool handlesOwnAuthentication : false
    readonly property bool useCompactLayout: demoPage.width < 600
    enum ViewType {
        LocalScene,
        GlobalScene,
        Map
    }

    property int viewType: {
        if (mapViewContents === null) {
            if (sceneViewContents === null && localSceneViewContents !== null) {
                return DemoPage.ViewType.LocalScene;
            } else if (sceneViewContents !== null) {
                return DemoPage.ViewType.GlobalScene;
            }
        } else {
            return DemoPage.ViewType.Map;
        }
    }

    readonly property var geoView: geoViewLoader.item

    readonly property var geoModel: {
        if (geoView instanceof MapView)
            return geoView.map
        else if (geoView instanceof SceneView || geoView instanceof LocalSceneView)
            return geoView.scene;
        else
            return null;
    }

    property Component localSceneViewContents: null

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
        title: "Set access token"
        implicitWidth: 300
        contentItem: TextField {
            id: apiKeyInput
            placeholderText: "Enter your access token here."
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
            geoViewLoader.sourceComponent = Qt.binding(
                        () =>
                        viewType === DemoPage.ViewType.GlobalScene ?
                            sceneViewContents :
                            viewType === DemoPage.ViewType.LocalScene ?
                                localSceneViewContents :
                                mapViewContents
                        );
        }
    }

    Loader {
        anchors.centerIn: demoPage
        active: !handlesOwnAuthentication
        sourceComponent: Component {
            Authenticator { }
        }
    }

    contentItem: GridLayout {
        id: gridLayout
        columns: 7
        clip: true
        Button {
            Layout.leftMargin: 5
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignLeft
            text: "Select a tool"
            onClicked: showToolsButtonPressed()
            enabled: !apiKeyPopup.visible
        }

        Item {
            Layout.fillWidth: true
        }

        Item {
            Layout.fillWidth: true
        }

        ComboBox {
            id: viewTypeCombo
            Layout.topMargin: 5
            Layout.columnSpan: 3
            Layout.alignment: Qt.AlignRight
            visible: useCompactLayout
            enabled: ArcGISRuntimeEnvironment.apiKey !== ""

            model: ["Map", "Global Scene", "Local Scene"]

            TextMetrics {
                id: comboMetrics
                font: viewTypeCombo.font
                text: "Global Scene"
            }

            Layout.preferredWidth: comboMetrics.width + viewTypeCombo.indicator.width + 50

            currentIndex: {
                if (viewType === DemoPage.ViewType.Map) return 0;
                if (viewType === DemoPage.ViewType.GlobalScene) return 1;
                if (viewType === DemoPage.ViewType.LocalScene) return 2;
                return 0;
            }

            delegate: ItemDelegate {
                required property int index
                required property string modelData
                width: parent.width
                text: modelData
                enabled: [mapViewContents !== null,
                    sceneViewContents !== null,
                    localSceneViewContents !== null][index]
                highlighted: index === parent.currentIndex
            }
            onActivated: (index) => {
                             if (index === 0) viewType = DemoPage.ViewType.Map;
                             else if (index === 1) viewType = DemoPage.ViewType.GlobalScene;
                             else if (index === 2) viewType = DemoPage.ViewType.LocalScene;
                         }
        }

        RadioButton {
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignRight
            visible: !useCompactLayout
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
            visible: !useCompactLayout
            text: "Global Scene"
            checkable: true
            autoExclusive: true
            checked: viewType === DemoPage.ViewType.GlobalScene
            onClicked: viewType = DemoPage.ViewType.GlobalScene;
            enabled: ArcGISRuntimeEnvironment.apiKey !== "" && sceneViewContents !== null
        }
        RadioButton {
            Layout.topMargin: 5
            Layout.alignment: Qt.AlignRight
            visible: !useCompactLayout
            text: "Local Scene"
            checkable: true
            autoExclusive: true
            checked: viewType === DemoPage.ViewType.LocalScene
            onClicked: viewType = DemoPage.ViewType.LocalScene;
            enabled: ArcGISRuntimeEnvironment.apiKey !== "" && localSceneViewContents !== null
        }
        Button {
            id: accessTokenButton
            Layout.topMargin: 5
            Layout.rightMargin: 5
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: useCompactLayout ? 36 : -1
            Layout.preferredHeight: 36
            leftPadding: useCompactLayout ? 0 : 8
            rightPadding: useCompactLayout ? 0 : 8
            topPadding: useCompactLayout ? 0 : 8
            bottomPadding: useCompactLayout ? 0 : 8
            onClicked: apiKeyPopup.open()
            enabled: !apiKeyPopup.visible
            text: useCompactLayout ? "" : qsTr("Set access token")

            contentItem: Item {
                implicitWidth: useCompactLayout ? 24 : textLabel.implicitWidth
                implicitHeight: useCompactLayout ? 24 : textLabel.implicitHeight

                Image {
                    id: keyIcon
                    source: "images/user-key.svg"
                    width: 24
                    height: 24
                    sourceSize.width: 24
                    sourceSize.height: 24
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    visible: useCompactLayout
                    layer.enabled: true
                    layer.smooth: true
                    layer.effect: MultiEffect {
                        anchors.fill: keyIcon
                        source: keyIcon
                        colorization: 1.0
                        brightness: 1.0
                        colorizationColor: Calcite.offWhite
                        visible: true
                    }
                }

                Label {
                    id: textLabel
                    text: accessTokenButton.text
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    visible: !useCompactLayout
                }
            }
        }
        Loader {
            id: geoViewLoader;
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 7
            Layout.maximumWidth: parent.width
            focus: true
            sourceComponent: Component {
                Label {
                    text: "No access token set. Please set an access token."
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }
        }
    }
}
