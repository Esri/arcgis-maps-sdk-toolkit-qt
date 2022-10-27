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
import Esri.ArcGISRuntime.Toolkit.Controller 100.15

import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Layouts

/*!
 \qmltype UtilityNetworkTrace
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.15
 \brief The user interface for the UtilityNetworkTrace.

 xxxx
 \image docs/utilityNetworkTrace.gif
 Example code in the QML API (C++ API might differ):
 \snippet qml_quick/src/demos/UtilityNetworkTraceDemoForm.qml Set up Bookmark QML
*/

Pane {
    id: utilityNetworkTrace

    /*!
      \qmlproperty UtilityNetworkTraceController controller
      \brief The controller handles binding logic between the UtilityNetworkTrace and
      the \c BookmarkListItem.

      The QML controller is documented \l{UtilityNetworkTraceController}{here} and the CPP controller is documented \l{Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceController}{here}.
    */
    property var controller: UtilityNetworkTraceController { }

    /*!
       \qmlproperty GeoView geoView
       \brief The \c GeoView for this tool. Currenlty, it can be a \c Map; but could be extended to use a \c Scene as well.
     */
    property var geoView;

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    wheelEnabled: true
    hoverEnabled: true

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
                id: startingPints
                visible: true
                model: utilityNetworkTrace.controller.startingPoints
                textRole: "name"
                /*onStartingPointsChanged: {
                    const index = currentIndex;
                    const formats = utilityNetworkTrace.controller.formats;
                    let modelData = formats[index];
                    if (modelData === undefined) {
                        modelData = formats.element(formats.index(index, 0));
                    }
                    inputFormat.type = modelData;
                    inputFormat.updateCoordinatePoint(utilityNetworkTrace.controller.currentPoint());
                }*/
            }

            /*TextField {
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
                icon.source: menuButton.checked ? "images/chevron-up.svg" : "images/chevron-down.svg"
            }*/
        }

        RowLayout {
            Layout.margins: 0
            visible: true
            /*Button {
                id: addConversionButton
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.margins: 5
                text: "Add conversion"
                flat: true
                onClicked: {
                    addConversionMenu.visible = true;
                }
            }

            Button {
                id: zoomToButton
                icon.source: "images/layer-zoom-to.svg"
                flat: true
                Layout.alignment: Qt.AlignRight

                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
                display: AbstractButton.IconOnly
                onClicked: utilityNetworkTrace.controller.zoomToCurrentPoint()
            }

            Button {
                id: flashCoordinateButton
                icon.source: "images/flash.svg"
                flat: true
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
                display: AbstractButton.IconOnly
                onClicked: {
                    if (!geoView)
                        return;

                    var screenPos = utilityNetworkTrace.controller.screenCoordinate();
                    if (screenPos === null || (screenPos.x === -1.0 && screenPos.y === -1.0))
                        return;

                    var itemPos = geoView.mapToItem(geoView, screenPos.x, screenPos.y);
                    checked = true;
                    var flashImage = internal.flashImageFactory.createObject(geoView, { "x": itemPos.x, "y": itemPos.y, "color": palette.highlight });
                    flashImage.finished.connect(function() { flashCoordinateButton.checked = false; });
                }
            }*/

            Button {
                id: selectStartingPointButton
                checkable: true
                flat: true
                text: "select starting point"
                //icon.source: "images/edit-attributes.svg"
                Layout.alignment: Qt.AlignRight
                Layout.maximumWidth: Layout.maximumHeight
                padding: 0
            }
        }
    }

    implicitHeight: 200
    implicitWidth: 300
}
