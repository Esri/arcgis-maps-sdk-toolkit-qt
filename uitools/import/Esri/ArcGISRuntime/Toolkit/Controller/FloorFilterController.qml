
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

// sites are optional, facilities are optional. a Facility might have no levels (restricted access facility)
QtObject {
    id: controller

    property GeoView geoView

    property GeoModel geoModel: {
        // check geoView has been set
        if (!geoView)
            return null
        if (typeof (geoView.map) !== "undefined")
            return geoView.map
        else if (typeof (geoView.scene) !== "undefined")
            return geoView.scene
        return null
    }

    property int updateLevelsMode: FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder

    property bool autoselectSingleFacilitySite

    property FloorManager floorManager

    //refresh()?
    property string selectedLevelId

    property string selectedFacilityId

    function setSelectedFacilityId(facilityId) {
        let idx = findElementIdxById(facilityId, floorManager.facilities,
                                     "facilityId")
        // check that the current site is the selected facility selected. Otherwise the click came from a populateAllFacilities
        var facility = floorManager.facilities[idx]
        if (!internal.selectedSite
                || facility.site.siteId !== internal.selectedSite.siteId) {
            // selection of facility came after click of populateAllFacilities.
            // this also sets the internal.selectedSite
            selectedSiteId = facility.site.siteId
        }
        selectedFacilityId = facilityId
    }

    property string selectedSiteId

    function setSelectedSiteId(siteId) {
        selectedSiteId = siteId
    }

    property ListModel levels: ListModel {}

    property ListModel facilities: ListModel {}

    property ListModel sites: ListModel {}

    property alias selectedSite: internal.selectedSite
    property alias selectedFacility: internal.selectedFacility
    property alias selectedLevel: internal.selectedLevel

    // iterates over \a listElements to find an element with \a id.
    //\a variableIdName is used to access the correct method name in each list (siteId, facilityId, levelId)
    function findElementIdxById(id, listElements, variableIdName) {
        let idx
        for (var i = 0; i < listElements.length; ++i) {
            let elementId = listElements[i][variableIdName]
            if (elementId === id) {
                idx = i
                break
            }
        }
        return idx
    }

    onSelectedLevelIdChanged: {
        // find the list element idx of the changed levelId
        let idx = findElementIdxById(selectedLevelId, floorManager.levels,
                                     "levelId")
        if (idx === undefined) {
            console.error("site id not found")
            return
        }

        // show level
        let level = floorManager.levels[idx]
        if (internal.selectedLevel !== null)
            internal.selectedLevel.visible = false
        internal.selectedLevel = level
        internal.selectedLevel.visible = true

        if (updateLevelsMode
                === FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder) {
            setVisibleLevelsMatchingVerticalOrder()
        }

        onSelectedChanged()
    }

    onSelectedFacilityIdChanged: {
        // find the list element idx of the changed facilityId
        let idx = findElementIdxById(selectedFacilityId,
                                     floorManager.facilities, "facilityId")
        if (idx === undefined) {
            console.error("facility id not found, resetting current facility")
            internal.selectedFacility = null
            return
        }
        internal.selectedFacility = floorManager.facilities[idx]

        populateLevels(floorManager.facilities[idx].levels)
        onSelectedChanged()
    }

    onSelectedSiteIdChanged: {
        // find the list element idx of the changed siteId
        let idx = findElementIdxById(selectedSiteId,
                                     floorManager.sites, "siteId")
        if (idx === undefined) {
            console.error("site id not found")
            return
        }
        internal.selectedSite = floorManager.sites[idx]
        populateFacilities(floorManager.sites[idx].facilities)
        onSelectedChanged()
        selectedFacilityId = ""
    }

    function onSelectedChanged() {}

    // geomodel loaded connection
    property Connections geoModelConn: Connections {
        target: geoModel
        function onLoadStatusChanged() {
            console.log("geomodel loaded: ", geoModel)
            // load floormanager after map has been loaded
            if (geoModel.loadStatus === Enums.LoadStatusLoaded) {
                floorManager = geoModel.floorManager
                floorManager.load()
            }
        }
    }

    property Connections floorManagerConn: Connections {
        target: floorManager
        function onLoadStatusChanged() {
            console.log("floormanager status (0 loaded)",
                        floorManager.loadStatus)
            if (floorManager.loadStatus === Enums.LoadStatusLoaded) {
                // load the listmodels
                populateSites(floorManager.sites)
            }
        }
    }

    function populateSites(listSites) {
        sites.clear()
        for (var i = 0; i < listSites.length; ++i) {
            let site = listSites[i]
            sites.append({
                             "name": site.name,
                             "modelId": site.siteId
                         })
        }
        // case single site: autoselect it and set the boolean used by the view
        if (autoselectSingleFacilitySite && listSites.length === 1) {
            internal.singleSite = true
            let site = listSites[0]
            selectedSiteId = site.siteId
        }
    }

    function populateFacilities(listFacilities) {
        facilities.clear()
        for (var i = 0; i < listFacilities.length; ++i) {
            let facility = listFacilities[i]
            facilities.append({
                                  "name": facility.name,
                                  "modelId": facility.facilityId
                              })
        }
        // case single facility: autoselect it
        if (autoselectSingleFacilitySite && listFacilities.length === 1) {
            selectedFacilityId = listFacilities[0].facilityId
        }
    }

    function populateAllFacilities() {
        var listFacilities = floorManager.facilities
        populateFacilities(listFacilities)
        // setting view site name as ""
        // internal.selectedSite = null
    }

    // populate levels in reverse order. Levels numbers in ascending order from component's bottom section.
    function populateLevels(listLevels) {
        levels.clear()
        let selectedLevel = ""
        let levelsExtracted = []
        for (var i = listLevels.length - 1; i >= 0; --i) {
            let level = listLevels[i]
            levelsExtracted.push(level)

            if (level.verticalOrder === 0) {
                selectedLevel = level.levelId
                selectedLevelId = level.levelId
            }
        }
        // sorting higher levels first
        levelsExtracted.sort(function (first, second) {
            return second.verticalOrder - first.verticalOrder
        })

        levelsExtracted.forEach(levelExtracted => {
                                    levels.append({
                                                      "shortName": levelExtracted.shortName,
                                                      "longName": levelExtracted.longName,
                                                      "modelId": levelExtracted.levelId
                                                  })
                                })
        // no suitable vertical order found. second check to from facilities with no levels
        if (!selectedLevel && listLevels[0])
            selectedLevelId = listLevels[0].levelId
    }

    function zoomToCurrentFacility() {
        if (!selectedFacilityId) {
            console.error("no facility yet selected")
            return
        }
        zoomToFacility(selectedFacilityId)
    }

    function zoomToCurrentSite() {
        if (!selectedSiteId) {
            console.error("no site yet selected")
            return
        }
        zoomToSite(selectedSiteId)
    }

    function zoomToFacility(facilityId) {
        let idx = findElementIdxById(selectedFacilityId,
                                     floorManager.facilities, "facilityId")
        let facility = floorManager.facilities[idx]
        const extent = facility.geometry.extent
        zoomToEnvelope(extent)
    }

    function zoomToSite(siteId) {
        let idx = findElementIdxById(selectedSiteId,
                                     floorManager.sites, "siteId")
        let site = floorManager.sites[idx]
        const extent = site.geometry.extent
        zoomToEnvelope(extent)
    }

    function zoomToEnvelope(envelope) {
        var builder = ArcGISRuntimeEnvironment.createObject('EnvelopeBuilder', {
                                                                "geometry": envelope
                                                            })
        builder.expandByFactor(internal.zoom_padding)
        var newViewpoint = ArcGISRuntimeEnvironment.createObject(
                    'ViewpointExtent', {
                        "extent": builder.geometry
                    })
        geoView.setViewpoint(newViewpoint)
    }


    /*!
      Setting the levels visible if they match the current selected level vertical order.
    */
    function setVisibleLevelsMatchingVerticalOrder() {
        for (var level in levels) {
            level.verticalOrder = level.verticalOrder === selectedLevel.verticalOrder
        }
    }

    onFloorManagerChanged: console.log("manager changed")

    enum UpdateLevelsMode {
        SingleLevel,
        AllLevelsMatchingVerticalOrder
    }

    /*! internal */
    property QtObject internal: QtObject {
        id: internal
        // used keep track of last level selected and toggle its visibility
        property FloorLevel selectedLevel
        // used to update the view with their names. _q could only store the name string
        property FloorFacility selectedFacility
        property FloorSite selectedSite
        // used to set the default initial view. if singleSite : true->facility view and site already selected
        property bool singleSite: false //_q should be better that the view checks the sites.length at changes the default view based on it?
        readonly property double zoom_padding: 1.5
    }
}
