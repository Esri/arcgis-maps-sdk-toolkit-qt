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
import QtQml
import QtQuick.Controls
import QtQuick.Layouts

import Esri.ArcGISRuntime.Toolkit
import Esri.ArcGISRuntime.Toolkit.Controller

/*!
   \qmltype DrawToolbar
   \ingroup ArcGISQtToolkit
   \ingroup ArcGISQtToolkitUiQmlViews
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 200.1
   \brief TODO
 */
Control {
    id: drawToolbar

    /*!
     \qmlproperty MapView mapView
     \brief TODO
     */
    property var mapView;

    /*!
     \qmlproperty DrawToolbarController controller
      \brief TODO
     */
    property var controller: DrawToolbarController {
        onDrawModeChanged: {
            selectButton.checked = drawMode === DrawToolbarController.DrawMode.SelectDrawMode;
            pointButton.checked = drawMode === DrawToolbarController.DrawMode.PointDrawMode;
            lineButton.checked = drawMode === DrawToolbarController.DrawMode.PolylineDrawMode;
            polygonButton.checked = drawMode === DrawToolbarController.DrawMode.PolygonDrawMode;
            freehandLineButton.checked = drawMode === DrawToolbarController.DrawMode.FreehandLineDrawMode;
            freehandAreaButton.checked = drawMode === DrawToolbarController.DrawMode.FreehandPolygonDrawMode;

            determineMouseCursor();
        }

        onSelectedElementTypeChanged: {
            determineMouseCursor();
        }
    }

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

    property var mouseCursorShape: Qt.ArrowCursor

    /*
Future enhancement for resize:
- Resize height - Qt.UpArrowCursor
- Resize width - Qt.SizeHorCursor
- Resize diagonal top right or bottom left - Qt.SizeBDiagCursor
- Resize diagonal top left or bottom right - Qt. SizeFDiagCursor
*/
    function determineMouseCursor() {
        if (controller.drawMode === DrawToolbarController.DrawMode.SelectDrawMode) {
            mouseCursorShape = Qt.ArrowCursor;
            return;
        }

        if (controller.selectedElementType === DrawToolbarController.SelectedElementType.MidVertex){
            mouseCursorShape = Qt.DragCopyCursor;
        } else if (controller.selectedElementType === DrawToolbarController.SelectedElementType.Vertex){
            mouseCursorShape = Qt.CrossCursor;
        } else if (controller.selectedElementType === DrawToolbarController.SelectedElementType.Geometry){
            mouseCursorShape = Qt.SizeAllCursor;
        } else if (controller.selectedElementType === DrawToolbarController.SelectedElementType.Part){
            mouseCursorShape = Qt.SizeAllCursor;
        } else if (controller.selectedElementType === DrawToolbarController.SelectedElementType.None){
            mouseCursorShape = Qt.CrossCursor;
        } else {
            mouseCursorShape = Qt.ArrowCursor;
        }
        mouseCursorShapeChanged();
    }

    spacing: 5

    contentItem: ToolBar {
        RowLayout {
            id: layout
            spacing: 0
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: 32

            ToolButton {
                id: selectButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight            
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/cursor.svg"
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.SelectDrawMode;
                }
            }

            ToolSeparator {

            }

            ToolButton {
                id: pointButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/point.svg"
                enabled: !controller.isEditing || checked
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.PointDrawMode;
                }
            }

            ToolButton {
                id: lineButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/polyline.svg"
                enabled: !controller.isEditing || checked
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.PolylineDrawMode;
                }
            }

            ToolButton {
                id: polygonButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/polygon.svg"
                enabled: !controller.isEditing || checked
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.PolygonDrawMode;
                }
            }

            ToolButton {
                id: freehandLineButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/freehand-line.svg"
                enabled: !controller.isEditing || checked
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.FreehandLineDrawMode;
                }
            }

            ToolButton {
                id: freehandAreaButton
                ButtonGroup.group: buttonGroup
                checkable: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/freehand-area.svg"
                enabled: !controller.isEditing || checked
                display: AbstractButton.IconOnly
                onCheckedChanged: {
                    if (checked)
                        controller.drawMode = DrawToolbarController.DrawMode.FreehandPolygonDrawMode;
                }
            }

            ToolSeparator {

            }

            ToolButton {
                checkable: false
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                enabled: controller.canUndo
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/undo.svg"
                display: AbstractButton.IconOnly
                onClicked: {
                    controller.undo();
                }
            }

            ToolButton {
                checkable: false
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                enabled: controller.canRedo
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/redo.svg"
                display: AbstractButton.IconOnly
                onClicked: {
                    controller.redo();
                }
            }

            ToolButton {
                checkable: false
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/trash.svg"
                display: AbstractButton.IconOnly
                onClicked: {
                    controller.clearAll();
                }
            }
            //            ToolButton {
            //                checkable: false
            //                Layout.fillHeight: true
            //                Layout.alignment: Qt.AlignCenter
            //                icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/check.svg"
            //                display: AbstractButton.IconOnly
            //                onClicked: {
            //                    controller.save();
            //                }
            //            }
        }
    }

    ButtonGroup {
        id: buttonGroup
        onCheckedButtonChanged: {
            if (!checkedButton) {
                console.log("none checked");
                selectButton.checked = true;
            }
        }
    }

    onMapViewChanged: {
        if (controller) {
            controller.mapView = mapView;
        }
    }
}
