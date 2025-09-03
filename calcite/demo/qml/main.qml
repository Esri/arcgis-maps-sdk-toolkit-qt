
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

    CalciteTest {
        id: model
        mapView: mapView
    }

    Component.onCompleted: {

        //Calcite.theme = Calcite.Dark;
    }

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true


        Pane {
            anchors {
                right: parent.right
                top: parent.top
            }
            ColumnLayout {
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
            anchors.centerIn: parent
            standardButtons: Dialog.Ok | Dialog.Cancel | Dialog.Help
            implicitWidth: 400
            implicitHeight: 300
            Pane {
                anchors.fill: parent
                contentItem: ListView {
                    clip: true
                    anchors.fill: parent
                    model: ["Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6"]
                    ScrollBar.vertical: ScrollBar {}
                    delegate: RadioDelegate {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        text: modelData
                    }
                }
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
            ColumnLayout {
                TextField {
                    placeholderText: "Textfield"
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    placeholderText: "Numbers 0-10"
                    Layout.alignment: Qt.AlignHCenter
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
                    anchors.horizontalCenter: parent.horizontalCenter
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
                    value: 5
                }
            }
        }

        ToolBar {
            enabled: enabler.checked
            anchors {
                right: parent.right
                bottom: parent.attributionTop
                margins: 5
            }
            ColumnLayout {

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
