
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

    //debug property: should always be true, autoselecting singlefacilitiesistes in case they are single
    property bool autoselectSingleFacilitySite: false

    property bool collapsedIcons: true

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
                id: closer
                checkable: true
                checked: true
                visible: !closer.checked
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                icon.source: "images/x.svg"
                text: "Close"
                display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
            }

            ToolSeparator {
                visible: !closer.checked
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            Repeater {
                id: repeater
                property int downItem

                model: controller.levels
                delegate: ToolButton {
                    visible: !closer.checked
                    checked: controller.selectedLevelId === model.modelId
                    autoExclusive: true
                    Layout.fillWidth: true
                    text: collapser.checked ? model.shortName : model.longName
                    onClicked: {
                        controller.selectedLevelId = model.modelId
                    }
                }
            }

            ToolSeparator {
                Layout.fillWidth: true
                visible: !closer.checked && !hideSiteFacilityButton
                orientation: Qt.Horizontal
            }
            // not visible when floorFilter is shown and not visible if the children has no text to be shown.
            ToolButton {
                visible: closer.checked && text !== ""
                id: itemSelectedButton
                text: controller.selectedLevel ? controller.selectedLevel.shortName : ""
                onClicked: {
                    closer.checked = false
                }
            }

            ToolButton {
                id: buildingMenuButton
                checkable: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                visible: !hideSiteFacilityButton
                icon.source: "images/organization.svg"
                text: "Browse"
                display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
            }

            ToolSeparator {
                orientation: Qt.Horizontal
            }

            ToolButton {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                icon.source: "images/zoom-to-object.svg"
                text: "Zoom to"
                display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                onClicked: {
                    controller.zoomToCurrentFacility()
                }
            }

            ToolSeparator {
                orientation: Qt.Horizontal
            }

            ToolButton {
                id: collapser
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                icon.source: collapsedIcons ? "images/chevrons-right.svg" : "images/chevrons-left.svg"
                text: "Collapse"
                display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                onClicked: collapsedIcons = !collapsedIcons
            }
        }
    }

    Pane {
        padding: 0
        anchors.left: levelFilterMenu.right
        anchors.margins: 10
        visible: buildingMenuButton.checked
        GridLayout {
            Component.onCompleted: {
                console.log("completed")
            }
            flow: GridLayout.TopToBottom
            Layout.alignment: Qt.AlignBottom
            rows: 5
            columnSpacing: 0
            rowSpacing: 0
            Button {
                Layout.margins: 0
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
                }
                onClicked: internal.currentVisibileListView = FloorFilter.VisibleListView.Site
            }

            TextField {
                id: searchTextField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                Layout.margins: 5
                placeholderText: "Search"
                leftPadding: searchImg.width + 5
                Image {
                    id: searchImg
                    sourceSize.width: 32
                    sourceSize.height: 32
                    width: height
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        margins: 4
                    }

                    source: "images/search.svg"
                }
            }

            CheckBox {
                text: "show all facilities"
                Layout.alignment: Qt.AlignCenter
                Layout.columnSpan: 3
                Layout.margins: 5
                onClicked: {
                    controller.populateAllFacilities()
                    internal.currentVisibileListView = FloorFilter.VisibleListView.Facility
                }
            }

            ListView {
                id: listView
                visible: true
                Layout.preferredHeight: 200
                Layout.columnSpan: 3
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: contentHeight / count
                Layout.maximumHeight: contentHeight / count * 3
                Layout.topMargin: 5

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
                                    if (item.model.name.toLowerCase().includes(
                                                searchTextField.text.toLowerCase(
                                                    )))
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
                    delegate: RadioDelegate {
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
                                closer.checked = false
                                controller.zoomToFacility(model.modelId)
                            }
                        }
                    }
                }
            }
            Label {
                id: siteLabel
                font.bold: true
                Layout.fillWidth: true
                Layout.topMargin: 5
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedSite ? controller.selectedSite.name : "Select the Site"
            }
            Label {
                Layout.fillWidth: true
                Layout.bottomMargin: 5
                horizontalAlignment: Text.AlignHCenter
                text: controller.selectedFacility ? controller.selectedFacility.name : "Select the Facility"
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
                //spacing: 0
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 32
                icon {
                    width: 32
                    height: 32
                    source: "images/x.svg"
                    color: siteLabel.color
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
