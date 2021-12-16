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
import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.12

import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

/*!
 \qmltype CoordinateConversion
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.12
 \brief The user interface for the coordinate conversion tool.

 This tool allows a user to select a point on the map or to enter a point by
 text entry.
 The point may be highlighted by zooming into that point, or by highlighting
 the point on the GeoView.
 A list of different coordinate formats representing the same point are
 available to the user.
 \image docs/coordinateConversion.png coordinate_conversion
 Example code in the QML API (C++ API might differ):
 \snippet qml_quick/src/demos/CoordinateConversionDemoForm.qml Set up Coordinate Conversion QML
 */

Pane {
    id: coordinateConversionWindow

    clip: true

    /*!
       \qmlproperty GeoView geoView
       \brief The GeoView for this tool. Should be a SceneView or a MapView.
     */
    property var geoView;

    /*!
       \qmlproperty bool inInputMode
       \brief Whether whether the tool is in input mode.
      
       If \c true, the tool will convert a point set via a mouse click or text entry.
     */
    readonly property bool inInputMode: editCoordinateButton.checked || captureModeButton.checked

    /*!
      \qmlproperty CoordinateConversionResult inputFormat
      \brief This is the inputFormat - which dictates what format the user can
      edit and thereby update all the other formats in the ListModel.
    */
    property var inputFormat: CoordinateConversionResult { }

    /*!
      \qmlproperty CoordinateConversionController controller.
      \brief the Controller handles connections writing/reading to the GeoView,
      and maintaining our list of textual representations of a single point
      in multiple formats.
    */
    property var controller: CoordinateConversionController { }

    Connections {
        target: controller
        function onCurrentPointChanged(point) {
            inputFormat.updateCoordinatePoint(point);
        }
    }

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    // Two way binding for picking updates.
    Binding {
        target: captureModeButton
        property: "checked"
        value: controller.inPickingMode
    }

    Binding {
        target: controller
        property: "inPickingMode"
        value: captureModeButton.checked
    }

    contentItem: ColumnLayout {
        id: gridLayout
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
        }

        RowLayout {
            Layout.margins: 0
            ComboBox {
                Layout.margins: 5
                id: inputModeButton
                model: coordinateConversionWindow.controller.formats
                textRole: "name"
                onCurrentIndexChanged: {
                    const index = currentIndex;
                    const formats = coordinateConversionWindow.controller.formats;
                    let modelData = formats[index];
                    if (modelData === undefined) {
                        modelData = formats.element(formats.index(index, 0));
                    }
                    inputFormat.type = modelData;
                    inputFormat.updateCoordinatePoint(coordinateConversionWindow.controller.currentPoint());
                }
            }

            TextField {
                id: editPointEntry
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom
                Layout.margins: 5
                placeholderText: "No position"
                readOnly: !editCoordinateButton.checked
                selectByMouse: !readOnly
                text: inputFormat.type? inputFormat.notation : "No position"
                onEditingFinished: {
                    controller.setCurrentPoint(text, inputFormat.type);
                    editCoordinateButton.checked = false;
                }
            }

            Button {
                id: menuButton
                checkable: true
                checked: false
                flat: true
                Layout.margins: 5
                Layout.alignment: Qt.AlignRight
                icon.source: menuButton.checked ? "images/menu_expand.png" : "images/menu_collapse.png"
            }
        }

        RowLayout {
            Layout.margins: 0
            visible: menuButton.checked
            Button {
                id: addConversionButton
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.margins: 5
                text: "Add conversion"
                flat: true
                onClicked: {
                    addConversionMenu.visible = true;
                }

                Menu {
                    id: addConversionMenu
                    Repeater {
                        model: coordinateConversionWindow.controller.formats
                        MenuItem {
                            text: name
                            enabled: text !== inputModeButton.text
                            onTriggered: {
                                coordinateConversionWindow.controller.addNewCoordinateResultForOption(modelData);
                            }
                        }
                    }
                }
            }

            Button {
                id: zoomToButton
                icon.source: "images/zoom.png"
                flat: true
                Layout.alignment: Qt.AlignRight

                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
                display: AbstractButton.IconOnly
                onClicked: coordinateConversionWindow.controller.zoomToCurrentPoint()
            }

            Button {
                id: flashCoordinateButton
                icon.source: "images/flash.png"
                flat: true
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
                display: AbstractButton.IconOnly
                onClicked: {
                    if (!geoView)
                        return;

                    var screenPos = coordinateConversionWindow.controller.screenCoordinate();
                    if (screenPos === null || (screenPos.x === -1.0 && screenPos.y === -1.0))
                        return;

                    var itemPos = geoView.mapToItem(geoView, screenPos.x, screenPos.y);
                    checked = true;
                    var flashImage = internal.flashImageFactory.createObject(geoView, { "x": itemPos.x, "y": itemPos.y, "color": palette.highlight });
                    flashImage.finished.connect(function() { flashCoordinateButton.checked = false; });
                }
            }

            Button {
                id: editCoordinateButton
                checkable: true
                flat: true
                icon.source: "images/text_editing_mode.png"
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
            }

            Button {
                id: captureModeButton
                checkable: true
                flat: true
                icon.source: "images/mouse_click_mode.png"
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: Layout.maximumHeight
                Layout.margins: 5
                padding: 0
            }
        }

        Repeater {
            model: coordinateConversionWindow.controller.results
            visible: menuButton.checked
            RowLayout {
                Label {
                    id: formatName
                    text: name
                    Layout.minimumWidth: inputModeButton.width
                    Layout.maximumWidth: inputModeButton.width
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    visible: menuButton.checked
                }

                Label {
                    id: formatNotation
                    text: notation
                    Layout.fillWidth: true
                    visible: menuButton.checked
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Button {
                    id: editMenuButton
                    visible: menuButton.checked
                    Layout.minimumWidth: menuButton.width
                    Layout.maximumWidth: menuButton.width
                    Layout.alignment: Qt.AlignRight
                    Layout.margins: 5
                    icon.source: "images/menu.png"
                    flat: true
                    onClicked: editMenu.open()

                    Menu {
                        id: editMenu
                        MenuItem {
                            text: "Delete"
                            onClicked: {
                                coordinateConversionWindow.controller.removeCoordinateResultAtIndex(index);
                            }
                        }
                        MenuItem {
                            text: "Copy to clipboard"
                            // Copy not available in pure QML API
                            enabled: modelData.copyNotationToClipboard !== undefined
                            visible: enabled
                            height: enabled ? implicitHeight : 0
                            onClicked: {
                                modelData.copyNotationToClipboard();
                            }
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal
        property Component flashImageFactory: Qt.createComponent("FlashImage.qml");
    }
}
