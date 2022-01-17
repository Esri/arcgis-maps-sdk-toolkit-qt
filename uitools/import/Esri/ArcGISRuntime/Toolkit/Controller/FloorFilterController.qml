
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

    property var geoView

    property GeoModel geoModel
    property FloorManager floorManager
    //refresh()?
    property string selectedLevelId

    property string selectedFacilityId

    property string selectedSiteId

    property ListModel levels: ListModel {}

    property ListModel facilities: ListModel {}

    property ListModel sites: ListModel {}

    onFacilitiesChanged: {

    }

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
            console.error("site id not found")
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
    }

    function onSelectedChanged() {}

    function filterFacilities(query) {
        filteredFacilities.clear()
        console.log(facilities.count)
        for (var i = 0; i < facilities.count; ++i) {
            const facility = facilities.get(i)
            if (facility.name.includes(query))
                filteredFacilities.append(facility)
        }
    }

    property Connections geoModelConn: Connections {
        target: geoView ? (geoView.scene ? geoView.scene : (geoView.map ? geoView.map : null)) : null
        ignoreUnknownSignals: true
        function onLoadStatusChanged() {
            console.log("map status: ", geoView.map.loadStatus)
            // load floormanager after map has been loaded
            if (geoView.map.loadStatus === Enums.LoadStatusLoaded) {
                floorManager = geoView.map.floorManager
                floorManager.load()
            }
        }
    }

    property Connections floorManagerConn: Connections {
        target: floorManager
        ignoreUnknownSignals: true
        function onLoadStatusChanged() {
            console.log("floormanager status (0 loaded)",
                        floorManager.loadStatus)
            if (floorManager.loadStatus === Enums.LoadStatusLoaded) {
                // load the listmodels
                populateSites(floorManager.sites)
            }
        }
    }

    onGeoViewChanged: {
        //new geoView added, load the FloorManager
        geoModel = geoView.map
    }

    function populateSites(listSites) {
        for (var i = 0; i < listSites.length; ++i) {
            let site = listSites[i]
            sites.append({
                             "name": site.name,
                             "modelId": site.siteId
                         })
        }
        console.log(sites.count)
        console.log(sites.get(0).name)
    }

    function populateFacilities(listFacilities) {
        for (var i = 0; i < listFacilities.length; ++i) {
            let facility = listFacilities[i]
            facilities.append({
                                  "name": facility.name,
                                  "modelId": facility.facilityId
                              })
        }
    }

    function populateLevels(listLevels) {
        for (var i = 0; i < listLevels.length; ++i) {
            let level = listLevels[i]
            levels.append({
                              "shortName": level.shortName,
                              "modelId": level.levelId
                          })
        }
    }

    onFloorManagerChanged: console.log("manager changed")

    /*! internal */
    property QtObject internal: QtObject {
        // used keep track of last level selected and toggle its visibility
        property FloorLevel selectedLevel
        // used to update the view with their names
        property FloorFacility selectedFacility
        property FloorSite selectedSite
    }
}
