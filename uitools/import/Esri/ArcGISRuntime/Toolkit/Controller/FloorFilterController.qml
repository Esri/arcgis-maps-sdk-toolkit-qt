
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

    property FloorManager floorManager

    //refresh()?
    property string selectedLevelId

    property string selectedFacilityId

    property string selectedSiteId

    property ListModel levels: ListModel {}

    property ListModel facilities: ListModel {}

    property ListModel sites: ListModel {}

    property alias selectedSite: internal.selectedSite
    property alias selectedFacility: internal.selectedFacility

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
        onSelectedChanged()
    }

    onSelectedFacilityIdChanged: {
        // find the list element idx of the changed facilityId
        let idx = findElementIdxById(selectedFacilityId,
                                     floorManager.facilities, "facilityId")
        if (idx === undefined) {
            console.error("facility id not found")
            return
        }
        internal.selectedFacility = floorManager.facilities[idx]
        // check that the current site is the selected facility selected. Otherwise the click came from a populateAllFacilities
        var facility = floorManager.facilities[idx]
        if (facility.site.siteId !== internal.selectedSite.siteId) {
            // selection of facility came after click of populateAllFacilities.
            // this also sets the internal.selectedSite
            selectedSiteId = facility.site.siteId
        }

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
        if (listSites.length === 1) {
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
        for (var i = listLevels.length - 1; i >= 0; --i) {
            let level = listLevels[i]
            levels.append({
                              "shortName": level.shortName,
                              "modelId": level.levelId
                          })
        }
    }

    function zoomToFacility(facilityId) {
        let idx = findElementIdxById(selectedFacilityId,
                                     floorManager.facilities, "facilityId")
        let facility = floorManager.facilities[idx]
        const extent = facility.geometry.extent
        var builder = ArcGISRuntimeEnvironment.createObject('EnvelopeBuilder', {
                                                                "geometry": extent
                                                            })
        builder.expandByFactor(internal.zoom_padding)
        var newViewpoint = ArcGISRuntimeEnvironment.createObject(
                    'ViewpointExtent', {
                        "extent": builder.geometry
                    })
        geoView.setViewpoint(newViewpoint)
        console.log("set viewqpoint")
    }

    function zoomToEnvelope(envelope) {}

    onFloorManagerChanged: console.log("manager changed")

    /*! internal */
    property QtObject internal: QtObject {
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
