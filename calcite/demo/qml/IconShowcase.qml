/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
import QtQuick.Layouts
import Esri.Calcite
import Calcite as C

Pane {
    id: root

    property bool submoduleInitialized

    property bool canOpenLocalFolders: Qt.platform.os === "windows" ||
                                       Qt.platform.os === "osx" ||
                                       Qt.platform.os === "linux"

    property var icons: !root.submoduleInitialized? [] :[
                                                        // Arrows
                                                        { name: "chevron-left", path: "qrc:/calcite/icons/chevron-left-24.svg", category: "Arrows" },
                                                        { name: "chevron-right", path: "qrc:/calcite/icons/chevron-right-24.svg", category: "Arrows" },
                                                        { name: "chevron-up", path: "qrc:/calcite/icons/chevron-up-24.svg", category: "Arrows" },
                                                        { name: "chevron-down", path: "qrc:/calcite/icons/chevron-down-24.svg", category: "Arrows" },
                                                        { name: "arrow-left", path: "qrc:/calcite/icons/arrow-left-24.svg", category: "Arrows" },
                                                        { name: "arrow-right", path: "qrc:/calcite/icons/arrow-right-24.svg", category: "Arrows" },

                                                        // Generic
                                                        { name: "save", path: "qrc:/calcite/icons/save-24.svg", category: "Generic" },
                                                        { name: "ellipsis", path: "qrc:/calcite/icons/ellipsis-24.svg", category: "Generic" },
                                                        { name: "home", path: "qrc:/calcite/icons/home-24.svg", category: "Generic" },

                                                        // Objects
                                                        { name: "trash", path: "qrc:/calcite/icons/trash-24.svg", category: "Objects" },
                                                        { name: "pencil", path: "qrc:/calcite/icons/pencil-24.svg", category: "Objects" },
                                                        { name: "copy", path: "qrc:/calcite/icons/copy-24.svg", category: "Objects" },
                                                        { name: "gear", path: "qrc:/calcite/icons/gear-24.svg", category: "Objects" },

                                                        // Symbols
                                                        { name: "plus", path: "qrc:/calcite/icons/plus-24.svg", category: "Symbols" },
                                                        { name: "minus", path: "qrc:/calcite/icons/minus-24.svg", category: "Symbols" },
                                                        { name: "x", path: "qrc:/calcite/icons/x-24.svg", category: "Symbols" },
                                                        { name: "check", path: "qrc:/calcite/icons/check-24.svg", category: "Symbols" },
                                                        { name: "search", path: "qrc:/calcite/icons/search-24.svg", category: "Symbols" },
                                                        { name: "information", path: "qrc:/calcite/icons/information-24.svg", category: "Symbols" },
                                                        { name: "question", path: "qrc:/calcite/icons/question-24.svg", category: "Symbols" },

                                                        // GIS/Layers
                                                        { name: "map", path: "qrc:/calcite/icons/map-24.svg", category: "GIS" },
                                                        { name: "pin", path: "qrc:/calcite/icons/pin-24.svg", category: "GIS" },
                                                        { name: "layer", path: "qrc:/calcite/icons/layer-24.svg", category: "Layers" }

                                                    ]

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            text: qsTr("Calcite Icons Showcase")
            font.pixelSize: 20
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: qsTr("Sample Icons from the Calcite Design System")
            font.pixelSize: 12
            opacity: 0.7
            Layout.alignment: Qt.AlignHCenter
        }

        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            GridLayout {
                id: iconGrid
                width: scrollView.availableWidth
                // Calculate columns based on actual available width
                columns: {
                    const itemWidth = 120
                    const spacing = 15
                    const availableWidth = iconGrid.width
                    const cols = Math.floor(availableWidth / itemWidth)
                    return Math.max(1, Math.min(cols, 4))
                }
                rowSpacing: 15
                columnSpacing: 15

                Repeater {
                    model: root.icons

                    Pane {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 100

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            Button {
                                icon.source: modelData.path
                                icon.width: 32
                                icon.height: 32
                                icon.color: C.Calcite.theme === C.Calcite.Dark ? "#ffffff" : "#151515"
                                flat: true
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Label {
                                text: modelData.name
                                font.pixelSize: 10
                                horizontalAlignment: Text.AlignHCenter
                                Layout.alignment: Qt.AlignHCenter
                                Layout.preferredWidth: parent.width
                                wrapMode: Text.WordWrap
                            }

                            Label {
                                text: modelData.category
                                font.pixelSize: 8
                                opacity: 0.6
                                horizontalAlignment: Text.AlignHCenter
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: parent.width * 0.8
            spacing: 10

            Button {
                text: qsTr("View All Icons Online")
                icon.source: root.submoduleInitialized? "qrc:/calcite/icons/web-24.svg" : ""
                icon.color: "#ffffff"
                Layout.fillWidth: true
                onClicked: {
                    Qt.openUrlExternally("https://developers.arcgis.com/calcite-design-system/icons/")
                }
            }

            Button {
                text: qsTr("Open Icons Folder")
                icon.source: root.submoduleInitialized? "qrc:/calcite/icons/folder-24.svg" : ""
                icon.color: "#ffffff"
                Layout.fillWidth: true
                visible: root.canOpenLocalFolders
                onClicked: {
                    var appPath = Qt.application.arguments[0].replace(/\\/g, "/")
                    var iconsPath
                    var fileUrl

                    if (Qt.platform.os === "windows") {
                        var calciteIndex = appPath.lastIndexOf("/calcite/")
                        if (calciteIndex === -1) {
                            console.log("Could not find calcite folder in path")
                            return
                        }
                        var calcitePath = appPath.substring(0, calciteIndex + 8)
                        iconsPath = calcitePath + "/calcite-design-system/packages/ui-icons/icons/"
                        fileUrl = "file:///" + iconsPath
                    } else {
                        var appDir = appPath.substring(0, appPath.lastIndexOf("/"))
                        var parts = appDir.split("/")
                        parts = parts.slice(0, -6)
                        var basePath = parts.join("/")
                        iconsPath = basePath + "/calcite-design-system/packages/ui-icons/icons/"
                        fileUrl = encodeURI("file://" + iconsPath)
                    }
                    Qt.openUrlExternally(fileUrl)
                }
            }

        }

        Label {
            text: qsTr("23 sample icons shown • 1,000+ icons available in submodule")
            font.pixelSize: 10
            opacity: 0.5
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
