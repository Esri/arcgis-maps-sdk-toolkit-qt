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
    id: floorFilterControllerFuncTest
    name: "FloorFilterControllerFuncTest"

    Credential {
        id: viewerCredentialIndoors
        // need to add CREDENTIAL_USERNAME and CREDENTIAL_PASSWORD to the Environment variables.
        username: credential_username
        password: credential_password
    }

    MapView {
        id: viewMulti
        Map {
            id: mapMulti
            item: PortalItem {
                itemId: "49520a67773842f1858602735ef538b5"
                portal: Portal {
                    Component.onCompleted: load()
                    onErrorChanged: {
                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
                            console.log(error.message);
                        }
                    }
                    credential: viewerCredentialIndoors
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

    // 1 site, 1 facility, 3 levels
    Component {
        id: controlSingleComponent
        FloorFilterController {
            id: single
            geoView: viewSingle
            property SignalSpy spy : SignalSpy {
                target: single
                signalName: "onLoaded" // signal outputted from FloorFilter once the floorManager has been loaded
            }
        }
    }

    // more sites, more facilities
    Component {
        id: controlMultiComponent
        FloorFilterController {
            id: multi
            geoView: viewMulti
            property SignalSpy spy : SignalSpy {
                target: multi
                signalName: "onLoaded" // signal outputted from FloorFilter once the floorManager has been loaded
            }
        }
    }

    function loadFloorManagerSingle() {
        var control = createTemporaryObject(controlSingleComponent, floorFilterControllerFuncTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            control.spy.wait(10000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function loadFloorManagerMultiple() {
        var control = createTemporaryObject(controlMultiComponent, floorFilterControllerFuncTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            control.spy.wait(10000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function test_allSitesFacilitySelection() {
        var control = loadFloorManagerMultiple();
        // autofires populateAllFacilities()
        control.selectedSiteRespected = false;
        var facility = control.floorManager.facilities[0];
        compare(facility.facilityId, "1000.US01.WAREHOUSE.T");
        control.setSelectedFacilityId(facility.facilityId);
        compare(control.selectedSiteId, facility.site.siteId);
        compare(control.selectedSite.siteId, facility.site.siteId);
    }

    // singlesite mode on. only 1 site and 1 facility, automatically select them on load.
    function test_singleSiteAutoSelect() {
        var control = loadFloorManagerSingle();
        compare(control.floorManager.sites.length, 1);
        compare(control.sites.count, 1);
        compare(control.facilities.count, 1);
        verify(control.selectedSiteId !== "") // not empty string
        verify(control.selectedSite != null);
        verify(control.selectedFacilityId !== "");
        verify(control.selectedFacility != null);
    }

    Component {
        id: mapComponentMulti
        Map {
            id: map
            item: PortalItem {
                itemId: "49520a67773842f1858602735ef538b5"
                portal: Portal {
                    Component.onCompleted: load()
                    onErrorChanged: {
                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
                            console.log(error.message);
                        }
                    }
                    credential: viewerCredentialIndoors
                    url: "https://indoors.maps.arcgis.com/"
                }
            }
            onLoadStatusChanged: {
                if (loadStatus === Enums.LoadStatusLoaded) {
                    console.log("map loaded")
                    map.floorManager.load()
                }
            }
            property SignalSpy spy: SignalSpy {
                target: map.floorManager
                signalName: "onLoadStatusChanged"
            }
        }

    }

    // selecting facilities. check that the facilities are set and its default levels are automatically selected.
    function test_selectFacilities() {
        var control = loadFloorManagerMultiple();
        // select facility
        control.setSelectedFacilityId("1000.US01.MAIN.M");
        compare(control.selectedFacilityId, "1000.US01.MAIN.M");
        compare(control.selectedFacility.name, "RED M");
        compare(control.selectedSite.name, "Esri Redlands Main Campus");
        verify(control.selectedLevel != null);
        compare(control.selectedLevelId, "1000.US01.MAIN.M1")

        // select facility with no level. check that the selectedLevel is reset
        control.setSelectedFacilityId("1000.US01.MAIN.N");
        compare(control.selectedFacilityId, "1000.US01.MAIN.N");
        compare(control.selectedFacility.name, "RED N");
        compare(control.selectedSite.name, "Esri Redlands Main Campus");
        verify(control.selectedLevel == null);
        compare(control.selectedLevelId, "");
    }

    // don't change the visibility of levels when ff is loaded.
    // load a new map with floormanager. check its levels visibility with the same map loaded in the floorfilter
    function test_noModifyOriginalVisibilityLevels() {
        var control = loadFloorManagerMultiple();
        var map = createTemporaryObject(mapComponentMulti, floorFilterControllerFuncTest);
        map.load();
        // wait for map to be loaded, then able to set a spy on the floormanager.
        map.spy.wait();
        if(map.floorManager.loadStatus !== Enums.LoadStatusLoaded)
            map.spy.wait(); // the first wait outputter a Loading status.
        // floormanager automatically loaded from the map component
        control.spy.wait();
        verify(map.floorManager.loadStatus === Enums.LoadStatusLoaded);
        verify(map.floorManager.levels.length > 0);
        verify(map.floorManager.levels.length === control.floorManager.levels.length);
        for (var i = 0; i < map.floorManager.levels; ++i) {
            verify(map.floorManager.levels[i].visible === control.floorManager.levels[i].visible);
        }
    }
}
