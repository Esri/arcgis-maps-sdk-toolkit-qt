
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
  \brief Allows to display and filter the available floor aware layers in the current \c GeoView.
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
                visible: !internal.isFloorFilterCollapsed
                // property var itemSelected: {text: ""}
                property string itemSelected: ""

                Action {
                    id: close
                    icon.source: "images/x.svg"
                    onTriggered: {
                        internal.isFloorFilterCollapsed = true
                    }
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
                        id: repeater
                        property int downItem
                        model: controller.levels
                        delegate: ToolButton {
                            Layout.fillWidth: true
                            text: model.shortName
                            onClicked: {
                                repeater.setItemHighlighted(
                                            index) // _q can access Repeater without using the id?
                                levelFilterMenu.itemSelected = model.shortName
                                controller.selectedLevelId = model.modelId

                                console.log(levelFilterMenu.itemSelected)
                            }
                        }
                        function setItemHighlighted(index) {
                            if (downItem !== undefined) {
                                itemAt(downItem).highlighted = false
                            }
                            itemAt(index).highlighted = true
                            downItem = index
                        }
                    }

                    ToolSeparator {
                        Layout.fillWidth: true
                        orientation: Qt.Horizontal
                    }
                }
            }
            ToolBar {
                id: itemSelectedToolBar
                // not visible when floorFilter is shown and not visible if the children has no text to be shown.
                visible: internal.isFloorFilterCollapsed
                         && itemSelectedButton.text !== ""
                ToolButton {
                    id: itemSelectedButton
                    text: levelFilterMenu.itemSelected
                    onClicked: {
                        console.log(parent)
                        internal.isFloorFilterCollapsed = false
                    }
                }
            }

            ToolBar {
                Action {
                    id: facility
                    icon.source: "images/organization-24.svg"
                    onTriggered: {
                        facilityFilterMenu.visible = !facilityFilterMenu.visible
                    }
                }

                ToolButton {
                    Layout.fillWidth: true
                    action: facility
                    icon.color: "transparent"
                }
            }
        }

        GridLayout {
            id: facilityFilterMenu
            flow: GridLayout.TopToBottom
            Layout.alignment: Qt.AlignBottom
            rows: 4
            //visible: internal.currentVisibileListView !== FloorFilter.VisibleListView.NONE
            Rectangle {
                Layout.fillHeight: true
                Layout.rowSpan: 2
                Layout.alignment: Qt.AlignHCenter
                // if == facility-> gets set twice: once to 24 and after to 0.
                width: internal.currentVisibileListView
                       === FloorFilter.VisibleListView.FACILITY ? 24 : 0
                border.color: "black"
                Image {
                    anchors.centerIn: parent
                    id: leftChevronImg
                    source: "images/chevron-left.svg"
                    sourceSize.width: 24
                    sourceSize.height: 24
                    width: internal.currentVisibileListView
                           === FloorFilter.VisibleListView.FACILITY ? sourceSize.width : 0
                    height: internal.currentVisibileListView
                            === FloorFilter.VisibleListView.FACILITY ? sourceSize.width : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: internal.currentVisibileListView
                                   = FloorFilter.VisibleListView.SITE
                    }
                }
            }

            Image {
                id: searchImg
                sourceSize.width: 32
                sourceSize.height: 32
                source: "images/search.svg"
            }

            ListView {
                Component.onCompleted: console.log("listview: ", count)
                visible: true
                Layout.columnSpan: 3
                Layout.fillWidth: true
                implicitHeight: contentHeight
                implicitWidth: contentWidth
                model: internal.currentVisibileListView
                       === FloorFilter.VisibleListView.SITE ? controller.sites : controller.facilities
                delegate: ItemDelegate {
                    //width: parent.width
                    text: '\u2022 ' + model.name
                    onClicked: {
                        // switch to facility view
                        if (internal.currentVisibileListView === FloorFilter.VisibleListView.SITE) {
                            controller.selectedSiteId = model.modelId
                            internal.currentVisibileListView = FloorFilter.VisibleListView.FACILITY
                        } // switch to level view
                        else if (internal.currentVisibileListView
                                 === FloorFilter.VisibleListView.FACILITY) {
                            controller.selectedFacilityId = model.modelId
                            facilityFilterMenu.visible = false
                            internal.isFloorFilterCollapsed = false
                        }
                    }
                    background: Rectangle {
                        border.color: "black"
                    }
                }
            }
            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedSiteId
                      === "" ? "Select the Site" : controller.selectedSiteId
            }
            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedFacilityId
                      === "" ? "Select the Facility" : controller.selectedFacilityId
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
                sourceSize: Qt.size(24, 24)
                source: "images/x.svg"
                MouseArea {
                    anchors.fill: parent
                    onClicked: facilityFilterMenu.visible = false
                }
            }
        }
    }

    // used to switch between site and facilities listviews and models.
    enum VisibleListView {
        SITE,
        FACILITY
    }

    QtObject {
        id: internal
        property int currentVisibileListView: FloorFilter.VisibleListView.SITE
        property bool isFloorFilterCollapsed: true
        onCurrentVisibileListViewChanged: console.log("curr changed",
                                                      currentVisibileListView)
        onIsFloorFilterCollapsedChanged: console.log(
                                             "floorFilterMenu ",
                                             isFloorFilterCollapsed ? "collapsed" : "shown")
    }
}
