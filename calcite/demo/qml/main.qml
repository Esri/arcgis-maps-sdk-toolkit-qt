/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
import QtQuick
import QtQuick.Controls
import Esri.Calcite
import Calcite as C
import QtQuick.Layouts

ApplicationWindow {
    id: appWindow
    width: 800
    height: 800
    title: "Calcite_test"
    visible: true
    visibility: Window.Maximized

    CalciteTest {
        id: model
        mapView: mapView
    }

    // Check if Calcite icons submodule is initialized
    property bool calciteIconsAvailable: {
        // Check if icon resources are available
        var testPath = "qrc:/calcite/icons/check-24.svg"
        return Qt.resolvedUrl(testPath).toString().length > 0
    }

    readonly property int topInset: Qt.platform.os === "android" ? 40 : 0
    readonly property int bottomInset: Qt.platform.os === "android" ? 30 : 0
    Item {
        id: safeArea
        anchors {
            fill: parent
            topMargin: topInset
            bottomMargin: bottomInset
        }
        // add a mapView component
        MapView {
            id: mapView
            anchors.fill: parent
            // set focus to enable keyboard navigation
            focus: true

            Pane {
                id: topRightPane
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 5
                }
                z: 100

                implicitWidth: themeLayout.implicitWidth + leftPadding + rightPadding
                implicitHeight: themeLayout.implicitHeight + topPadding + bottomPadding
                // Scale to fit smaller screens
                scale: Math.min(1.0,
                                Math.min(mapView.width * 0.3 / Math.max(implicitWidth, 1),
                                         mapView.height * 0.3 / Math.max(implicitHeight, 1)))
                transformOrigin: Item.TopRight

                background: Rectangle {
                    color: C.Calcite.background
                    border.color: C.Calcite.border1
                    border.width: 1
                }

                ColumnLayout {
                    id: themeLayout
                    Switch {
                        id: themeSwitch
                        text: checked ? "Dark Mode" : "Light Mode"
                        Layout.alignment: Qt.AlignRight
                        enabled: !C.Calcite.useSystemTheme
                        property var selectedTheme: themeSwitch.checked ? C.Calcite.Dark : C.Calcite.Light

                        Binding {
                            target: C.Calcite
                            property: "theme"
                            value: themeSwitch.enabled ? themeSwitch.selectedTheme : Application.styleHints.colorScheme
                        }


                    }
                    CheckBox {
                        id: enabler
                        text: "Enabled"
                        checked: true
                    }
                    CheckBox {
                        id: useSystemThemeCheckBox
                        text: "Use System Theme"
                        checked: true

                        Binding {
                            target: C.Calcite
                            property: "useSystemTheme"
                            value: useSystemThemeCheckBox.checked
                        }
                        Binding {
                            target: C.Calcite
                            property: "theme"
                            value: useSystemThemeCheckBox.checked ? Application.styleHints.colorScheme :
                                                                    themeSwitch.selectedTheme
                        }
                    }
                }
            }

            Dialog {
                id: dialog
                title: "Dialog"
                parent: Overlay.overlay
                anchors.centerIn: parent
                modal: true
                standardButtons: Dialog.Ok | Dialog.Cancel | Dialog.Help
                width: Math.min(400, appWindow.width * 0.85)
                height: Math.min(300, appWindow.height * 0.85)

                // Black tinted background
                Overlay.modal: Rectangle {
                    color: "#80000000"
                }
                Pane {
                    anchors.fill: parent
                    clip: true
                    contentItem: ListView {
                        clip: true
                        anchors.fill: parent
                        model: ["Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6"]
                        ScrollBar.vertical: ScrollBar {}
                        delegate: RadioDelegate {
                            width: ListView.view.width
                            text: modelData
                            // Prevent text overflow
                            clip: true
                        }
                    }
                }
            }

            Dialog {
                id: iconShowcaseDialog
                parent: Overlay.overlay
                anchors.centerIn: parent
                modal: true
                standardButtons: Dialog.Close
                width: Math.min(600, appWindow.width * 0.9)
                height: Math.min(500, appWindow.height * 0.9)

                // Black tinted background
                Overlay.modal: Rectangle {
                    color: "#80000000"
                }

                // Only available if submodule is initialized
                property bool iconsAvailable: {
                    // Try to load an icon to check if submodule is initialized
                    var testImage = Qt.createQmlObject('import QtQuick; Image { source: "qrc:/calcite/icons/check-24.svg" }', iconShowcaseDialog, "testImage")
                    var available = testImage.status === Image.Ready || testImage.status === Image.Loading
                    testImage.destroy()
                    return available
                }

                IconShowcase {
                    anchors.fill: parent
                    visible: iconShowcaseDialog.iconsAvailable
                    submoduleInitialized: iconShowcaseDialog.iconsAvailable
                }

                Label {
                    anchors.centerIn: parent
                    width: Math.min(implicitWidth, parent.width - 40)
                    visible: !iconShowcaseDialog.iconsAvailable
                    text: "Icons not available.\n\nInitialize the submodule and rebuild:\ngit submodule update --init --recursive"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    clip: true
                }
            }

            Drawer {
                id: drawer
                width: 0.66 * mapView.width
                height: mapView.height
                Column {
                    StackLayout {
                        id: stackLayout

                        PageIndicator {
                            currentIndex: stackLayout.currentIndex
                            count: stackLayout.count
                            interactive: true
                        }

                        Page {}
                        Page {// ...
                        }
                        Page {// ...
                        }
                    }
                }
            }

            Menu {
                id: contextMenu
                MenuItem {
                    text: "Cut"
                }
                MenuItem {
                    text: "Copy"
                }
                MenuSeparator {}
                MenuItem {
                    text: "Paste"
                }
            }

            Frame {
                id: pane
                enabled: enabler.checked
                anchors.centerIn: parent
                padding: 20

                implicitWidth: columnLayout.implicitWidth + leftPadding + rightPadding
                implicitHeight: columnLayout.implicitHeight + topPadding + bottomPadding

                scale: Math.min(1.0,
                                Math.min(mapView.width * 0.8 / Math.max(implicitWidth, 1),
                                         mapView.height * 0.85 / Math.max(implicitHeight, 1)))
                transformOrigin: Item.Center

                ColumnLayout {
                    id: columnLayout
                    width: Math.min(implicitWidth, parent.availableWidth)
                    spacing: 8

                    TextField {
                        placeholderText: "Textfield"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.maximumWidth: 160
                    }

                    TextField {
                        placeholderText: "Numbers 0-10"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.maximumWidth: 160
                        validator: IntValidator {
                            bottom: 0
                            top: 10
                        }
                    }
                    Button {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Open Dialog"
                        onPressed: {
                            dialog.visible = true
                        }
                    }
                    Button {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Open Menu"
                        flat: true
                        onPressed: {
                            contextMenu.popup()
                        }
                    }
                    RoundButton {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Round button "
                    }
                    RoundButton {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Round button flat"
                        flat: true
                    }
                    GroupBox {
                        title: "Group box"
                        RowLayout {
                            RadioButton {
                                text: "Radio button 1"
                                checked: true
                            }
                            RadioButton {
                                text: "Radio button 2"
                            }
                        }
                    }
                    Column {
                        spacing: 4
                        topPadding: 4
                        bottomPadding: 4
                        Layout.alignment: Qt.AlignHCenter
                        CheckBox {
                            id: parentCheck
                            text: "Root Checkbox"
                            checkState: {
                                if (child1.checked && child2.checked) {
                                    return Qt.Checked
                                } else if (!child1.checked && !child2.checked) {
                                    return Qt.Unchecked
                                } else {
                                    return Qt.PartiallyChecked
                                }
                            }

                            onClicked: {
                                let newState = checkState
                                child1.checked = newState
                                child2.checked = newState
                            }
                        }
                        Column {
                            spacing: 4
                            leftPadding: 32
                            anchors.horizontalCenter: parent.horizontalCenter

                            CheckBox {
                                id: child1
                                text: "Checkbox 1"
                                checked: false
                                onClicked: parentCheck.forceActiveFocus()
                            }

                            CheckBox {
                                id: child2
                                text: "Checkbox 2"
                                checked: true
                                onClicked: parentCheck.forceActiveFocus()
                            }
                        }
                    }
                    ComboBox {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.maximumWidth: 160
                        model: ["Banana", "Apple", "Coconut"]
                    }

                    Switch {
                        text: "Switch"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        Layout.alignment: Qt.AlignHCenter
                    }

                    RangeSlider {
                        Layout.alignment: Qt.AlignHCenter
                    }

                    BusyIndicator {
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Label {
                        text: "Loading..."
                        Layout.alignment: Qt.AlignHCenter
                    }
                    SpinBox {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.maximumWidth: 160
                        value: 5

                    }
                    Label {
                        text: "Progress Bar"
                    }
                    ProgressBar {
                        Layout.fillHeight: false
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: 50
                        clip: true
                        indeterminate: true
                    }
                }

            }

            // Icon Showcase Button - Bottom Left
            Button {
                id: iconButton
                anchors {
                    left: parent.left
                    bottom: parent.attributionTop
                    margins: 10
                }
                text: "Calcite Icons"
                visible: appWindow.calciteIconsAvailable
                enabled: appWindow.calciteIconsAvailable

                // Scale to fit smaller screens
                scale: Math.min(1.0,
                                Math.min(mapView.width * 0.3 / Math.max(implicitWidth, 1),
                                         mapView.height * 0.15 / Math.max(implicitHeight, 1)))
                transformOrigin: Item.BottomLeft

                onPressed: {
                    iconShowcaseDialog.open()
                }
            }

            Pane {
                id: bottomRightToolbar
                enabled: enabler.checked
                anchors {
                    right: parent.right
                    bottom: parent.attributionTop
                    margins: 5
                }

                implicitWidth: toolbarLayout.implicitWidth + leftPadding + rightPadding
                implicitHeight: toolbarLayout.implicitHeight + topPadding + bottomPadding

                scale: Math.min(1.0,
                                Math.min(mapView.width * 0.3 / Math.max(implicitWidth, 1),
                                         mapView.height * 0.3 / Math.max(implicitHeight, 1)))
                transformOrigin: Item.BottomRight

                background: Rectangle {
                    color: C.Calcite.background
                    border.color: C.Calcite.border1
                    border.width: 1
                }

                ColumnLayout {
                    id: toolbarLayout

                    ToolButton {
                        icon.source: "qrc:/esri.com/imports/Calcite/images/check.svg"
                        checkable: true
                        text: "Checkable ToolButton"
                    }

                    ToolSeparator {
                        Layout.fillWidth: true
                        orientation: Qt.Horizontal
                    }

                    ToolButton {
                        Layout.fillWidth: true
                        flat: true
                        checkable: true
                        text: "Regular Flat ToolButton "
                    }
                }
            }
        }
    }
}
