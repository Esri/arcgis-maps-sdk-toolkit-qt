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
    name: "FloorFilterControllerUnitTest"

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
        signalName: "onLoaded" // signal outputted from FloorFilter once the floorManager has been loaded
    }

    function loadFloorManagerSingle() {
        ffLoadedSpy.clear();
        var control = createTemporaryObject(controlSingleComponent, floorFilterControllerFuncTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            ffLoadedSpy.target = control;
            ffLoadedSpy.wait(10000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function loadFloorManagerMultiple() {
        ffLoadedSpy.clear();
        var control = createTemporaryObject(controlMultiComponent, floorFilterControllerFuncTest);

        if(!control.floorManager || control.floorManager.loadStatus !== Enums.LoadStatusLoaded) {
            ffLoadedSpy.target = control;
            ffLoadedSpy.wait(10000);
        }
        compare(control.floorManager.loadStatus, Enums.LoadStatusLoaded);
        return control;
    }

    function test_allSitesFacilitySelection() {
        var control = loadFloorManagerMultiple();
        // autofires populateAllFacilities()
        control.selectedSiteRespected = false;
        var facility = control.floorManager.facilities[0];
        verify(facility.facilityId);
        control.setSelectedFacilityId(facility.facilityId);
        compare(control.selectedSiteId, facility.site.siteId);
        compare(control.selectedSite.siteId, facility.site.siteId);
    }

    // singlesite mode on. only 1 site and 1 facility, automatically select them on load.
    function test_singleSiteAutoSelect() {
        var control = loadFloorManagerSingle();
        verify(1, control.floorManager.sites.count);
        compare(1, control.sites.count);
        compare(1, control.facilities.count);
        verify(control.selectedSiteId) // not empty string
        verify(control.selectedSite);
        verify(control.selectedFacilityId);
        verify(control.selectedFacility);
    }


    Component {
        id: mapComponentMulti
        Map {
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
            onLoadStatusChanged: console.log("loading map", loadStatus)
        }

    }

    // don't change the visibility of levels when ff is loaded.
    // load a new map with floormanager. check its levels visibility with the same map loaded in the floorfilter
    function test_noModifyOriginalVisibilityLevels() {
        var control = loadFloorManagerMultiple();
        var map = createTemporaryObject(mapComponentMulti, floorFilterControllerFuncTest);
        var control = createTemporaryObject(controlMultiComponent, floorFilterControllerFuncTest);
        map.load();
        var spy = createTemporaryQmlObject('import QtQuick 2.0; SignalSpy {signalName : "onLoadStatusChanged"}',
                                           floorFilterControllerFuncTest);
        spy.target = map;
        // wait for map to be loaded, then able to set a spy on the floormanager.
        spy.wait();
        spy.clear();
        spy.target = Qt.binding(function() { return map.floorManager});
        map.floorManager.load();
        spy.wait();
        console.log(map.floorManager.loadStatus);
        if(map.floorManager.loadStatus !== Enums.LoadStatusLoaded)
            spy.wait();
        verify(map.floorManager.levels.length > 0);
        verify(map.floorManager.levels.length === control.floorManager.levels.length);
        for (var i = 0; i < map.floorManager.levels; ++i) {
            verify(map.floorManager.levels[i].visible === control.floorManager.levels[i].visible);
        }
    }

}
