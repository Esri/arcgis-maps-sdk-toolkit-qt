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
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

/*!
  \qmltype FloorFilter
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \ingroup ArcGISQtToolkitUiQmlViews
  \since 100.14
  \brief Displays and filters the available floor aware layers in the current \c GeoModel.
  This component supports manual selection and text-based filtering for sites and facilities.
  Once a site and facility is selected, levels can be selected and the GeoView will be updated.

  In addition to the browsing workflow where sites and facilities are manually selected, automatic facility
  selection based on the GeoView's current viewpoint. This mode is useful when you are exploring the
  GeoView but do not know the name of the site or facility.
  For the same reason, it is possible to list all the facilties available in the \c FloorManager using the button on the siteView.

  The user interface is driven from the FloorAware data that is available in the GeoModel's FloorManager.

  2D maps and 3D scenes are supported.
  \image floorfilter.gif
  \snippet qml/demos/FloorFilterDemoForm.qml Set up Floor Filter QML
*/
Control {
    id: floorFilter

    /*!
      \qmlproperty GeoView geoView
      \brief The \c GeoView for this tool. Should be a \c SceneView or \c MapView.
    */
    property var geoView

    /*!
      \internal
    */
    property var controller: FloorFilterController {}

    /*!
      \qmlproperty enumeration updateLevelsMode
      \brief The mode to use for updating levels visibility.
    */
    property int updateLevelsMode: controller.updateLevelsMode

    /*!
      \qmlproperty int maxNumberLevels
      \brief trims the maximum number of viewable levels.
      A scrollable component is automatically used in case of higher number of levels.
    */
    property int maxNumberLevels: 2

    padding: 5

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
        objectName: "gridOuterWrapper"
        rows: 1
        // changing the layout direction based on the current position of the floorfilter relative to its parent.
        // switching the layout will result in switching the levels toolbar with the site/facility view.
        layoutDirection: (internal.parentPosition === FloorFilter.ParentPosition.UpperLeft)
                         || (internal.parentPosition === FloorFilter.ParentPosition.LowerLeft) ? Qt.LeftToRight : Qt.RightToLeft
        // verticalalignment based on current position of the floorfilter relative to its parent.
        // upper: top align; lower: bottom align
        verticalItemAlignment: (internal.parentPosition === FloorFilter.ParentPosition.UpperLeft)
                               || (internal.parentPosition === FloorFilter.ParentPosition.UpperRight) ? Grid.AlignTop : Grid.AlignBottom
        columnSpacing: 5
        ToolBar {
            id: levelFilterMenu

            Layout.alignment: Qt.AlignBottom
            ColumnLayout {
                spacing: 0
                ToolButton {
                    id: closeButton
                    objectName: "closer"
                    checkable: true
                    checked: true
                    visible: !closeButton.checked
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/x.svg"
                    text: qsTr("Close")
                    display: internal.collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                }

                ToolSeparator {
                    visible: !closeButton.checked
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                ToolButton {
                    id: collapser
                    objectName: "collapser"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    // use different pointing icon based on the leader position. if icons are collapsed and left position: use right pointing icon;
                    // if right position: use left pointing icon; if collapsedIcons === true, switch the icon.
                    icon.source: (internal.parentPosition === FloorFilter.ParentPosition.UpperRight ||
                                    internal.parentPosition === FloorFilter.ParentPosition.LowerRight) === internal.collapsedIcons
                                 ? "qrc:/Esri/ArcGISRuntime/Toolkit/chevrons-left.svg" : "qrc:/Esri/ArcGISRuntime/Toolkit/chevrons-right.svg"
                    text: qsTr("Collapse")
                    flat: true
                    display: internal.collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                    onClicked: internal.collapsedIcons = !internal.collapsedIcons
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                ToolButton {
                    objectName: "zoom"
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/zoom-to-object.svg"
                    text: qsTr("Zoom to")
                    flat: true
                    // Zooming to currentVisibleListView, facility or site. If listView is not visible, zoom to facility (regardeless of currentVisibleListView).
                    // Also check if facility/site are defined, otherwise zoom button not enabled.
                    enabled: listView.visible ? (internal.currentVisibileListView === FloorFilter.VisibleListView.Site ? controller.selectedSiteId
                                              : controller.selectedFacilityId) : controller.selectedFacilityId
                    display: internal.collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                    onClicked: {
                        switch(internal.currentVisibileListView){
                            case FloorFilter.VisibleListView.Site:
                                controller.zoomToSite(controller.selectedSiteId);
                                break;
                            case FloorFilter.VisibleListView.Facility:
                                controller.zoomToFacility(controller.selectedFacilityId);
                                break;
                            default:
                                console.error("extra enum not accounted for.");
                        }
                    }
                }

                ToolSeparator {
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                //Layout width: fill the parent Layout if is larger than the `contentItem.width`.
                // If `contentItem.width` is larger, stretch to keep all its items without resizing them (don't elide text children).
                Flickable {
                    id: flickable
                    objectName: "flickable"
                    visible: !closeButton.checked
                    // dont need to use the id of the column
                    contentHeight: contentItem.childrenRect.height
                    Layout.maximumHeight: repeater.buttonHeight * maxNumberLevels
                    Layout.preferredHeight: repeater.buttonHeight * repeater.count
                    Layout.fillWidth: true
                    clip: true
                    // scroll to the bottom of the flickable (so verticalorder 0 default selected level is visible)
                    contentY : contentHeight - height
                    ScrollBar.vertical: ScrollBar {
                        policy: parent.contentHeight > parent.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    }

                    ColumnLayout {
                        objectName: "wrapper"
                        spacing: 0
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        Repeater {
                            id: repeater
                            objectName: "repeater"
                            // defaulting to 0, so in case of model.count === 0, the buttonHeight value is not undefined.
                            // buttonHeight used to set the buttons height, so the flickable visible will be exactly buttonHeight * numberLevels. They are not clipped half a level but assumes that all levels have same height.
                            property int buttonHeight: 0
                            model: controller.levels
                            delegate: ToolButton {
                                Component.onCompleted: repeater.buttonHeight = Math.max(repeater.buttonHeight, this.height)
                                id: levelButton
                                visible: !closeButton.checked
                                checked: controller.selectedLevelId === model.modelId
                                autoExclusive: true
                                Layout.fillWidth: true
                                text: closeButton.checked
                                      || internal.collapsedIcons ? model.shortName : model.longName
                                flat: true
                                display: AbstractButton.TextOnly
                                onClicked: {
                                    controller.setSelectedLevelId(model.modelId);
                                }
                            }
                        }
                    }
                }

                ToolSeparator {
                    visible: flickable.visible && repeater.count !== 0
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                // not visible when floorFilter is shown and not visible if the children has no text to be shown.
                ToolButton {
                    id: itemSelectedButton
                    objectName: "itemSelectedButton"
                    visible: closeButton.checked && text !== ""
                    Layout.fillWidth: true
                    // always checked
                    checkable: false
                    checked: true
                    text: controller.selectedLevel ? (internal.collapsedIcons ? controller.selectedLevel.shortName : controller.selectedLevel.longName) : ""
                    flat: true
                    display: AbstractButton.TextOnly
                    onClicked: {
                        closeButton.checked = false;
                    }
                }

                ToolSeparator {
                    visible: itemSelectedButton.visible
                    Layout.fillWidth: true
                    orientation: Qt.Horizontal
                }

                ToolButton {
                    id: buildingMenuButton
                    objectName: "buildingMenuButton"
                    checkable: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "qrc:/Esri/ArcGISRuntime/Toolkit/organization.svg"
                    text: qsTr("Browse")
                    flat: true
                    display: internal.collapsedIcons ? AbstractButton.IconOnly : AbstractButton.TextBesideIcon
                }
            }
        }

        Pane {
            padding: 0
            visible: buildingMenuButton.checked
            GridLayout {
                flow: GridLayout.TopToBottom
                Layout.alignment: Qt.AlignBottom
                rows: 6
                columnSpacing: 0
                rowSpacing: 0
                Button {
                    objectName: "backToSite"
                    Layout.margins: 0
                    Layout.fillHeight: true
                    Layout.rowSpan: 2
                    Layout.alignment: Qt.AlignHCenter
                    // setting width to 0 and not setting visibility to false, otherwise it would break the rows layout by having a missing row.
                    Layout.preferredWidth: internal.currentVisibileListView
                                           === FloorFilter.VisibleListView.Facility && controller.sites.count > 0 ? 32 : 0
                    display: AbstractButton.IconOnly
                    flat: true
                    // removing all paddings and spacing from component so icon will fill compeltely the button
                    topPadding: 0
                    bottomPadding: 0
                    leftPadding: 0
                    rightPadding: 0
                    spacing: 0

                    icon {
                        width: 32
                        height: 32
                        source: "qrc:/Esri/ArcGISRuntime/Toolkit/chevron-left.svg"
                    }
                    onClicked: {
                        internal.currentVisibileListView = FloorFilter.VisibleListView.Site;
                        // When switching to site view, reset the `selectedSiteRespected` to true. (this button is the only way to get to the site view).
                        controller.selectedSiteRespected = true;
                    }
                }

                TextField {
                    id: searchTextField
                    objectName: "searchTextField"
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    Layout.margins: 5
                    placeholderText: internal.currentVisibileListView === FloorFilter.VisibleListView.Site ? qsTr("Filter Sites") : qsTr("Filter Facilities")
                    leftPadding: searchImg.width + 5
                    Image {
                        id: searchImg
                        sourceSize.width: 32
                        sourceSize.height: 32
                        visible: true
                        source: "qrc:/Esri/ArcGISRuntime/Toolkit/search.svg"
                        width: height
                        anchors {
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                            margins: 4
                        }
                    }
                }

                Label {
                    id: noResultsFoundLabel
                    objectName: "noResultsFoundLabel"
                    text: qsTr("No matches found")
                    visible: false
                    Layout.columnSpan: 3
                    // expanding to the showAllFacilities cell (and its ToolSeparator) in case is not visible. This will not break the grid layout, otherwise there is a missing row.
                    // no checks on listView visibility because they are opposite visibility, so one takes the other cell when it is visible.
                    Layout.rowSpan: !showAllFacilities.visible ? 3 : 1
                    Layout.fillWidth: true
                    Layout.topMargin: 5
                    Layout.bottomMargin: 5
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                ListView {
                    id: listView
                    objectName: "listView"
                    visible: true
                    Layout.preferredHeight: 200
                    Layout.columnSpan: 3
                    cacheBuffer: Math.max(contentHeight,0)
                    contentWidth: contentItem.childrenRect.width
                    Layout.minimumWidth: contentItem.childrenRect.width
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: count ? contentHeight / count : 0
                    Layout.maximumHeight: count ? contentHeight / count * 3 : 0
                    Layout.topMargin: 5
                    // span taking space of showAllFacilities and the related ToolSeparator
                    Layout.rowSpan: showAllFacilities.visible ? 1 : 3
                    ScrollBar.vertical: ScrollBar {}
                    spacing: 5
                    clip: true
                    // known bug: currentIndex is not on sync with the highlighted delegate when a new model is set/updated (listview automatically set currentIndex to 0).
                    // scroll list view to selected item.
                    onCurrentIndexChanged: positionViewAtIndex(currentIndex, ListView.Visible)

                    Connections {
                        target: searchTextField
                        function setItemsinFiltered() {
                            for(let i = 0; i < delegateModel.items.count; ++i)
                                delegateModel.items.get(i).inFiltered = true;
                        }

                        function onTextChanged() {
                            const searchTextLower = searchTextField.text.toLowerCase();
                            // empty string clears the filtering. All elements are visible (set into the `filtered` group).
                            if (searchTextLower === "")
                                setItemsinFiltered();
                            else {
                                //using `items` group to fetch all the elements (all elements are set into this group by default).
                                for (let i = 0; i < delegateModel.items.count; ++i) {
                                    var item = delegateModel.items.get(i);
                                    if (item.model.name.toLowerCase().includes(searchTextLower))
                                        item.inFiltered = true;
                                    else
                                        item.inFiltered = false;
                                }
                            }
                            listView.visible = filteredGroup.count > 0;
                            noResultsFoundLabel.visible = !listView.visible;
                        }
                    }
                    // delegate model used to implement the filtering functionality, by setting visible elements in the `filtered` group.
                    model: DelegateModel {
                        id: delegateModel


                        // switch between controller model based on the currentVisibleListView
                        model: internal.currentVisibileListView
                               === FloorFilter.VisibleListView.Site ? controller.sites : controller.facilities

                        // elements belonging to this group are visible.
                        filterOnGroup: "filtered"
                        groups: [
                            DelegateModelGroup {
                                id: filteredGroup
                                name: "filtered"
                                includeByDefault: true
                            }
                        ]
                        // sort the items in filteredGroup when in facility view.
                        items.onChanged: {
                            if(internal.currentVisibileListView === FloorFilter.VisibleListView.Facility){
                                var elements = [];
                                // populate js array
                                for(var i = 0; i < filteredGroup.count; ++i){
                                    elements.push(filteredGroup.get(i));
                                }
                                // utilize the sort function on the Array.
                                elements.sort(function(first, second){
                                    if(first.model.name < second.model.name)
                                        return -1;
                                    else if(first.model.name > second.model.name)
                                        return 1;
                                    return 0;
                                });

                                // move the elements in the filteredGroup using the index from the js Array.
                                for(i = 0; i < elements.length; ++i) {
                                    filteredGroup.move(elements[i].filteredIndex, i, 1);
                                }
                            }
                        }
                        delegate: RadioDelegate {
                            id: radioDelegate
                            // if listView is larger than the delegates, resize them to match the listview.
                            width : Math.max(implicitWidth, listView.width)
                            // facility might not have a parentSite, guarding from it.
                            property var parentSiteName: model.parentSiteName ?? ""
                            // highlight only set by the modelId comparison, not by the user click. Otherwise double selected facility could happen.
                            checkable: true
                            checked: highlighted
                            highlighted: internal.currentVisibileListView === FloorFilter.VisibleListView.Site ?
                                                                                     model.modelId === internal.selectedSiteId : model.modelId === internal.selectedFacilityId
                            onHighlightedChanged: listView.currentIndex = index;
                            // show parentSiteName once `all Sites` button is clicked (selectedSiteRespected-> false).
                            text: model.name + (model.parentSiteName && !controller.selectedSiteRespected ? "<br/>" + parentSiteName : "")

                            onClicked: {
                                // switch to facility view
                                if (internal.currentVisibileListView
                                        === FloorFilter.VisibleListView.Site) {
                                    controller.setSelectedSiteId(model.modelId);
                                    controller.zoomToSite(model.modelId);
                                    internal.currentVisibileListView
                                            = FloorFilter.VisibleListView.Facility;
                                } // switch to level view
                                else if (internal.currentVisibileListView
                                         === FloorFilter.VisibleListView.Facility) {
                                    controller.setSelectedFacilityId(model.modelId);
                                    // manually call the setSelectedSiteId, so it is sure that `allSites` mode is handled both on the cpp and qml versions.
                                    if(model.parentSiteName != null && internal.selectedFacilityId != null)
                                        controller.setSelectedSiteId(model.parentSiteId);
                                    // hide the whole facilityview
                                    buildingMenuButton.checked = false;
                                    closeButton.checked = false;
                                    controller.zoomToFacility(model.modelId);
                                }
                            }
                        }
                    }
                }

                ToolSeparator {
                    visible: showAllFacilities.visible
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    orientation: Qt.Horizontal
                }

                Button {
                    id: showAllFacilities
                    objectName: "showAllFacilities"
                    text: qsTr("All sites")
                    Layout.alignment: Qt.AlignCenter
                    Layout.columnSpan: 3
                    Layout.margins: 5
                    Layout.fillWidth: true
                    flat: true
                    visible: internal.currentVisibileListView === FloorFilter.VisibleListView.Site

                    onClicked: {
                        // show all facilities from all sites.
                        controller.selectedSiteRespected = false;
                        internal.currentVisibileListView = FloorFilter.VisibleListView.Facility;
                    }
                }

                Label {
                    id: siteLabel
                    objectName: "siteLabel"
                    visible : controller.sites.count > 0
                    font.bold: internal.currentVisibileListView === FloorFilter.VisibleListView.Site
                    Layout.fillWidth: true
                    // the two titles are close to each others
                    Layout.topMargin: 5
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    horizontalAlignment: Text.AlignHCenter
                    text: controller.selectedSite ? controller.selectedSite.name : qsTr("Select a Site")
                }

                Label {
                    objectName: "facilityLabel"
                    font.bold: internal.currentVisibileListView
                               === FloorFilter.VisibleListView.Facility
                    Layout.fillWidth: true
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    Layout.bottomMargin: 5
                    // if no site label, set top margin. Otherwise is squished
                    Layout.topMargin: !siteLabel.visible ? 5 : 0
                    horizontalAlignment: Text.AlignHCenter
                    text: controller.selectedFacility ? controller.selectedFacility.name : qsTr("Select a Facility")
                }

                Button {
                    objectName: "closerListView"
                    Layout.rowSpan: 2
                    Layout.fillHeight: true
                    display: AbstractButton.IconOnly
                    flat: true
                    //removing all paddings and spacing from component so icon will fill completely the button
                    topPadding: 0
                    bottomPadding: 0
                    leftPadding: 0
                    rightPadding: 0
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: 32
                    icon {
                        width: 32
                        height: 32
                        source: "qrc:/Esri/ArcGISRuntime/Toolkit/x.svg"
                        color: siteLabel.color
                    }
                    onClicked: buildingMenuButton.checked = false
                }
            }
        }
    }

    /*!
      \brief Types of elements shown in the \c ListView.
      \value VisibleListView.Site Site contains Facilities
      \value VisibleListView.Facility Facility contains levels.
      Default is \c VisibleListView.Site
    */
    // used to switch between site and facilities listviews and models.
    enum VisibleListView {
        Site,
        Facility
    }

    /*!
      \brief Enums to generalize the current \c FloorFilter position relative to its parent.
      4 different options, created by tracing 2 lines passing by the parent center point and its \c {width / 2} or its \c {height / 2 points}.
      \value ParentPosition.UpperLeft Top left square.
      \value ParentPosition.UpperRight Top right square.
      \value ParentPosition.LowerRight Bottom right square.
      \value ParentPosition.LowerLeft Bottom left square.
    */
    enum ParentPosition {
        UpperLeft,
        UpperRight,
        LowerRight,
        LowerLeft
    }

    QtObject {
        id: internal
        objectName: "internal"
        // expands/collapse icons, showing long/short name.
        property bool collapsedIcons : true
        // set the initial view as the facility view in case of single site or no sites.
        property int currentVisibileListView: floorFilter.controller.sites.count <= 1 ? FloorFilter.VisibleListView.Facility : FloorFilter.VisibleListView.Site
        property string selectedFacilityId: controller.selectedFacilityId
        property string selectedSiteId: controller.selectedSiteId
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
        // position of floorfilter related to its parent.
        property int parentPosition: {
            if (centerFloorFilterX < centerParentX) {
                if (centerFloorFilterY < centerParentY)
                    FloorFilter.ParentPosition.UpperLeft;
                else
                    FloorFilter.ParentPosition.LowerLeft;
            } else {
                if (centerFloorFilterY < centerParentY)
                    FloorFilter.ParentPosition.UpperRight;
                else
                    FloorFilter.ParentPosition.LowerRight;
            }
        }
        onCurrentVisibileListViewChanged: {
            // reset the search text when the currentListView is changed
            searchTextField.text = ""
        }
    }
}
