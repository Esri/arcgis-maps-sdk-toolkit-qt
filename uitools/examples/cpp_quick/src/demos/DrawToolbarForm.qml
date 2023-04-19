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

import QtQuick
import QtQuick.Controls
import Esri.ArcGISRuntime
import Esri.ArcGISRuntime.Toolkit
import DemoApp

DemoPage {
    mapViewContents: Component {
        MapView {
            id: view
            Shortcut {
                context: Qt.ApplicationShortcut
                sequences: ["Ctrl+Z"]
                onActivated: drawToolbar.controller.undo()
            }
            Shortcut {
                context: Qt.ApplicationShortcut
                sequences: ["Ctrl+Y", "Ctrl+Shift+Z"]
                onActivated: drawToolbar.controller.redo()
            }
            Shortcut {
                context: Qt.ApplicationShortcut
                sequences: ["Ctrl+D", "D", StandardKey.Backspace, StandardKey.Delete]
                onActivated: drawToolbar.controller.deleteSelected()
            }
            Shortcut {
                context: Qt.ApplicationShortcut
                sequences: ["Esc"]
                onActivated: drawToolbar.controller.clearAll()
            }

            DrawToolbar {
                id: drawToolbar
                anchors {
                    right: parent.right
                    top: parent.top
                    margins: 10
                }

                mapView: view
            }

            DrawToolbarDemo {
                geoView: view
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                cursorShape: drawToolbar.mouseCursorShape
                onClicked: mouse=>{
                               if (drawToolbar.controller.isEditing) {
                                   contextMenu.popup();
                                   mouse.accepted = false;
                               }
                           }

                Menu {
                    id: contextMenu
                    MenuItem {
                        text: "Delete Selected"
                        visible: drawToolbar.controller.isEditing
                        action: Action {
                            onTriggered: {
                                drawToolbar.controller.deleteSelected();
                            }
                        }
                    }
                    MenuItem {
                        text: "Add Part"
                        visible: drawToolbar.controller.isEditing
                        action: Action {
                            onTriggered: {
                                drawToolbar.controller.addPart();
                            }
                        }
                    }
                    MenuItem {
                        text: "Undo"
                        visible: drawToolbar.controller.isEditing
                        action: Action {
                            onTriggered: {
                                drawToolbar.controller.undo();
                            }
                        }
                    }
                    MenuItem {
                        text: "Redo"
                        visible: drawToolbar.controller.isEditing
                        action: Action {
                            onTriggered: {
                                drawToolbar.controller.redo();
                            }
                        }
                    }
                    MenuItem {
                        text: "Finish"
                        visible: drawToolbar.controller.isEditing
                        action: Action {
                            onTriggered: {
                                drawToolbar.controller.save();
                            }
                        }
                    }
                }
            }
        }
    }
}
