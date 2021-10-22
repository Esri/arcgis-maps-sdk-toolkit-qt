// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.12
import Calcite 1.0 as C
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "Calite_test"
    visible: true

    Component.onCompleted: {
        //Calcite.theme = Calcite.Dark;
    }

    // add a mapView component
    MapView {
        id: mapView
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true

        // add a map to the mapview
        Map {
            // add the ArcGISStreets basemap to the map
            BasemapImagery { }
        }

        Pane {
            anchors.right: parent.right
            anchors.top: parent.to
            ColumnLayout {
                Switch {
                    id: themeSwitch
                    text: checked ? "Dark Mode" : "Light Mode"
                    Layout.alignment: Qt.AlignRight
                    Binding {
                        target: C.Calcite
                        property: "theme"
                        value: themeSwitch.checked ? C.Calcite.Dark : C.Calcite.Light
                    }
                }
                CheckBox {
                    id: enabler
                    text: "Enabled"
                    checked: true
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

                    Page {
                    }
                    Page {
                        // ...
                    }
                    Page {
                        // ...
                    }
                }
            }
        }

        Menu {
            id: contextMenu
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuSeparator { }
            MenuItem { text: "Paste" }
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
                CheckBox {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Check box 1"
                }
                CheckBox {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Check box 2"
                    checkState: Qt.PartiallyChecked
                }
                CheckBox {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Check box 3"
                    checkState: Qt.Checked
                }
                ComboBox {
                    Layout.alignment: Qt.AlignHCenter
                    model: [ "Banana", "Apple", "Coconut" ]
                }

                Switch {
                    text: "Switch"
                    Layout.alignment: Qt.AlignHCenter
                }


//                Slider {
//                    Layout.alignment: Qt.AlignHCenter
//                }

                ProgressBar {
                    id: progressBar
                    Layout.alignment: Qt.AlignHCenter
                    PropertyAnimation {
                        target: progressBar
                        property: "value"
                        from: 0
                        to: 1
                        running: true
                        duration: 5000
                    }
                }

                ProgressBar {
                    Layout.alignment: Qt.AlignHCenter
                    value: 50
                    indeterminate: true
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: "Loading..."
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }
}

