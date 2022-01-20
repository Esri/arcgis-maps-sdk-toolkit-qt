
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
import QtQml.Models 2.15


/*!
  \qmltype FloorFilter
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \since 100.14
  \brief Allows to display and filter the available floor aware layers in the current \c GeoView.
*/
Control {
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
            spacing: 0

            ToolBar {
                id: levelFilterMenu
                visible: !internal.isFloorFilterCollapsed
                // property var itemSelected: {text: ""}
                property string itemSelected: ""

                ColumnLayout {

                    Action {
                        id: close
                        icon.source: "images/x.svg"
                        onTriggered: {
                            internal.isFloorFilterCollapsed = true
                        }
                    }

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
                            autoExclusive: true
                            Layout.fillWidth: true
                            text: model.shortName
                            onClicked: {
                                checked = true
                                levelFilterMenu.itemSelected = model.shortName
                                controller.selectedLevelId = model.modelId
                            }
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
                    icon.source: "images/organization.svg"
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
            rowSpacing: 0

            //visible: internal.currentVisibileListView !== FloorFilter.VisibleListView.NONE
            Button {
                Layout.fillHeight: true
                //Layout.fillWidth: true
                Layout.rowSpan: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: internal.currentVisibileListView
                                       === FloorFilter.VisibleListView.Facility ? 32 : 0
                display: AbstractButton.IconOnly
                flat: true
                //removing all paddings and spacing from component so icon will fill compeltely the button
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                spacing: 0

                icon {
                    width: 32
                    height: 32
                    source: "images/chevron-left.svg"
                    color: "black"
                }
                onClicked: internal.currentVisibileListView = FloorFilter.VisibleListView.Site
            }

            Image {
                id: searchImg
                sourceSize.width: 32
                sourceSize.height: 32
                source: "images/search.svg"
            }

            ListView {
                id: listView

                visible: true

                Layout.columnSpan: 3
                Layout.fillWidth: true
                implicitHeight: contentHeight
                implicitWidth: contentWidth

                model: DelegateModel {
                    id: dm
                    property Connections conn: Connections {
                        target: searchTextField
                        function onTextChanged() {

                            //usign items group to fetch all the elements (they are all setin this group automatically)
                            for (var i = 0; i < dm.items.count; ++i) {
                                var item = dm.items.get(i)

                                if (searchTextField.text === "") {
                                    item.inFiltered = true
                                } else {
                                    if (item.model.name.includes(
                                                searchTextField.text))
                                        item.inFiltered = true
                                    else
                                        item.inFiltered = false
                                }
                            }
                        }
                    }

                    model: internal.currentVisibileListView
                           === FloorFilter.VisibleListView.Site ? controller.sites : controller.facilities
                    Component.onCompleted: console.log("count:", count)
                    onCountChanged: console.log("count:", count)
                    filterOnGroup: "filtered"
                    groups: [
                        DelegateModelGroup {
                            id: filteredGroup
                            name: "filtered"
                            includeByDefault: true
                        }
                    ]
                    delegate: ItemDelegate {
                        width: parent.width
                        highlighted: internal.currentVisibileListView
                                     === FloorFilter.VisibleListView.Site ? index === internal.selectedSiteIdx : index === internal.selectedFacilityIdx
                        text: model.name
                        onClicked: {
                            // switch to facility view
                            if (internal.currentVisibileListView
                                    === FloorFilter.VisibleListView.Site) {
                                internal.selectedSiteIdx = index
                                controller.selectedSiteId = model.modelId
                                internal.currentVisibileListView
                                        = FloorFilter.VisibleListView.Facility
                            } // switch to level view
                            else if (internal.currentVisibileListView
                                     === FloorFilter.VisibleListView.Facility) {
                                controller.selectedFacilityId = model.modelId
                                internal.selectedFacilityIdx = index
                                facilityFilterMenu.visible = false
                                internal.isFloorFilterCollapsed = false
                            }
                        }
                    }
                }
            }
            Text {
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedSite ? controller.internal.selectedSite.name : "Select the Site"
            }
            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedFacility ? controller.internal.selectedFacility.name : "Select the Facility"
            }

            TextField {
                id: searchTextField
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 2
                x: searchImg.width
                placeholderText: "Search"
            }

            Button {
                Layout.rowSpan: 2
                Layout.fillHeight: true
                display: AbstractButton.IconOnly
                flat: true
                //removing all paddings and spacing from component so icon will fill compeltely the button
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                spacing: 0
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 32
                icon {
                    width: 32
                    height: 32
                    source: "images/x.svg"
                    color: "black"
                }
                onClicked: facilityFilterMenu.visible = false
            }
        }
    }

    // used to switch between site and facilities listviews and models.
    enum VisibleListView {
        Site,
        Facility
    }

    QtObject {
        id: internal
        property int currentVisibileListView: FloorFilter.VisibleListView.Site
        property bool isFloorFilterCollapsed: true
        //idx refers to repeter or listview idx, not the model idx.
        property int selectedFacilityIdx: -1
        property int selectedSiteIdx: -1
        onCurrentVisibileListViewChanged: console.log("curr changed",
                                                      currentVisibileListView)
        onIsFloorFilterCollapsedChanged: console.log(
                                             "floorFilterMenu ",
                                             isFloorFilterCollapsed ? "collapsed" : "shown")
    }
}
