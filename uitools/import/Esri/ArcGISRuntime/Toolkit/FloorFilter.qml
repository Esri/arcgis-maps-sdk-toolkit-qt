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
import QtGraphicalEffects 1.12

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

    property int updateLevelsMode: controller.updateLevelsMode

    property bool hideSiteFacilityButton: false

    property int automaticSelectionMode: FloorFilterController.AutomaticSelectionMode.Always

    Binding {
        target: controller
        property: "automaticSelectionMode"
        value: floorFilter.automaticSelectionMode
    }

    property bool collapsedIcons: true

    property int maxNumberLevels: 2

    padding: 5

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

    contentItem: Grid {
        rows: 1
        layoutDirection: (internal.leaderPosition === FloorFilter.LeaderPosition.UpperLeft)
                         || (internal.leaderPosition === FloorFilter.LeaderPosition.LowerLeft) ? Qt.LeftToRight : Qt.RightToLeft
        verticalItemAlignment: (internal.leaderPosition === FloorFilter.LeaderPosition.UpperLeft)
                               || (internal.leaderPosition === FloorFilter.LeaderPosition.UpperRight) ? Grid.AlignTop : Grid.AlignBottom
        columnSpacing: 5
        ToolBar {

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
                ToolButton {
                    id: collapser
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    icon.source: !((internal.leaderPosition === FloorFilter.LeaderPosition.UpperRight ||
                                    internal.leaderPosition === FloorFilter.LeaderPosition.LowerRight) ^ collapsedIcons) ? "images/chevrons-left.svg" : "images/chevrons-right.svg"
                    text: "Collapse"
                    flat: true
                    display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                    onClicked: collapsedIcons = !collapsedIcons
                }
                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }
                ToolButton {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "images/zoom-to-object.svg"
                    text: "Zoom to"
                    flat: true
                    enabled: !listView.visible ? controller.selectedFacilityId : internal.currentVisibileListView === FloorFilter.VisibleListView.Site ? controller.selectedSiteId : controller.selectedFacilityId
                    display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                    onClicked: {
                        if (internal.currentVisibileListView === FloorFilter.VisibleListView.Site
                                && listView.visible)
                            controller.zoomToSite(controller.selectedSiteId)
                        else if (internal.currentVisibileListView
                                 === FloorFilter.VisibleListView.Facility
                                 || !listView.visible)
                            controller.zoomToFacility(
                                        controller.selectedFacilityId)
                        else
                            console.error("extra enum not accounted for.")
                    }
                }
                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                Flickable {
                    visible: !closer.checked

                    // dont need to use the id of the column
                    contentHeight: contentItem.childrenRect.height
                    Layout.fillWidth: true
                    Layout.maximumHeight: repeater.buttonHeight * maxNumberLevels
                    Layout.preferredHeight: repeater.buttonHeight * repeater.count
                    clip: true
                    ScrollBar.vertical: ScrollBar {}

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        Repeater {
                            id: repeater

                            property int downItem
                            // defaulting to 0, so in case of model.count === 0, the buttonHeight value is not undefined
                            property int buttonHeight: 0
                            model: controller.levels
                            delegate: ToolButton {
                                Component.onCompleted: repeater.buttonHeight = this.height
                                id: levelButton
                                visible: !closer.checked
                                checked: controller.selectedLevelId === model.modelId
                                autoExclusive: true
                                anchors.left: parent.left
                                anchors.right: parent.right
                                text: closer.checked
                                      || collapsedIcons ? model.shortName : model.longName
                                flat: true
                                display: AbstractButton.TextOnly
                                onClicked: {
                                    controller.setSelectedLevelId(model.modelId)
                                }
                            }
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
                    Layout.fillWidth: true
                    // always checked
                    checkable: false
                    checked: true
                    text: controller.selectedLevel ? controller.selectedLevel.shortName : ""
                    flat: true
                    display: AbstractButton.TextOnly
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
                    flat: true
                    display: collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                }
            }
        }

        Pane {
            padding: 0
            //            anchors.left: levelFilterMenu.right
            //            anchors.margins: 10
            visible: buildingMenuButton.checked
            GridLayout {
                Component.onCompleted: {
                    console.log("completed")
                }
                flow: GridLayout.TopToBottom
                Layout.alignment: Qt.AlignBottom
                rows: 6
                columnSpacing: 0
                rowSpacing: 0
                Button {
                    Layout.margins: 0
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
                    }
                    onClicked: {
                        internal.currentVisibileListView = FloorFilter.VisibleListView.Site
                        // When showing site view, resetting the not ignore current site. (this button is only way to get to site view)
                        controller.selectedSiteRespected = true
                    }
                }

                TextField {
                    id: searchTextField
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    //                Layout.rowSpan: noResultsFoundLabel.visible ? 1 : 2
                    Layout.margins: 5
                    placeholderText: "Search"
                    leftPadding: searchImg.width + 5
                    Image {
                        id: searchImg
                        sourceSize.width: 32
                        sourceSize.height: 32
                        visible: false
                        source: "images/search.svg"
                        width: height
                        anchors {
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                            margins: 4
                        }
                    }
                    ColorOverlay {
                        anchors.fill: searchImg
                        source: searchImg
                        color: noResultsFoundLabel.color
                    }
                }

                Label {
                    id: noResultsFoundLabel
                    text: "No results found"
                    visible: false
                    Layout.columnSpan: 3
                    Layout.rowSpan: showAllFacilities.visible ? 1 : 2
                    Layout.fillWidth: true
                    Layout.topMargin: 5
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                ListView {
                    id: listView
                    visible: true
                    Layout.preferredHeight: 200
                    Layout.columnSpan: 3
                    cacheBuffer: Math.max(contentHeight,0)
                    contentWidth: contentItem.childrenRect.width
                    //implicitWidth: contentWidth
                    Layout.minimumWidth: contentItem.childrenRect.width
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: count ? contentHeight / count : 0
                    Layout.maximumHeight: count ? contentHeight / count * 3 : 0
                    Layout.topMargin: 5
                    Layout.rowSpan: showAllFacilities.visible ? 1 : 2
                    ScrollBar.vertical: ScrollBar {}
                    spacing: 5
                    clip: true

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
                                        if (item.model.name.toLowerCase(
                                                    ).includes(
                                                    searchTextField.text.toLowerCase(
                                                        )))
                                            item.inFiltered = true
                                        else
                                            item.inFiltered = false
                                    }
                                }
                                listView.visible = filteredGroup.count > 0
                                noResultsFoundLabel.visible = !listView.visible
                            }
                        }

                        model: internal.currentVisibileListView
                               === FloorFilter.VisibleListView.Site ? controller.sites : controller.facilities
                        filterOnGroup: "filtered"
                        groups: [
                            DelegateModelGroup {
                                id: filteredGroup
                                name: "filtered"
                                includeByDefault: true
                            }
                        ]
                        delegate: RadioDelegate {
                            id: radioDelegate
                            // wait that the radiodelegates are all set, then resize them into the largest of them (stored in the listview contentItem)
                            Component.onCompleted: width = listView.contentItem.childrenRect.width
                            property var parentSiteName: model.parentSiteName ?? ""
                                                                                 highlighted: internal.currentVisibileListView === FloorFilter.VisibleListView.Site ? index === internal.selectedSiteIdx : index === internal.selectedFacilityIdx
                            text: model.name + (model.parentSiteName && !controller.selectedSiteRespected ? '<br/>' + parentSiteName : "")

                            onClicked: {
                                // switch to facility view
                                if (internal.currentVisibileListView
                                        === FloorFilter.VisibleListView.Site) {
                                    controller.setSelectedSiteId(model.modelId)
                                    internal.selectedSiteIdx = index
                                    // resetting the previous selected facility
                                    internal.selectedFacilityIdx = -1
                                    controller.zoomToSite(model.modelId)
                                    internal.currentVisibileListView
                                            = FloorFilter.VisibleListView.Facility
                                } // switch to level view
                                else if (internal.currentVisibileListView
                                         === FloorFilter.VisibleListView.Facility) {
                                    controller.setSelectedFacilityId(model.modelId)
                                    internal.selectedFacilityIdx = index
                                    buildingMenuButton.checked = false
                                    closer.checked = false
                                    controller.zoomToFacility(model.modelId)
                                    if (!controller.selectedSiteRespected) {
                                        const idx = controller.getCurrentSiteIdx();
                                        // idx could be null if not found, guard from it
                                        if (idx != null)
                                            internal.selectedSiteIdx = idx;

                                    }
                                }
                            }
                        }
                    }
                }
                ToolSeparator {
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    orientation: Qt.Horizontal
                }
                Button {
                    id: showAllFacilities
                    text: 'All sites'
                    Layout.alignment: Qt.AlignCenter
                    Layout.columnSpan: 3
                    Layout.margins: 5
                    Layout.fillWidth: true
                    flat: true
                    visible: internal.currentVisibileListView === FloorFilter.VisibleListView.Site

                    onClicked: {
                        controller.selectedSiteRespected = false
                        internal.currentVisibileListView = FloorFilter.VisibleListView.Facility
                    }
                }

                Label {
                    id: siteLabel
                    font.bold: internal.currentVisibileListView === FloorFilter.VisibleListView.Site
                    Layout.fillWidth: true
                    // the two titles are close to each others
                    Layout.topMargin: 5
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    horizontalAlignment: Text.AlignHCenter
                    text: controller.selectedSite ? controller.selectedSite.name : "Select the Site"
                }
                Label {
                    font.bold: internal.currentVisibileListView
                               === FloorFilter.VisibleListView.Facility
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
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
    }

    // used to switch between site and facilities listviews and models.
    enum VisibleListView {
        Site,
        Facility
    }

    enum LeaderPosition {
        UpperLeft,
        UpperRight,
        LowerRight,
        LowerLeft
    }

    QtObject {
        id: internal

        property int currentVisibileListView: FloorFilter.VisibleListView.Site
        //idx refers to repeter or listview idx, not the model idx.
        property int selectedFacilityIdx: -1
        property int selectedSiteIdx: -1
        onCurrentVisibileListViewChanged: console.log("curr changed",
                                                      currentVisibileListView)
        // absolute position parent floorfilter
        property var parentOrigin: mapToItem(floorFilter.parent,
                                             floorFilter.parent.x,
                                             floorFilter.parent.y)
        // absolute position center point floorfilter
        property var floorFilterOrigin: (mapToItem(floorFilter.parent,
                                                   floorFilter.x,
                                                   floorFilter.y))
        property double centerFloorFilterX: (floorFilterOrigin.x + floorFilter.width) / 2
        property double centerFloorFilterY: (floorFilterOrigin.y + floorFilter.height) / 2
        property double centerParentX: (parentOrigin.x + floorFilter.parent.width) / 2
        property double centerParentY: (parentOrigin.y + floorFilter.parent.height) / 2
        property int leaderPosition: {
            if (centerFloorFilterX < centerParentX) {
                if (centerFloorFilterY < centerParentY)
                    FloorFilter.LeaderPosition.UpperLeft
                else
                    FloorFilter.LeaderPosition.LowerLeft
            } else {
                if (centerFloorFilterY < centerParentY)
                    FloorFilter.LeaderPosition.UpperRight
                else
                    FloorFilter.LeaderPosition.LowerRight
            }
        }
    }
}
