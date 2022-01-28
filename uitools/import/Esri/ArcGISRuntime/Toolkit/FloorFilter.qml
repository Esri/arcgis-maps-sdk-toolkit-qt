
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

    property var controller: FloorFilterController {}

    property int updateLevelsMode: {
        // dont create a binding to the controller. just set intial value
        updateLevelsMode = controller.updateLevelsMode
    }

    property bool hideSiteFacilityButton: false

    property bool autoselectSingleFacilitySite: false

    Binding {
        target: controller
        property: "autoselectSingleFacilitySite"
        value: floorFilter.autoselectSingleFacilitySite
    }

    // create singlepointing binding towards controller
    Binding {
        target: controller
        property: "updateLevelsMode"
        value: floorFilter.updateLevelsMode
    }

    Binding {
        target: controller
        property: "geoView"
        value: floorFilter.geoView
    }

    contentItem: ToolBar {
        id: levelFilterMenu
        Layout.alignment: Qt.AlignBottom

        ColumnLayout {
            spacing: 0
            ToolButton {
                id: collapser
                checkable: true
                checked: true
                visible: !collapser.checked
                Layout.fillWidth: true
                icon.source: "images/x.svg"
            }

            ToolSeparator {
                visible: !collapser.checked
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            Repeater {
                id: repeater
                property int downItem

                model: controller.levels
                delegate: ToolButton {
                    visible: !collapser.checked
                    checked: controller.selectedLevelId === model.modelId
                    autoExclusive: true
                    Layout.fillWidth: true
                    text: model.shortName
                    onClicked: {
                        controller.selectedLevelId = model.modelId
                    }
                }
            }

            ToolSeparator {
                Layout.fillWidth: true
                visible: !collapser.checked && !hideSiteFacilityButton
                orientation: Qt.Horizontal
            }
            // not visible when floorFilter is shown and not visible if the children has no text to be shown.
            ToolButton {
                visible: collapser.checked && text !== ""
                id: itemSelectedButton
                text: controller.selectedLevel ? controller.selectedLevel.shortName : ""
                onClicked: {
                    collapser.checked = false
                }
            }

            ToolButton {
                id: buildingMenuButton
                checkable: true
                Layout.fillWidth: true
                visible: !hideSiteFacilityButton
                icon.source: "images/organization.svg"
            }
        }
    }

    Pane {
        anchors.left: levelFilterMenu.right
        anchors.margins: 10
        visible: buildingMenuButton.checked
        GridLayout {
            flow: GridLayout.TopToBottom
            Layout.alignment: Qt.AlignBottom
            rows: 5
            rowSpacing: 0

            Button {
                text: "show all facilities"
                Layout.columnSpan: 3
                onClicked: {
                    controller.populateAllFacilities()
                    internal.currentVisibileListView = FloorFilter.VisibleListView.Facility
                }
            }

            Button {
                Layout.fillHeight: true
                //Layout.fillWidth: true
                Layout.rowSpan: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: autoselectSingleFacilitySite ? 0 : (internal.currentVisibileListView === FloorFilter.VisibleListView.Facility ? 32 : 0)
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
                Component.onCompleted: console.log("cont", contentHeight)
                id: listView

                visible: true

                Layout.columnSpan: 3
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: contentHeight / count
                Layout.maximumHeight: contentHeight / count * 3

                ScrollBar.vertical: ScrollBar {}
                clip: true

                //                implicitHeight: contentHeight
                //                implicitWidth: contentWidth
                model: DelegateModel {
                    id: dm
                    property Connections conn: Connections {
                        target: searchTextField
                        function onTextChanged() {

                            //usign items group to fetch all the elements (they are all setting this group automatically)
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
                        Component.onCompleted: console.log("delegate", height)
                        width: listView.width
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
                                buildingMenuButton.checked = false
                                collapser.checked = false
                                controller.zoomToFacility(model.modelId)
                            }
                        }
                    }
                }
            }
            Text {
                font.bold: true
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedSite ? controller.selectedSite.name : "Select the Site"
            }
            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedFacility ? controller.selectedFacility.name : "Select the Facility"
            }

            TextField {
                id: searchTextField
                Layout.fillWidth: true
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
                onClicked: buildingMenuButton.checked = false
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
        //idx refers to repeter or listview idx, not the model idx.
        property int selectedFacilityIdx: -1
        property int selectedSiteIdx: -1
        onCurrentVisibileListViewChanged: console.log("curr changed",
                                                      currentVisibileListView)
    }
}
