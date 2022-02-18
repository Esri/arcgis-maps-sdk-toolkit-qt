
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

    property FloorManager floorManager

    property bool selectedSiteResepected: true

    //refresh()?
    readonly property alias selectedLevelId: internal.selectedLevelId

    function setSelectedLevelId(levelId) {
        internal.selectedLevelId = levelId
    }

    readonly property alias selectedFacilityId: internal.selectedFacilityId

    function setSelectedFacilityId(facilityId) {
        let idx = findElementIdxById(facilityId,
                                     FloorFilterController.TypeElement.Facility)
        if (idx == null) {
            console.error("not found facility")
            return
        }

        // check that the current site is the selected facility selected. Otherwise the click came from a populateAllFacilities
        var facility = floorManager.facilities[idx]
        if (!internal.selectedSite
                || facility.site.siteId !== internal.selectedSite.siteId) {
            // selection of facility came after click of populateAllFacilities.
            // this also sets the internal.selectedSite
            internal.selectedSiteId = facility.site.siteId
        }
        internal.selectedFacilityId = facilityId
    }

    readonly property alias selectedSiteId: internal.selectedSiteId

    function setSelectedSiteId(siteId) {
        console.log("set selected site id")
        internal.selectedSiteId = siteId
    }

    property ListModel levels: ListModel {}

    property ListModel facilities: ListModel {}

    property ListModel sites: ListModel {}

    property alias selectedSite: internal.selectedSite
    property alias selectedFacility: internal.selectedFacility
    property alias selectedLevel: internal.selectedLevel

    // iterates over \a listElements to find an element with \a id.
    //\a variableIdName is used to access the correct method name in each list (siteId, facilityId, levelId)
    function findElementIdxById(id, typeElement) {
        var model
        var variableIdName
        switch (typeElement) {
        case FloorFilterController.TypeElement.Level:
            model = floorManager.levels
            variableIdName = "levelId"
            break
        case FloorFilterController.TypeElement.Facility:
            model = floorManager.facilities
            variableIdName = "facilityId"
            break
        case FloorFilterController.TypeElement.Site:
            model = floorManager.sites
            variableIdName = "siteId"
            break
        default:
            console.error("no element type matched")
            return null
        }

        for (var i = 0; i < model.length; ++i) {
            let elementId = model[i][variableIdName]
            if (elementId === id) {
                return i
            }
        }
        return null
    }

    onSelectedLevelIdChanged: {
        // find the list element idx of the changed levelId
        let idx = findElementIdxById(selectedLevelId,
                                     FloorFilterController.TypeElement.Level)
        if (idx == null) {
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
                                     FloorFilterController.TypeElement.Facility)
        // checking both null and undefined
        if (idx == null) {
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
                                     FloorFilterController.TypeElement.Site)
        if (idx == null) {
            console.error("site id not found")
            return
        }

        internal.selectedSite = floorManager.sites[idx]

        // dont populate facilities if they are total number and we are ignoring the current selected site
        if (!selectedSiteResepected
                && facilities.count === floorManager.facilities.length)
            return
        populateFacilities(floorManager.sites[idx].facilities)
        onSelectedChanged()
        internal.selectedFacilityId = ""
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
            internal.selectedSiteId = site.siteId
        }
    }

    function populateFacilities(listFacilities) {
        facilities.clear()
        let facilitiesExtracted = Array.from(listFacilities)
        facilitiesExtracted.sort(function (first, second) {
            if (first.site.name < second.site.name)
                return -1
            else if (first.site.name > second.site.name)
                return 1
            else {

                //first and second have same parent site, order them alphabetically
                if (first.name < second.name)
                    return -1
                else if (first.name > second.name)
                    return 1

            }
               return 0
        })
        facilitiesExtracted.forEach(facility => {
                                        facilities.append({
                                                              "name": facility.name,
                                                              "modelId": facility.facilityId,
                                                              "parentSiteName": facility.site.name
                                                          })
                                    })

        // case single facility: autoselect it
        if (listFacilities.length === 1) {
            internal.selectedFacilityId = listFacilities[0].facilityId
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
                internal.selectedLevelId = level.levelId
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
            internal.selectedLevelId = listLevels[0].levelId
    }

    function zoomToFacility(facilityId) {
        let idx = findElementIdxById(facilityId,
                                     FloorFilterController.TypeElement.Facility)
        let facility = floorManager.facilities[idx]
        const extent = facility.geometry.extent
        zoomToEnvelope(extent)
    }

    function zoomToSite(siteId) {
        let idx = findElementIdxById(siteId,
                                     FloorFilterController.TypeElement.Site)
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
        console.log("--")
        for (let i = 0; i < floorManager.levels.length; ++i) {
            let level = floorManager.levels[i]
            console.log(level.verticalOrder)
            level.visible = level.verticalOrder === selectedLevel.verticalOrder
        }
    }

    onFloorManagerChanged: console.log("manager changed")

    enum UpdateLevelsMode {
        SingleLevel,
        AllLevelsMatchingVerticalOrder
    }

    enum TypeElement {
        Level,
        Facility,
        Site
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
    }
}
