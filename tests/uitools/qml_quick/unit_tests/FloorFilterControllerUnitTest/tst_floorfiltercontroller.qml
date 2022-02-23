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

import QtQuick 2.0
import QtTest 1.0

import Esri.ArcGISRuntime 100.14
import Esri.ArcGISRuntime.Toolkit 100.14
import Esri.ArcGISRuntime.Toolkit.Controller 100.14

TestCase {
    name: "FloorFilterControllerUnitTest"

    MapView {
        id: view
        Map {
            id: map
            item: PortalItem {
                itemId: "f133a698536f44c8884ad81f80b6cfc7"
                portal: Portal {
                    Component.onCompleted: load()
                    onErrorChanged: {
                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
                            console.log(error.message);
                        }
                    }
                    url: "https://www.arcgis.com"
                }
            }
        }
    }

    FloorFilterController {
        id: control
        geoView: view
    }
    SignalSpy {
        id: ffSpy
        target: control.floorManager
        signalName: "onLoadStatusChanged"
    }

    SignalSpy {
        id: ffLevelIdSpy
        target: control
        signalName: "onSelectedLevelIdChanged"
    }

    SignalSpy {
        id: viewpointSpy
        target: control.geoView
        signalName: "setViewpointCompleted"
    }

    property alias floorManager: control.floorManager
    function initTestCase() {
        ffSpy.wait();
        // first signal is loading, wait for loaded signal
        if (control.floorManager.loadStatus !== Enums.LoadStatusLoaded)
            ffSpy.wait(10000);
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
    }

    function test_findElementIdxById() {
        // find site
        var idx = control.findElementIdxById(floorManager.sites[0].siteId, FloorFilterController.TypeElement.Site);
        compare(0, idx);
        // not existing site id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Site);
        compare(null, idx);
        // find facility
        idx = control.findElementIdxById(floorManager.facilities[0].facilityId, FloorFilterController.TypeElement.Facility);
        compare(0, idx);
        // not existing facility id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Facility);
        compare(null, idx);
        // find level
        idx = control.findElementIdxById(floorManager.levels[0].levelId, FloorFilterController.TypeElement.Level);
        compare(0, idx);
        // not existing level id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Level);
        compare(null, idx);
    }

    function test_setSelectedSiteId() {
        control.setSelectedSiteId("Esri Redlands Main");
        compare(control.selectedSiteId, "Esri Redlands Main");
    }

    function test_setSelectedFacilityId() {
        var facility = floorManager.facilities[0];
        control.setSelectedFacilityId(facility.facilityId);
        verify(facility.facilityId !== "");
        compare(control.selectedFacilityId, facility.facilityId);
        // not performing object equality, just the facilityId
        compare(control.selectedFacility.facilityId, facility.facilityId);
    }

    // check level id is set, selected level object is set
    function test_setSelectedLevelId() {
        ffLevelIdSpy.clear()
        var level = floorManager.levels[1];
        control.setSelectedLevelId(level.levelId);
        verify(level.levelId !== "");
        compare(control.selectedLevelId, level.levelId);
        compare(control.selectedLevel.levelId, level.levelId);
        console.log("count", ffLevelIdSpy.count);
        ffLevelIdSpy.wait();
        compare(floorManager.levels[1].visible, true);
        // set empty level, clear the level selection
        control.setSelectedLevelId("");
        compare(control.selectedLevelId, "");
        compare(control.selectedLevel, null);
        compare(floorManager.levels[0].visible, false);
    }

    // check levels selection modes are respected
    function test_updateLevelsModeSelection() {
        // all levels matching vertical ordering mode.
        // default AllLevelsMatchingVerticalOrder
        compare(control.updateLevelsMode, FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder);
        var level = floorManager.levels[0];
        verify(level && level.levelId);
        control.setSelectedLevelId(level.levelId);

        var verticalOrder = level.verticalOrder
        verify(floorManager.levels.length > 1);
        // all the levels with matching vertical order are visible
        for(var i = 0; i < floorManager.levels.length; ++i){
            let levelI = floorManager.levels[i];
            if(levelI.verticalOrder === verticalOrder)
                verify(levelI.visible);
        }

        // updatelevelsmode SingleLevel
        control.updateLevelsMode = FloorFilterController.SingleLevel;
        // set all level's visiblities as false to build test.
        for(var i = 0; i < floorManager.levels.length; ++i){
            floorManager.levels[i].visibility = false;
        }
        // setting a new level as selected (last one)
        level = floorManager.levels[floorManager.levels.length - 1];
        control.setSelectedLevelId(level.levelId);

        for(var i = 0; i < floorManager.levels.length - 1; ++i){
            verify(!floorManager.levels.visible);
        }
    }

    function test_setVisibilityCurrentLevel() {
        // set true visibile
        var floorLevel = floorManager.levels[0];
        control.internal.selectedLevel = floorLevel;
        control.setVisibilityCurrentLevel(true);
        compare(true, control.selectedLevel.visible);
        // set false visibile
        control.setVisibilityCurrentLevel(false);
        compare(false, control.selectedLevel.visible);
        // set visibile to null level
        control.internal.selectedLevel = null;
        control.setVisibilityCurrentLevel(true);
        compare(null, control.selectedLevel);
    }

    function test_populateSites() {
        control.populateSites(floorManager.sites);
        compare(control.sites.count, floorManager.sites.length);
        // check site model loaded correctly
        var siteElem = control.sites.get(0);
        compare(siteElem.name, floorManager.sites[0].name);
        compare(siteElem.modelId, floorManager.sites[0].siteId);
        // todo: autoselect switch? is it unit or functional?
    }

    function test_populateFacilities() {
        control.populateFacilities(floorManager.facilities);
        compare(control.facilities.count, floorManager.sites.length);
        // check facility model loaded correctly
        var facilityElem = control.facilities.get(0)
        compare(facilityElem.name, floorManager.facilities[0].name);
        compare(facilityElem.modelId, floorManager.facilities[0].facilityId);
        // todo: autoselect switch? is it unit or functional?
    }

    function test_populateAllFacilities() {
        control.populateAllFacilities();
        compare(control.facilities.count, floorManager.facilities.length)
        // check facility model loaded correctly
        var facilityElem = control.facilities.get(0)
        compare(facilityElem.name, floorManager.facilities[0].name);
        compare(facilityElem.modelId, floorManager.facilities[0].facilityId);
    }

    function test_populateLevels() {
        control.populateLevels(floorManager.levels);
        compare(control.levels.count, floorManager.levels.length);
        // check level model loaded correctly (bottom to top levels)
        var levelElem = control.levels.get(control.levels.count - 1);
        compare(levelElem.shortName, floorManager.levels[0].shortName);
        compare(levelElem.longName, floorManager.levels[0].longName);
        compare(levelElem.modelId, floorManager.levels[0].levelId);
    }

    // skipping functions that are based on viewpoints (upateselection and the zoom ones)



















}// sort functionality
