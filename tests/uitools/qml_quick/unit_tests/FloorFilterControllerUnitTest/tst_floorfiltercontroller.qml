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
        if (control.floorManager.loadStatus !== Enums.LoadStatusLoaded);
        ffSpy.wait(10000);
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
    }

    function cleanupTestCase() {}

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
        print(facility.facilityId);
        verify(facility.facilityId !== "");
        compare(control.selectedFacilityId, facility.facilityId);
        // not performing object equality, just the facilityId
        compare(control.selectedFacility.facilityId, facility.facilityId);
    }

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

    function test_setVisibilityCurrentLevel() {
        var floorLevel = floorManager.levels[0];
        control.internal.selectedLevel = floorLevel;
        control.setVisibilityCurrentLevel(true);
        compare(true, control.selectedLevel.visible);
        control.setVisibilityCurrentLevel(false);
        compare(false, control.selectedLevel.visible);

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
