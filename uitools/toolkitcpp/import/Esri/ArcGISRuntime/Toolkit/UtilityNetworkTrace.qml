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
import Esri.ArcGISRuntime.Toolkit.Controller

import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Layouts

/*!
 \qmltype UtilityNetworkTrace
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 200.0
 \brief The user interface for the UtilityNetworkTrace tool.

 The UI is only displayed if there is at least one Utility Network loaded in the data model.
 When there are multiple Utility Networks present in the map, the first one in the list is displayed.

 The UI includes a TabBar with one tab for configuring a new trace and another Tab for displaying results.

 The new trace Tab includes a custom UtilityNetworkTraceStartingPointsModel for displaying the different
 properties of starting points in the UI.

 The result Tab itself contains a TabBar of the results made up of custom Tabs defined in
 UtilityNetworkFunctionTraceResultsModel.
 */

Pane {
    id: utilityNetworkTrace
    height: 600
    width: implicitWidth * 1.20

    implicitHeight: 250
    implicitWidth: 300

    /*!
      \qmlproperty UtilityNetworkTraceController controller
      \brief Loads a map with utility networks, on which different trace operations can be performed and the
      operation's results are shown both on the map and textually in the UI.

    */
    property var controller: UtilityNetworkTraceController { }

    /*!
       \qmlproperty GeoView geoView
       \brief The \c GeoView for this tool. Currently, it can be a \c Map; but could be extended to use a \c Scene as well.
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

        id: root

        TabBar {
            id: bar

            Layout.fillWidth: true

            TabButton {
                text: qsTr("New Trace")
                font.pixelSize: 14
            }
            TabButton {
                text: qsTr("Trace Result")
                font.pixelSize: 14
            }
        }

        StackLayout {

            currentIndex: bar.currentIndex
            clip: true

            ColumnLayout {
                id: gridLayoutTrace
                spacing: 5
                width: root.width

                Label {
                    text: "Trace Configuration"
                }

                ComboBox {
                    Layout.margins: 5
                    id: traceConfigurationBox
                    model: controller.traceConfigurationNames
                    Layout.fillWidth: true
                    onCurrentIndexChanged: {
                        controller.setSelectedTraceConfigurationNameByIndex(currentIndex);
                    }
                }

                Label {
                    text: "Starting Points"
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignHCenter
                    running: controller.isAddingStartingPointInProgress
                    visible: running
                }

                ListView {
                    id: startPointList
                    anchors.margins: 4
                    width: parent.width
                    Layout.preferredHeight: 250
                    clip: true
                    ScrollBar.vertical: ScrollBar {}
                    model: controller.startingPoints
                    delegate: Pane {

                        ColumnLayout {
                            width: startPointList.width - (startPointList.anchors.margins * 4)
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                ColumnLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                        text: sourceName
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Label {
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                        text: groupName
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                                Button {
                                    Layout.preferredWidth: 48
                                    icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/zoom-to-object.svg"
                                    icon.width: 16
                                    icon.height: 16
                                    onClicked: controller.zoomToStartingPoint(index)
                                }
                                Button {
                                    Layout.preferredWidth: 48
                                    icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/trash.svg"
                                    icon.width: 16
                                    icon.height: 16
                                    onClicked: controller.removeStartingPoint(index)
                                }
                            }
                            RowLayout {
                                spacing: 4
                                Layout.fillWidth: true

                                ComboBox {
                                    id: multipleTerminalsCombo
                                    Layout.fillWidth: true
                                    padding: 0
                                    visible: hasMultipleTerminals
                                    model: multipleTerminalNames
                                    onCurrentIndexChanged: {
                                        if (selectedTerminalIndex !== currentIndex)
                                            selectedTerminalIndex = currentIndex;
                                    }
                                }

                                Slider {
                                    id: slider
                                    Layout.fillWidth: true
                                    padding: 0
                                    stepSize: 0.01
                                    snapMode: Slider.SnapAlways
                                    from: 0.0
                                    to: 1.0
                                    visible: hasFractionAlongEdge
                                    value: fractionAlongEdge
                                    onValueChanged: {
                                        if (fractionAlongEdge !== value) {
                                            fractionAlongEdge = value
                                        }
                                    }
                                }
                                Label {
                                    text: slider.value.toFixed(2)
                                    visible: hasFractionAlongEdge
                                }
                            }
                        }
                    }
                }

                Label {
                    visible: controller.isAddingStartingPointEnabled
                    text: "Click on the map to identify starting points."
                    horizontalAlignment: Text.AlignLeft
                }

                RowLayout {
                    Layout.margins: 0
                    visible: true

                    Button {
                        id: selectStartingPointButton
                        text: controller.isAddingStartingPointEnabled ? "Cancel" : "Add Starting Point"
                        Layout.alignment: Qt.AlignRight
                        Layout.maximumWidth: Layout.maximumHeight
                        padding: 0
                        onClicked: controller.isAddingStartingPointEnabled = !controller.isAddingStartingPointEnabled
                    }

                    Button {
                        id: removeAllButton
                        text: "Remove All"
                        Layout.alignment: Qt.AlignLeft
                        Layout.maximumWidth: Layout.maximumHeight
                        padding: 0
                        onClicked: controller.removeAllStartingPoints()
                        visible: startPointList.count > 0
                    }
                }

                ColumnLayout {
                    id: gridLayoutResults
                    width: root.width
                    spacing: 0

                    Label {
                        visible: controller.isInsufficientStartingPoints
                        text: "The selected trace configuration requires additional starting points."
                        horizontalAlignment: Text.AlignLeft
                        width: gridLayoutTrace.width
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Label {
                        visible: controller.isAboveMinimumStartingPoint
                        text: "There are more starting points than required for the selected trace configuration."
                        horizontalAlignment: Text.AlignLeft
                        width: gridLayoutTrace.width
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Button {
                        id: runTrace
                        text: "Run trace"
                        Layout.alignment: Qt.AlignCenter
                        Layout.maximumWidth: Layout.maximumHeight
                        padding: 0
                        implicitWidth: root.width
                        onClicked: {
                            bar.currentIndex = 1
                            controller.runTrace("n/a")
                        }
                        enabled: !controller.isInsufficientStartingPoints
                    }
                }
            }

            ColumnLayout {
                id: gridLayoutTraceResults
                spacing: 5
                width: root.width

                BusyIndicator {
                    Layout.alignment: Qt.AlignHCenter
                    running: controller.isTraceInProgress
                    visible: running
                }

                ListView {
                    id: functionResultsList
                    anchors.margins: 4
                    width: parent.width
                    Layout.preferredHeight: 250
                    clip: true
                    ScrollBar.vertical: ScrollBar {}
                    model: controller.functionResults
                    delegate: Pane {
                        ColumnLayout {
                            width: functionResultsList.width - (functionResultsList.anchors.margins * 4)
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                ColumnLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                        text: name + ": " + value
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Label {
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                        text: "Function type: <i>" + type + "</i>"
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                            }
                        }
                    }
                }

                Button {
                    id: clearTraceResults
                    text: "Clear results"
                    Layout.alignment: Qt.AlignCenter | Qt.AlignBottom
                    Layout.maximumWidth: Layout.maximumHeight
                    padding: 0
                    onClicked: {
                        bar.currentIndex = 0
                        controller.resetTraceResults()
                    }
                    enabled: controller.isResetResultsEnabled
                    visible: true
                }
            }
        }
    }
}
