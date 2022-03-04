
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

    property int automaticSelectionMode: FloorFilterController.AutomaticSelectionMode.Always

    Binding {
        target: controller
        property: "automaticSelectionMode"
        value: floorFilter.automaticSelectionMode
    }

    property bool collapsedIcons: true

    property int maxNumberLevels: 2

    padding: 5

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
                    icon.source: collapsedIcons ? "images/chevrons-right.svg" : "images/chevrons-left.svg"
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
                            controller.zoomToCurrentSite()
                        else if (internal.currentVisibileListView
                                 === FloorFilter.VisibleListView.Facility
                                 || !listView.visible)
                            controller.zoomToCurrentFacility()
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
                    implicitHeight: repeater.buttonHeight * repeater.count
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
                                    controller.selectedLevelId = model.modelId
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
                    //                Layout.rowSpan: noResultsFoundLabel.visible ? 1 : 2
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

                    Button {
                        flat: true
                        display: AbstractButton.IconOnly
                        width: 32
                        height: 32
                        topPadding: 0
                        bottomPadding: 0
                        leftPadding: 0
                        rightPadding: 0
                        anchors {
                            right: parent.right
                            top: parent.top
                            bottom: parent.bottom
                            margins: 1
                        }
                        icon {
                            // half size the parent (small 'x')
                            width: parent.width / 2
                            height: parent.height / 2
                            source: "images/x.svg"
                        }
                        onClicked: searchTextField.text = ""
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
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight / count
                    Layout.maximumHeight: contentHeight / count * 3
                    Layout.topMargin: 5
                    Layout.rowSpan: showAllFacilities.visible ? 1 : 2
                    ScrollBar.vertical: ScrollBar {}
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
                            width: listView.width
                            highlighted: internal.currentVisibileListView
                                         === FloorFilter.VisibleListView.Site ? index === internal.selectedSiteIdx : index === internal.selectedFacilityIdx
                            text: model.name
                            onClicked: {
                                // switch to facility view
                                if (internal.currentVisibileListView
                                        === FloorFilter.VisibleListView.Site) {
                                    controller.setSelectedSiteId(model.modelId)
                                    internal.currentVisibileListView
                                            = FloorFilter.VisibleListView.Facility
                                    controller.zoomToCurrentSite()
                                } // switch to level view
                                else if (internal.currentVisibileListView
                                         === FloorFilter.VisibleListView.Facility) {
                                    controller.setSelectedFacilityId(
                                                model.modelId)
                                    buildingMenuButton.checked = false
                                    closer.checked = false
                                    controller.zoomToCurrentFacility()
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

                Label {
                    id: showAllFacilities
                    text: '<a href=" "style="text-decoration: none">All sites</a>'
                    textFormat: Text.RichText
                    Layout.alignment: Qt.AlignCenter
                    Layout.columnSpan: 3
                    Layout.margins: 5
                    visible: internal.currentVisibileListView === FloorFilter.VisibleListView.Site

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            controller.populateAllFacilities()
                            internal.currentVisibileListView = FloorFilter.VisibleListView.Facility
                        }
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
