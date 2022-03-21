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

import QtQuick 2.12
import Esri.ArcGISRuntime 100.14

/*!
  \qmltype FloorFilterController
  \inqmlmodule Esri.ArcGISRuntime.Toolkit
  \since Esri.ArcGISRuntime 100.14
  \ingroup ArcGISQtToolkitUiQmlControllers
  \brief The controller part of a FloorFilter. This class handles the toggling of selected levels' visibility
  associated to the \c FloorFilter inputted \c GeoModel flooraware layers.
  Automatically loads the \c GeoModel and its associated \c FloorManager.
*/
QtObject {
    // sites are optional, facilities are optional. a Facility might have no levels (restricted access facility)
    id: controller

    /*!
      \qmlproperty GeoView geoView
      \brief The \c GeoView the controller is listening to for viewpoint changes.
      Controller sets the \c GeoView viewpoint when selecting site/facility.
    */
    property GeoView geoView

    /*!
      \qmlproperty enumeration automaticSelectionMode
      \brief The mode selected defines how the \c FloorFilter updates its selection as the user navigates the connected \c GeoView.
      \value AutomaticSelectionMode.Never Viewpoint navigation does not affect the currently selected facility.
      \value AutomaticSelectionMode.Always When the geoview's current viewpoint updates, the controller tests to see if the facility interect the viewpoint,
      and selects it if so. If no facility intersects the viewpoint, then the current facility is deselected.
      \value AutomaticSelectionMode.AlwaysNonClearing Variant of \c Always, but if there is no facility to select within the viewpoint then we do not
      deselect the current viewpoint.

      Default is \c{AutomaticSelectionMode.Always}
    */
    property int automaticSelectionMode

    /*!
      \brief Tries to update the current selected site,facility and level based on the current viewcenter point geometry.
      In case a site/facility is in the current viewcenter point, it is selected and its default level shown.
      The \l automaticSelectionMode is used to decide the specific behavior of level selection.
    */
    function tryUpdateSelection() {
        var viewpointCenter = geoView.currentViewpointCenter;

        if (automaticSelectionMode === FloorFilterController.AutomaticSelectionMode.Never || !viewpointCenter || isNaN(viewpointCenter.targetScale)) {
            return;
        }

        var floorManager = controller.floorManager;
        var targetScale = 0;

        if (floorManager)
            targetScale = controller.floorManager.siteLayer ? controller.floorManager.siteLayer.minScale : 0;

        if (targetScale === 0)
            targetScale = 4300;

        if (viewpointCenter.targetScale > targetScale) {
            if (automaticSelectionMode === FloorFilterController.AutomaticSelectionMode.Always) {

                setSelectedSiteId("");
                setSelectedFacilityId("");
                setSelectedLevelId("");
            }
            // Assumption: if too zoomed out to see sites, also too zoomed out to see facilities
            return;
        }

        if(!floorManager)
            return;

        let selectSite
        for (var i = 0; i < floorManager.sites.length; ++i) {
            if (GeometryEngine.intersects(
                        floorManager.sites[i].geometry.extent,
                        viewpointCenter.center)) {
                selectSite = floorManager.sites[i];
                break;
            }
        }

        if (selectSite !== undefined) {
            setSelectedSiteId(selectSite.siteId);
        } else if (automaticSelectionMode === FloorFilterController.AutomaticSelectionMode.Always) {
            setSelectedSiteId("");
        }

        targetScale = floorManager.facilityLayer ? floorManager.facilityLayer.minScale : 0
        if (targetScale === 0)
            targetScale = 1500;

        if(viewpointCenter.targetScale > targetScale)
            return;

        let selectFacility
        for (i = 0; i < floorManager.facilities.length; ++i) {
            let facility;
            if (GeometryEngine.intersects(
                        floorManager.facilities[i].geometry.extent,
                        viewpointCenter.center)) {
                selectFacility = floorManager.facilities[i];
                break;
            }
        }
        if (selectFacility !== undefined) {
            setSelectedFacilityId(selectFacility.facilityId);
        } else if (automaticSelectionMode === FloorFilterController.AutomaticSelectionMode.Always) {
            setSelectedFacilityId("");
            setSelectedLevelId("");
        }
    }

    /*!
      \qmlproperty GeoModel geoModel
      \brief The geomodel the controller is listening for \c FloorManager changes.
    */
    property GeoModel geoModel: {
        // check geoView has been set
        if (!geoView)
            return null;
        if (typeof (geoView.map) !== "undefined")
            return geoView.map;
        else if (typeof (geoView.scene) !== "undefined")
            return geoView.scene;
        return null;
    }

    onGeoModelChanged: {
        // make sure that the geomodel is loaded.
        geoModel.load();
    }

    /*!
      \qmlproperty int updateLevelsMode
      \brief The mode that defines how levels are made visible/invisible in the geoView.
      \value UpdateLevelsMode.AllLevelsMatchingVerticalOrder When a level is selected, all levels with matching vertical order are made visible, all other levels are invisible.
      \value UpdateLevelsMode.SingleLevel All levels with \c{verticalOrder == 0} are set to visible, the currently selected level is also visible, all other levels are set to invisible. This
      matches the JavaScript Widget FloorFilter functionality.

      Default is \c{UpdateLevelsMode.AllLevelsMatchingVerticalOrder}
    */
    property int updateLevelsMode: FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder

    /*!
      \qmlproperty FloorManager floorManager
      \brief The FloorManager associated to the \l geoModel.
      It can be null if no associated geoModel is available or is not loaded.
    */
    property FloorManager floorManager : geoModel? geoModel.floorManager : null

    onFloorManagerChanged: {
        internal.onFloorManagerLoaded();
    }

    /*!
      \brief Load the \l floorManager associated with the \l geoModel and populate the sites.
      \sa populateSites
    */
    property Connections floorManagerConn: Connections {
        target: floorManager
        function onLoadStatusChanged() {
            internal.onFloorManagerLoaded();
        }
    }

    /*!
      \qmlproperty bool selectedSiteRespected
      \brief Repopulates the \l facilities model when toggled false.
      When the selected site is respected, the facilities list is limited to the facilities which match the selected site.
      If the selected site is not respected, then facilities will contain all facilities from all sites, regardless of the selected site.
      Default is {true}.
    */
    property bool selectedSiteRespected: true

    onSelectedSiteRespectedChanged: {
        if (!selectedSiteRespected) {
            populateAllFacilities();
        }
    }
    
    /*!
      \qmlproperty string selectedLevelId
      \brief Stores the current selected level Id.
      Default is {""}.
    */
    readonly property alias selectedLevelId: internal.selectedLevelId

    /*!
      \brief Sets the \a levelId as \l selectedLevelId.
      This will change the current visibile level on the \c GeoView.
      If \a levelId is empty string, current selected level is deseleted and its visibility is set to false.
      Resets the previous level visibility. \l updateLevelsMode modifies the behavior of selecting level.

    */
    function setSelectedLevelId(levelId) {
        internal.selectedLevelId = levelId;
    }

    /*!
      \qmlproperty string selectedFacilityId
      \brief Stores the current selected facility Id.
      Default is {""}.
    */
    readonly property alias selectedFacilityId: internal.selectedFacilityId

    /*!
      \brief Sets the \a facilityId as \l selectedFacilityId.
      This will load the \l levels with the levels withing the facility and
      reset by selecting \l selectedLevelId with level matching vertical ordering == 0.
      Set empty string to reset the facility.
      \sa setSelectedLevelId
    */
    function setSelectedFacilityId(facilityId) {
        // avoid findind null idx, immediately return if empty string
        if(facilityId === ""){
            internal.selectedFacilityId = facilityId;
            return;
        }

        let idx = findElementIdxById(facilityId,
                                     FloorFilterController.TypeElement.Facility);
        if (idx == null) {
            console.error("not found facility");
            internal.selectedFacilityId = "";
            return;
        }

        // check that the current site is the selected facility selected's parent site. Otherwise the click came from a populateAllFacilities
        var facility = floorManager.facilities[idx];

        if (facility.site != null && (!internal.selectedSite || facility.site.siteId !== internal.selectedSite.siteId) ) {
            // selection of facility came after click of populateAllFacilities.
            // this also sets the internal.selectedSite
            internal.selectedSiteId = facility.site.siteId;
        }
        internal.selectedFacilityId = facilityId;
    }

    /*!
      \qmlproperty string selectedSiteId
      \brief Stores the current selected site Id.
      Default is {""}.
    */
    readonly property alias selectedSiteId: internal.selectedSiteId

    /*!
      \brief Sets the \a siteId as \l selectedSiteId.
      This will load the \l facilities with the facilities within the site and reset \l selectedFacilityId.
      Does not repopulate facilities in case of not \l selectedSiteRespected.
    */
    function setSelectedSiteId(siteId) {
        internal.selectedSiteId = siteId;
    }

    /*!
      \qmlproperty ListModel levels
      \brief Stores the levels in the currently selected facility in alphabetical order.
      If not \l selectedSiteRespected, all levels are stored.
      \sa selectedLevelId
    */
    readonly property ListModel levels: ListModel {}

    /*!
      \qmlproperty ListModel facilities
      \brief Stores the facilities in the currently selected site.
      \sa selectedSiteId
    */
    readonly property ListModel facilities: ListModel {}

    /*!
      \qmlproperty ListModel sites
      \brief Stores all the sites in the current \l floorManager.
      \sa selectedSiteId
    */
    readonly property ListModel sites: ListModel {}

    readonly property alias selectedSite: internal.selectedSite
    readonly property alias selectedFacility: internal.selectedFacility
    readonly property alias selectedLevel: internal.selectedLevel


    // iterates over \a listElements to find an element with \a id.
    //\a variableIdName is used to access the correct method name in each list (siteId, facilityId, levelId)
    /*!
      \internal
      \brief Finds an element index in the internal models using its \a id.
      \a typeElement is one of the \c FloorFilterController.TypeElement values.
      It automatically iterates on the correct internal model based on the \a typeElement passed.
      \value TypeElement.Level level
      \value TypeElement.Facility facility
      \value TypeElement.Site site
      Returns the id or \c null if no matching element id was found in the model.
    */
    function findElementIdxById(id, typeElement) {
        var model;
        var variableIdName;
        switch (typeElement) {
        case FloorFilterController.TypeElement.Level:
            model = floorManager.levels;
            variableIdName = "levelId";
            break;
        case FloorFilterController.TypeElement.Facility:
            model = floorManager.facilities;
            variableIdName = "facilityId";
            break;
        case FloorFilterController.TypeElement.Site:
            model = floorManager.sites;
            variableIdName = "siteId";
            break;
        default:
            console.error("no element type matched");
            return null;
        }

        for (var i = 0; i < model.length; ++i) {
            let elementId = model[i][variableIdName];
            if (elementId === id) {
                return i;
            }
        }
        return null;
    }

    onSelectedLevelIdChanged: {
        if (selectedLevelId === "") {
            setVisibilityCurrentLevel(false);
        }
        // find the list element idx of the changed levelId
        let idx = findElementIdxById(selectedLevelId,
                                     FloorFilterController.TypeElement.Level);
        if (idx == null) {
            console.error("level id not found, resetting current level");
            if(internal.selectedLevelId === "")
              internal.selectedLevel.visible = false;
            internal.selectedLevel = null;
            return;
        }

        // show level
        let level = floorManager.levels[idx];
        if (internal.selectedLevel !== null)
            internal.selectedLevel.visible = false;
        internal.selectedLevel = level;
        internal.selectedLevel.visible = true;

        if (updateLevelsMode
                === FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder) {
            resetLevelsVisibility(internal.selectedLevel.verticalOrder);
        }

        onSelectedChanged();
    }

    onSelectedFacilityIdChanged: {
        // find the list element idx of the changed facilityId
        let idx = findElementIdxById(selectedFacilityId,
                                     FloorFilterController.TypeElement.Facility);
        // checking both null and undefined
        if (idx == null) {
            console.error("facility id not found, resetting current facility");
            internal.selectedFacility = null;
            levels.clear();
            return;
        }
        internal.selectedFacility = floorManager.facilities[idx];
        populateLevels(floorManager.facilities[idx].levels);
        // reset the levels visibilty to vertical order 0 once a facility is selected.
        resetLevelsVisibility(0);
        onSelectedChanged();
    }

    onSelectedSiteIdChanged: {
        // find the list element idx of the changed siteId
        let idx = findElementIdxById(selectedSiteId,
                                     FloorFilterController.TypeElement.Site);
        if (idx == null) {
            console.error("site id not found");
            internal.selectedSite = null;
            facilities.clear();
            return;
        }

        internal.selectedSite = floorManager.sites[idx];

        // dont populate facilities if they are total number and we are ignoring the current selected site
        if (!selectedSiteRespected
                && facilities.count === floorManager.facilities.length)
            return;
        populateFacilities(floorManager.sites[idx].facilities);
        onSelectedChanged();
        internal.selectedFacilityId = "";
    }

    /*!
      \brief Set the \a visibility to the \c selectedLevel.
    */
    function setVisibilityCurrentLevel(visibility) {
        if(internal.selectedLevel)
            internal.selectedLevel.visible = visibility;
    }

    /*!
      \brief Setting the levels visible that match the \a verticalOrder
    */
    function resetLevelsVisibility(verticalOrder){
        for(var i = 0; i < floorManager.levels.length; ++i) {
            var level = floorManager.levels[i];
            level.visible = level.verticalOrder === verticalOrder;
        }
    }

    function onSelectedChanged() {}

    // geomodel loaded connection
    /*!
      \qmlproperty Connections geoModelConn
      \brief load the \c FloorManager associated with the \l geoModel.
    */
    property Connections geoModelConn: Connections {
        target: geoModel
        function onLoadStatusChanged() {
            // load floormanager after map has been loaded
            if (geoModel.loadStatus === Enums.LoadStatusLoaded) {
                floorManager = geoModel.floorManager;
                floorManager.load();
            }
        }
    }

    /*!
      \brief Signal emitted once the \l floorManager has been successfully loaded.
    */
    signal loaded()

    /*!
      \brief Clears and repopulates \l sites with \a listSites.
      Expects the site elements to have properties `name` and `siteId`.
      If only one site exists in the model, then we select it automatically. \sa setSelectedSiteId.
    */
    function populateSites(listSites) {
        sites.clear();
        for (var i = 0; i < listSites.length; ++i) {
            let site = listSites[i];
            sites.append({
                             "name": site.name,
                             "modelId": site.siteId
                         });
        }
        // case single site: autoselect it and set the boolean used by the view
        if (listSites.length === 1) {
            internal.singleSite = true;
            let site = listSites[0];
            setSelectedSiteId(site.siteId);
        }
    }

    /*!
      \brief Clears and repopulates \l facilities with \a listFacilities.
      Expects the facility elements to have `name`, `facilityId` and `site.name` properties.
      Model \l facilities elements are alphabetically ordered.
      If only on facility exists we select that facility, otherwise we do not automatically select
      a facility.
    */
    function populateFacilities(listFacilities) {
        facilities.clear();
        let facilitiesExtracted = Array.from(listFacilities);
        facilitiesExtracted.sort(function (first, second) {
            if (first.name < second.name)
                return -1;
            else if (first.name > second.name)
                return 1;
            return 0;
        });

        facilitiesExtracted.forEach(facility => {
                                        let obj = { "name": facility.name,
                                            "modelId": facility.facilityId};
                                        if(facility.site != null)
                                            obj["parentSiteName"] = facility.site.name;
                                        facilities.append(obj);
                                    });

        // case single facility: autoselect it
        if (listFacilities.length === 1) {
            internal.selectedFacilityId = listFacilities[0].facilityId;
        }
    }


    /*!
     \internal
     \brief Populate the \l facilities model with all the facilities fetched from the \c floorManager.
     \sa populateFacilities
    */
    function populateAllFacilities() {
        var listFacilities = floorManager.facilities;
        populateFacilities(listFacilities);
    }

    // populate levels in reverse order. Levels numbers in ascending order from component's bottom section.
    /*!
      \brief Clears and repopulates the \l levels with \a listLevels.
      Expects the levels elements to have `shortName`, `longName`, `levelId` properties.
      Levels are sorted in terms of their vertical order, and a default level is selected, favouring the
      level with verticalOrder == 0.
    */
    function populateLevels(listLevels) {
        levels.clear();
        let selectedLevel = "";
        let levelsExtracted = [];
        for (var i = listLevels.length - 1; i >= 0; --i) {
            let level = listLevels[i];
            levelsExtracted.push(level);

            if (level.verticalOrder === 0) {
                selectedLevel = level.levelId;
                internal.selectedLevelId = level.levelId;
            }
        }
        // sorting higher levels first
        levelsExtracted.sort(function (first, second) {
            return second.verticalOrder - first.verticalOrder;
        });

        levelsExtracted.forEach(levelExtracted => {
                                    levels.append({
                                                      "shortName": levelExtracted.shortName,
                                                      "longName": levelExtracted.longName,
                                                      "modelId": levelExtracted.levelId
                                                  });
                                });
        // no suitable vertical order found. second check to from facilities with no levels
        if (!selectedLevel && listLevels[0])
            internal.selectedLevelId = listLevels.length ? listLevels[0].levelId : "";
    }

    /*!
      \brief On the GeoView, zooms to the facility with ID matching \a facilityId.
    */
    function zoomToFacility(facilityId) {
        let idx = findElementIdxById(facilityId,
                                     FloorFilterController.TypeElement.Facility);
        let facility = floorManager.facilities[idx];
        const extent = facility.geometry.extent;
        zoomToEnvelope(extent);
    }

    /*!
      \brief On the \c GeoView, zooms to the facility with ID matching \a siteId.
    */
    function zoomToSite(siteId) {
        let idx = findElementIdxById(siteId,
                                     FloorFilterController.TypeElement.Site);
        let site = floorManager.sites[idx];
        const extent = site.geometry.extent;
        zoomToEnvelope(extent);
    }

    /*!
      \internal
      \brief Signal emitted once \c geoModel has completed a setViewpoint.
      This signal is hooked up to setViewpoint changes requested by the \c FloorFilter. Doing so, it is possible
      to understand which signal comes from \c FloorFilter and which from user panning the \c GeoModel.
    */
    signal doneViewpointChanged

    // signal used to set active again the \c Connections geoViewConnections pointing to onSetViewpoint.
    /*!
      \internal
      Re-enables \l geoViewConnections.
    */
    onDoneViewpointChanged: {
        controller.internal.geoViewConnections.enabled = true;
        geoView.onSetViewpointCompleted.disconnect(
                    controller.doneViewpointChanged);
    }

    function zoomToEnvelope(envelope) {
        var builder = ArcGISRuntimeEnvironment.createObject('EnvelopeBuilder', {
                                                                "geometry": envelope
                                                            });
        builder.expandByFactor(internal.zoom_padding);
        var newViewpoint = ArcGISRuntimeEnvironment.createObject(
                    'ViewpointExtent', {
                        "extent": builder.geometry
                    });
        // disconnect temporarily the \c Connections geoViewConnections. This is done to ignore all the \c GeoView onSetViewpoint
        // events triggered from the floorfilter that would be triggered once the floorFilter zoom button is clicked (or zoomToEnvelope function called).
        controller.internal.geoViewConnections.enabled = false;
        // doneViewpoint signal sets active again the \c Connection onSetViewpoint.
        geoView.onSetViewpointCompleted.connect(controller.doneViewpointChanged);
        geoView.setViewpoint(newViewpoint);
    }

    /*!
      \brief The mode that defines how levels are made visible/invisible in the geoView.
      \value AllLevelsMatchingVerticalOrder When a level is selected, all levels with matching vertical order are made visible, all other levels are invisible.
      \value SingleLevel All levels with \c{verticalOrder == 0} are set to visible, the currently selected level is also visible, all other levels are set to invisible. This
      matches the JavaScript Widget FloorFilter functionality.
      \sa updateLevelsMode
    */
    enum UpdateLevelsMode {
        AllLevelsMatchingVerticalOrder,
        SingleLevel
    }

    /*!
      \brief The type of elements used in the internal models.
      \value TypeElement.Level
      \value TypeElement.Facility
      \value TypeElement.Site
    */
    enum TypeElement {
        Level,
        Facility,
        Site
    }

    /*!
      \brief The mode that defines how facilities are selected by viewpoint navigation.
      \value Never Viewpoint navigation does not affect the currently selected facility.
      \value Always When the geoview's current viewpoint updates, the controller tests to see if the facility interect the viewpoint,
      and selects it if so. If no facility intersects the viewpoint, then the current facility is deselected.
      \value AlwaysNonClearing Variant of \c Always, but if there is no facility to select within the viewpoint then we do not
      deselect the current viewpoint.
      \sa automaticSelectionMode
    */
    enum AutomaticSelectionMode {
        // Never update selection based on the GeoView's current viewpoint
        Never,
        // Always update selection based on the current viewpoint; clear the selection when the user navigates away
        Always,
        // Only update the selection when there is a new site or facility in the current viewpoint; don't clear selection when the user navigates away
        AlwaysNonClearing
    }

    /*! internal */
    property QtObject internal: QtObject {
        id: internal
        property string selectedLevelId
        property string selectedFacilityId
        property string selectedSiteId
        // used keep track of last level selected and toggle its visibility
        property FloorLevel selectedLevel
        // used to update the view with their names. _q could only store the name string
        property FloorFacility selectedFacility
        property FloorSite selectedSite
        // used to set the default initial view. if singleSite : true->facility view and site already selected
        property bool singleSite: false //_q should be better that the view checks the sites.length at changes the default view based on it?
        readonly property double zoom_padding: 1.5


        property Connections geoViewConnections: Connections {
            target: geoView
            function onViewpointChanged() {
                tryUpdateSelection();
            }
            // manually set true when floorManager is loaded
            enabled: false
        }

        function onFloorManagerLoaded() {
            if (floorManager.loadStatus === Enums.LoadStatusLoaded) {
                // load the listmodels
                if(floorManager.sites.length > 0)
                    populateSites(floorManager.sites);
                else {
                    // sites could be empty, just load the facilites
                    populateAllFacilities();
                }
                controller.loaded();
                controller.internal.geoViewConnections.enabled = true;
            }
            else if(floorManager.loadStatus === Enums.LoadStatusFailedToLoad)
                console.error("floorManager failed to load.");
            else {
                floorManager.load();
            }
        }
    }
}
