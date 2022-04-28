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
    id: floorFilterControllerUnitTest
    name: "FloorFilterControllerUnitTest"

    MapView {
        id: viewMulti
        Map {
            id: mapMulti
            item: PortalItem {
                itemId: "b4b599a43a474d33946cf0df526426f5"
                portal: Portal {
                    onErrorChanged: {
                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
                            console.log(error.message);
                        }
                    }
                    url: "https://indoors.maps.arcgis.com/"
                }
            }
        }
    }

    MapView {
        id: viewSingle
        Map {
            id: mapSingle
            item: PortalItem {
                itemId: "f133a698536f44c8884ad81f80b6cfc7"
                portal: Portal {
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

    // 1 site, 1 facility, 3 levels
    Component {
        id: controlSingleComponent
        FloorFilterController {
            geoView: viewSingle
        }
    }

    // more sites, more facilities
    Component {
        id: controlMultiComponent
        FloorFilterController {
            geoView: viewMulti
        }
    }

    SignalSpy {
        id: ffLoadedSpy
        signalName: "onLoaded" // signal outputted from FloorFilter once the control.floorManager has been loaded
    }

    function loadFloorManagerSingle() {
        ffLoadedSpy.clear();
        var control = createTemporaryObject(controlSingleComponent, floorFilterControllerUnitTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            ffLoadedSpy.target = control;
            ffLoadedSpy.wait(10000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function loadFloorManagerMultiple() {
        ffLoadedSpy.clear();
        var control = createTemporaryObject(controlMultiComponent, floorFilterControllerUnitTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            ffLoadedSpy.target = control;
            ffLoadedSpy.wait(12000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function test_findElementIdxById() {
        var control = loadFloorManagerMultiple();
        // find site
        var idx = control.findElementIdxById(control.floorManager.sites[0].siteId, FloorFilterController.TypeElement.Site);
        compare(idx, 0);
        // not existing site id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Site);
        compare(idx, null);
        // find facility
        idx = control.findElementIdxById(control.floorManager.facilities[0].facilityId, FloorFilterController.TypeElement.Facility);
        compare(idx, 0);
        // not existing facility id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Facility);
        compare(idx, null);
        // find level
        idx = control.findElementIdxById(control.floorManager.levels[0].levelId, FloorFilterController.TypeElement.Level);
        compare(idx, 0);
        // not existing level id
        idx = control.findElementIdxById("", FloorFilterController.TypeElement.Level);
        compare(idx, null);
    }

    function test_setSelectedSiteId() {
        var control = loadFloorManagerMultiple();
        // set site that is not in the list
        control.setSelectedSiteId("azzzzz");
        compare(control.selectedSiteId, "");
        control.setSelectedSiteId("GFC.RA");
        compare(control.selectedSiteId, "GFC.RA");
    }

    function test_setSelectedFacilityId() {
        var control = loadFloorManagerMultiple();
        var facility = control.floorManager.facilities[0];
        control.setSelectedFacilityId(facility.facilityId);
        verify(facility.facilityId !== "");
        compare(control.selectedFacilityId, facility.facilityId);
        // not performing object equality, just the facilityId
        compare(control.selectedFacility.facilityId, facility.facilityId);
    }

    // check level id is set, selected level object is set
    function test_setSelectedLevelId() {
        var control = loadFloorManagerMultiple();
        var ffLevelIdSpy = createTemporaryQmlObject(
                    'import QtQuick 2.0; SignalSpy {signalName: "onSelectedLevelIdChanged"}', floorFilterControllerUnitTest);
        ffLevelIdSpy.target = control
        var level = control.floorManager.levels[1];
        control.setSelectedLevelId(level.levelId);
        verify(level.levelId !== "");
        compare(control.selectedLevelId, level.levelId);
        compare(control.selectedLevel.levelId, level.levelId);
        console.log("count", ffLevelIdSpy.count);
        ffLevelIdSpy.wait();
        compare(control.floorManager.levels[1].visible, true);
        // set empty level, clear the level selection
        control.setSelectedLevelId("");
        compare(control.selectedLevelId, "");
        compare(control.selectedLevel, null);
        compare(level.visible, false);
    }

    // check levels selection modes are respected
    function test_updateLevelsModeSelection() {
        var control = loadFloorManagerMultiple();
        // all levels matching vertical ordering mode.
        // default AllLevelsMatchingVerticalOrder
        compare(control.updateLevelsMode, FloorFilterController.UpdateLevelsMode.AllLevelsMatchingVerticalOrder);
        var level = control.floorManager.levels[0];
        verify(level && level.levelId);
        control.setSelectedLevelId(level.levelId);

        var verticalOrder = level.verticalOrder
        verify(control.floorManager.levels.length > 1);
        // all the levels with matching vertical order are visible
        for(var i = 0; i < control.floorManager.levels.length; ++i){
            let levelI = control.floorManager.levels[i];
            if(levelI.verticalOrder === verticalOrder)
                verify(levelI.visible);
        }
    }

    function test_setVisibilityCurrentLevel() {
        var control = loadFloorManagerMultiple();
        // set true visibile
        var floorLevel = control.floorManager.levels[0];
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
        var control = loadFloorManagerMultiple();
        control.populateSites(control.floorManager.sites);
        compare(control.sites.count, control.floorManager.sites.length);
        // check site model loaded correctly
        var siteElem = control.sites.get(0);
        compare(siteElem.name, control.floorManager.sites[0].name);
        compare(siteElem.modelId, control.floorManager.sites[0].siteId);
        // todo: autoselect switch? is it unit or functional?
    }

    function test_populateFacilities() {
        var control = loadFloorManagerSingle();
        control.populateFacilities(control.floorManager.facilities);
        compare(control.facilities.count, control.floorManager.facilities.length);
        // check facility model loaded correctly
        var facilityElem = control.facilities.get(0);
        compare(facilityElem.name, control.floorManager.facilities[0].name);
        compare(facilityElem.modelId, control.floorManager.facilities[0].facilityId);
        // todo: autoselect switch? is it unit or functional?
    }

    function test_populateAllFacilities() {
        var control = loadFloorManagerMultiple();
        control.populateAllFacilities();
        compare(control.facilities.count, control.floorManager.facilities.length);
        // check facility model loaded correctly. default ordering from the floormanager
        const facilityNames = ["Datum","Bearing","Azimuth","Contour","Lattice","Ellipsoid","Meridian","Geoid"];

        verify(facilityNames.length === control.facilities.count);
        for(var i = 0; i < control.facilities.count; ++i){
            compare(control.facilities.get(i).name, facilityNames[i]);
        }
    }

    // check levels populated are ordered ascending by their vertical order
    function test_populateLevels() {
        var control = loadFloorManagerMultiple();
        control.populateLevels(control.floorManager.facilities[5].levels);
        compare(control.levels.count, control.floorManager.facilities[5].levels.length);
        // check level model loaded correctly (bottom to top levels)
        var facility = control.floorManager.facilities[5]; // only 1 facility
        compare(control.levels.get(0).shortName, "4");
        compare(facility.levels[4].shortName, control.levels.get(0).shortName);
        var prevVerticalOrder = facility.levels[4].verticalOrder;

        compare(control.levels.get(1).shortName, "3");
        compare(facility.levels[3].shortName, control.levels.get(1).shortName);
        verify(prevVerticalOrder > facility.levels[3].verticalOrder);
        prevVerticalOrder = facility.levels[3].verticalOrder;

        compare(control.levels.get(2).shortName, "2");
        compare(facility.levels[2].shortName, control.levels.get(2).shortName);
        verify(prevVerticalOrder > facility.levels[2].verticalOrder);
        prevVerticalOrder = facility.levels[2].verticalOrder;

        compare(control.levels.get(3).shortName, "1");
        compare(facility.levels[1].shortName, control.levels.get(3).shortName);
        verify(prevVerticalOrder > facility.levels[1].verticalOrder);

        compare(control.levels.get(4).shortName, "B");
        compare(facility.levels[0].shortName, control.levels.get(4).shortName);
        verify(prevVerticalOrder > facility.levels[0].verticalOrder);
    }

    // skipping functions that are based on viewpoints (upateselection and the zoom ones)
}
