
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
import Esri.ArcGISRuntime.Toolkit.Controller 100.14
import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


/*!
  \qmltype FloorFilter
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \since 100.14
  \brief Allows to display and filter the available floor aware layers in the current \c GeoModel.
*/
Item {
    id: floorFilter
    property var geoView

    property FloorFilterController controller: FloorFilterController {}

    Binding {
        target: controller
        property: "geoView"
        value: floorFilter.geoView
    }

    GridLayout {
        columns: 2
        ColumnLayout {
            Layout.alignment: Qt.AlignBottom
            ToolBar {
                id: levelFilterMenu
                visible: true

                Action {
                    id: close
                    icon.source: "images/x.svg"
                    onTriggered: levelFilterMenu.visible = false
                }

                ColumnLayout {

                    ToolButton {
                        Layout.fillWidth: true
                        action: close
                    }

                    ToolSeparator {
                        Layout.fillWidth: true
                        orientation: Qt.Horizontal
                    }

                    Repeater {
                        model: controller.floors
                        delegate: ToolButton {
                            Layout.fillWidth: true
                            text: model.shortName
                        }
                    }

                    ToolSeparator {
                        Layout.fillWidth: true
                        orientation: Qt.Horizontal
                    }
                }
            }

            ToolBar {
                Action {
                    id: facility
                    icon.source: "images/organization-24.svg"
                    onTriggered: facilityFilterMenu.visible ? facilityFilterMenu.visible = false : facilityFilterMenu.visible = true
                }

                ToolButton {
                    Layout.fillWidth: true
                    action: facility
                    icon.color: "transparent"
                }
            }
            Component.onCompleted: console.log("toolbar", width, height)
        }

        GridLayout {
            id: facilityFilterMenu
            flow: GridLayout.TopToBottom
            Layout.alignment: Qt.AlignBottom
            rows: 4
            Component.onCompleted: console.log("grid", width, height)
            Rectangle {
                Layout.fillHeight: true
                Layout.rowSpan: 2
                width: leftChevronImg.width + 10
                border.color: "black"
                Image {
                    anchors.centerIn: parent
                    id: leftChevronImg
                    source: "images/chevron-left.svg"
                    //visible: internal.currentVisibileListView === FloorFilter.VisibleListView.FACILITY
                }
                //visible: internal.currentVisibileListView === FloorFilter.VisibleListView.FACILITY
            }

            Image {
                id: searchImg
                sourceSize.width: 32
                sourceSize.height: 32
                source: "images/search.svg"
            }

            ListView {
                Component.onCompleted: console.log("listview: ", width, height)
                visible: true
                Layout.columnSpan: 3
                Layout.fillWidth: true
                implicitHeight: contentHeight
                implicitWidth: contentWidth
                model: controller.filteredFacilities
                delegate: ItemDelegate {
                    //width: parent.width
                    text: '\u2022 ' + model.name
                    onClicked: {
                        facilityFilterMenu.visible = false
                        levelFilterMenu.visible = true
                    }
                    background: Rectangle {
                        Component.onCompleted: console.log("rect: ",
                                                           width, height)
                        border.color: "black"
                    }
                }
            }
            Text {
                Layout.fillWidth: true
                text: controller.selectedFacilityId
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedSiteId
            }

            TextField {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                x: searchImg.width
                placeholderText: "Search"
                onTextChanged: controller.filterFacilities(text)
            }

            Image {
                id: closeImg
                Layout.rowSpan: 2
                Layout.fillHeight: true
                source: "images/x.svg"
            }
        }
    }

    enum VisibleListView {
        SITE,
        FACILITY,
        NONE
    }

    QtObject {
        id: internal
        property int currentVisibileListView: FloorFilter.VisibleListView.NONE
    }
}
