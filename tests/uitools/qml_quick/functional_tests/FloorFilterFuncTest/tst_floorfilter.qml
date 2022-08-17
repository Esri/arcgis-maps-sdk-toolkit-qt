
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
import QtQuick.Controls 2.15
import QtTest 1.0

import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit 100.15
import Esri.ArcGISRuntime.Toolkit.Controller 100.15
import utils 1.0

Rectangle {
    id: root
    width: 640; height: 480

    MapView {
        id: viewMulti
        anchors.fill: parent
        property var initialViewpoint
        onCurrentViewpointCenterChanged: initialViewpoint = viewMulti.currentViewpointCenter
        Map {
            id: mapMulti
            item: PortalItem {
                itemId: "b4b599a43a474d33946cf0df526426f5"
                portal: Portal {
                    Component.onCompleted: load()
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

    Component {
        id: ffComponent
        FloorFilter {
        }
    }

    Component {
        id: ffMultiComponent
        FloorFilter {
            geoView: viewMulti
            controller: FloorFilterController {
                id: controllerMulti
                property SignalSpy spy : SignalSpy {
                    target: controllerMulti
                    signalName: "loaded"
                }
            }
        }
    }

    TestCase {
        id: floorFilterFuncTest
        name: "FloorFilterFuncTest"
        when: windowShown

        function test_collapserDefault() {
            let ff = createTemporaryObject(ffComponent, root);
            let collapser = findChild(ff, "collapser");
            let zoom = findChild(ff, "zoom");
            compare(collapser.text, qsTr("Collapse"));
            compare(collapser.display, AbstractButton.IconOnly);
            compare(zoom.text, qsTr("Zoom to"));
            compare(zoom.display, AbstractButton.IconOnly)
            // wait to give a bit of time on what is happening in the UI
            wait(1000);
            mouseClick(collapser);
            compare(collapser.display, AbstractButton.TextBesideIcon);
            compare(zoom.display, AbstractButton.TextBesideIcon);
            wait(1000);
        }

        // test ff components that change state based on ff realative position to its parent.
        // x axis changes the gridOuterWrapper layoutDirection switching toolbuttons position with the site/facility component. Collapser icon used (point left or right).
        function test_visual_changePosition() {
            let ff = createTemporaryObject(ffComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            let gridOuterWrapper = findChild(ff, "gridOuterWrapper");
            let collapser = findChild(ff, "collapser");
            mouseClick(buildingMenuButton);
            // set to top left
            ff.x = 0;
            ff.y = 0;

            // move ff until its center point is < parent center
            tryVerify(function() {
                if(ff.x + ff.width / 2 >= ff.parent.width / 2)
                    return true;
                compare(Qt.LeftToRight,  gridOuterWrapper.layoutDirection)
                compare("chevrons-right.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(50);
                return false;
            });

            // move ff until x-axis-end of parent
            tryVerify(function() {
                if(ff.x >= ff.parent.width)
                    return true;
                compare(Qt.RightToLeft,  gridOuterWrapper.layoutDirection)
                compare("chevrons-left.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(50);
                return false;
            });

            // reset the ff to top left
            ff.x = 0;
            ff.y = 0;
            // same as what done above, but for y-axis
            tryVerify(function() {
                if(ff.y + ff.height / 2 >= ff.parent.height / 2)
                    return true;
                compare(Grid.AlignTop, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20;
                wait(50);
                return false;
            });

            tryVerify(function() {
                if(ff.y >= ff.parent.height)
                    return true;
                compare(Grid.AlignBottom, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20;
                wait(50);
                return false;
            });
        }

        // click floorfilter button and open the site/facility listview. single and multi sites/facilities
        function test_floorfilterOpen() {
            //no sites, no facilities
            let ff = createTemporaryObject(ffComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            let internal = findChild(ff, "internal");
            let listView = findChild(ff, "listView");
            verify(internal)
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Facility);
            compare(false, listView.visible);
            mouseClick(buildingMenuButton);
            compare(true, buildingMenuButton.checked);
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Facility);
            verify(listView);
            compare(true, listView.visible);
            let backToSite = findChild(ff, "backToSite");
            // visible but with width size of 0
            compare(true, backToSite.visible);
            compare(0, backToSite.width);

            //multi sites
            let ffMulti = createTemporaryObject(ffMultiComponent, root);
            if(ffMulti.controller.floorManager != null ? ffMulti.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ffMulti.controller.spy.wait(10000);
            buildingMenuButton = findChild(ffMulti, "buildingMenuButton");
            internal = findChild(ffMulti, "internal");
            listView = findChild(ffMulti, "listView");
            verify(internal)
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Site);
            compare(false, listView.visible);
            mouseClick(buildingMenuButton);
            compare(true, buildingMenuButton.checked);
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Site);
            verify(listView);
            compare(true, listView.visible);
            backToSite = findChild(ffMulti, "backToSite");
            // visible but with width size of 0
            compare(true, backToSite.visible);
            compare(0, backToSite.width);
        }

        function test_listViewCloser() {
            let ff = createTemporaryObject(ffComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            compare(true, listView.visible);
            compare(true, buildingMenuButton.checked);
            let closerListView = findChild(ff, "closerListView");
            verify(closerListView);
            mouseClick(closerListView);
            compare(false, listView.visible);
            compare(false, buildingMenuButton.checked);
        }

        function test_filter() {
            let ff = createTemporaryObject(ffMultiComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            verify(ff.controller.floorManager.loadStatus === Enums.LoadStatusLoaded);
            verify(ff.controller.floorManager.levels.length > 0)
            verify(listView.model.count > 0);
            let searchTextField = findChild(ff, "searchTextField");
            verify(searchTextField)
            searchTextField.text = "west";
            compare(1, listView.model.count);
            let noResultsFoundLabel = findChild(ff, "noResultsFoundLabel");
            compare(false, noResultsFoundLabel.visible);
            searchTextField.text = "azz";
            compare(0, listView.model.count);
            compare(true, noResultsFoundLabel.visible);
            compare("No results found", noResultsFoundLabel.text);
            // select the searchtextfield
            mouseClick(searchTextField);
            // delete 2 letters from the search text
            keyClick(Qt.Key_Backspace);
            verify(listView.model.count < 1);
            keyClick(Qt.Key_Backspace);
            compare("a", searchTextField.text);
            verify(listView.model.count > 1);
        }

        function test_highlightElements() {
            let ff = createTemporaryObject(ffMultiComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            verify(listView.model.count > 0);
            let siteLabel = findChild(ff, "siteLabel");
            compare("Select a Site", siteLabel.text);
            // click the first element in the list view at central x position and 5 y axis
            let internal = findChild(ff, "internal");
            // select site
            mouseClick(listView, 25, 25);
            // no currentItem is selected
            verify(listView.currentItem == null);
            compare(-1, listView.currentIndex);
            compare("East", siteLabel.text);
            let backToSite = findChild(ff, "backToSite");
            // go back to site view
            mouseClick(backToSite);
            compare(FloorFilter.VisibleListView.Site, internal.currentVisibileListView);
            // reselect the site and show the facilities
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);

            compare(FloorFilter.VisibleListView.Facility, internal.currentVisibileListView);
            // select first facility
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(false, listView.visible);
            // open listview to check the selected
            mouseClick(buildingMenuButton);
            compare(true, listView.visible);
            // check selected facility is the first one
            //compare(true, listView.currentItem.highlighted);
            compare(0, listView.currentIndex);
        }

        // check all sites functionality, flickable/repeater and select levels.
        function test_allSites_levels() {
            // todo: checks on delegate text (composed by facility + new line + parent site)
            let ff = createTemporaryObject(ffMultiComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            let showAllFacilities = findChild(ff, "showAllFacilities");
            // show all facilities
            mouseClick(showAllFacilities, showAllFacilities.width / 2, showAllFacilities.height / 2, Qt.LeftButton, Qt.NoModifier, 100);
            verify(listView.model.count > 0);
            let internal = findChild(ff, "internal");
            compare(FloorFilter.VisibleListView.Facility, internal.currentVisibileListView);
            // search for 3 level facility
            let searchTextField = findChild(ff, "searchTextField");
            searchTextField.text = "ellipsoid";
            // select first facility
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare("GFC.WEST.E", internal.selectedFacilityId);
            let siteLabel = findChild(ff, "siteLabel");
            let facilityLabel = findChild(ff, "facilityLabel");
            compare("West", siteLabel.text);
            compare("Ellipsoid", facilityLabel.text);
            // check repeater
            let repeater = findChild(ff, "repeater");
            compare(5, repeater.count);
            let flickable = findChild(ff, "flickable");
            // check the height of the flickable is number of items * buttonheight
            // wait that the delegate sets the buttonHeight once it is completed.
            tryCompare(flickable, "height", 2 * repeater.buttonHeight, 50);
            // check the flickable scrollable height is 3 * buttonHeight
            compare(200, flickable.contentHeight);

            // select first level and check visiblity, text on gui, text on controller and checked status
            let wrapper = findChild(ff, "wrapper");
            // scroll up to have the highest level on top.
            mouseWheel(flickable, 10, 10, 0, 120, Qt.NoButton);
            wait(1000); // wait that the scroll is done.
            mouseClick(flickable, 10, 10);
            compare("GFC.WEST.E4", ff.controller.selectedLevelId);
            compare("4", wrapper.children[0].text);
            verify(wrapper.children[0].checked);
            verify(ff.controller.selectedLevel.visible === true);
            // select second level
            mouseClick(flickable, 10, 50);
            compare("GFC.WEST.E3", ff.controller.selectedLevelId);
            compare("3", wrapper.children[1].text);
            verify(wrapper.children[1].checked);
            verify(ff.controller.selectedLevel.visible === true);
            // select third level. have to scroll down (negative means scroll down, positive means scroll up). 120 is 15 degrees of scoll (enough for our needs).
            mouseWheel(flickable, 10, 10, 0, -120, Qt.NoButton);
            wait(1000);
            // select the third level, which now is at second position
            mouseClick(flickable, 10, 50);
            wait(1000)
            compare("GFC.WEST.EB", ff.controller.selectedLevelId);
            compare("B", wrapper.children[4].text);
            verify(wrapper.children[4].checked);
            // check the other levels are not checked anymore
            verify(!wrapper.children[0].checked);
            verify(!wrapper.children[1].checked);
            verify(!wrapper.children[2].checked);
            verify(!wrapper.children[3].checked);
            verify(ff.controller.selectedLevel.visible === true);
        }

        // zoom button enabled or not. Check if it changes viewpoint by spying on setViewpointCompleted
        function test_zoom() {
            let ff = createTemporaryObject(ffMultiComponent, root);
            ff.controller.automaticSelectionMode = FloorFilterController.Never;
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            let zoom = findChild(ff, "zoom");
            compare(false, zoom.enabled);
            // select a site (which has > 1 facility, otherwise the facility is automatically selected)
            listView.positionViewAtIndex(1, ListView.Beginning); // scroll to index 1 site, then click on it
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(false, zoom.enabled);
            // back to siteview
            let backToSite = findChild(ff, "backToSite");
            mouseClick(backToSite);
            compare(true, zoom.enabled);
            // pan mapview
            mouseDrag(viewMulti, viewMulti.width / 2, viewMulti.height / 2, 30, 0);
            // spy on the setViewpoint
            let spyViewpoint = createTemporaryQmlObject('import QtQuick 2.0; SignalSpy { target: viewMulti; signalName : "setViewpointCompleted" }', root);
            mouseClick(zoom);
            spyViewpoint.wait();
            // select site again, go to facility view
            listView.positionViewAtIndex(1, ListView.Beginning);
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(false, zoom.enabled);
            mouseDrag(viewMulti, viewMulti.width / 2, viewMulti.height / 2, 30, 0);
            // select a facility
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            spyViewpoint.clear();
            mouseClick(zoom);
            spyViewpoint.wait();
        }

        function test_close() {
            let ff = createTemporaryObject(ffMultiComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            let showAllFacilities = findChild(ff, "showAllFacilities");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            mouseClick(showAllFacilities);
            // select first all facility (Azimuth)
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            let closer = findChild(ff, "closer");
            verify(closer);
            let itemSelectedButton = findChild(ff, "itemSelectedButton");
            compare(false, itemSelectedButton.visible);
            compare(true, closer.visible);
            // close the floorfilter: collapses the levels and only show the current selected
            // have to use 10ms delay, otherwise the click is done befor that the closer is available
            mouseClick(closer, closer.width / 2, closer.height / 2, Qt.LeftButton, Qt.NoModifier, 10);
            let flickable = findChild(ff, "flickable");
            compare(false, closer.visible);
            compare(false, flickable.visible);
            compare(true, itemSelectedButton.visible);
            compare("1", itemSelectedButton.text);
            // check the not collapsed itemSelectedButton
            let collapser = findChild(ff, "collapser");
            // reopen the floorfilter, showing all the levels available
            mouseClick(itemSelectedButton);
            compare(true, flickable.visible);
            compare(true, closer.visible);
            compare(false, itemSelectedButton.visible);
        }

        // testing the viewpoint mode. This functionality is more related to the \c FloorFilterController, but
        // it is needed a rendering test in order to complete the `setViewpoint` and the GUI testing suite provides it.
        function test_viewpointMode() {
            let ff = createTemporaryObject(ffMultiComponent, root);
            let buildingMenuButton = findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            let listView = findChild(ff, "listView");
            let showAllFacilities = findChild(ff, "showAllFacilities");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                ff.controller.spy.wait(10000);
            let spyViewpoint = createTemporaryQmlObject('import QtQuick 2.0; SignalSpy { signalName : "setViewpointCompleted" }', root);
            spyViewpoint.target = ff.geoView;
            // reset by zooming out
            let builder = ArcGISRuntimeEnvironment.createObject('EnvelopeBuilder', {
                                                                    "geometry": ff.geoView.currentViewpointExtent.extent
                                                                });
            builder.expandByFactor(10.0);
            let outViewpoint = ArcGISRuntimeEnvironment.createObject(
                        'ViewpointExtent', {
                            "extent": builder.geometry
                        });
            ff.geoView.setViewpoint(outViewpoint);
            spyViewpoint.wait();
            compare(ff.controller.automaticSelectionMode, FloorFilterController.AutomaticSelectionMode.Always);
            //ff.controller.setSelectedSiteId(ff.controller.floorManager.sites[1].siteId);

            let selectFacility = ff.controller.floorManager.facilities[0];
            verify(selectFacility != null);
            let newViewpointFacility = ArcGISRuntimeEnvironment.createObject(
                        'ViewpointExtent', {
                            "extent": selectFacility.geometry
                        });
            viewMulti.setViewpoint(newViewpointFacility);
            // need to spy on the `selectedSite` because when the spyViewpoint is done, the `selectedSite` is not yet set.
            spyViewpoint.clear();
            spyViewpoint.wait();
            let internal = findChild(ff, "internal");
            // have to tryVerify until the `selectedSiteId` is not null/"" becuase it is set multiple times from the `tryUpdate`, so can't rely on a signalSpy on its onChanged event.
            tryVerify(function() {return internal.selectedSiteId != null && internal.selectedSiteId.length > 0});
            tryVerify(function() {return internal.selectedFacilityId === selectFacility.facilityId}, 5000, `internal.selectedFacilityId ${internal.selectedFacilityId}, selectFacility.facilityId ${selectFacility.facilityId}`);
            compare(internal.selectedSiteId, ff.controller.selectedSiteId);
            compare(ff.controller.selectedSiteId, selectFacility.site.siteId);
            // directly access controller properties
            verify(ff.controller.selectedSite != null);
            compare(internal.selectedFacilityId, ff.controller.selectedFacilityId);

            // reset the geoview viewpoint
            spyViewpoint.clear();
            ff.geoView.setViewpoint(outViewpoint);
            spyViewpoint.wait();
            // have to tryVerify until the `selectedSiteId` is null/"" becuase it is set multiple times from the `tryUpdate`, so can't rely on a signalSpy on its onChanged event.
            tryVerify(function() {return internal.selectedSiteId == null || internal.selectedSiteId.length === 0}, 5000, `internal.selectedSiteId: ${internal.selectedSiteId}`);
            compare(internal.selectedSiteId, ff.controller.selectedSiteId);
            compare(ff.controller.selectedSiteId, "");
            // directly access controller properties
            verify(ff.controller.selectedSite == null);
            compare(internal.selectedFacilityId, "");
            compare(internal.selectedFacilityId, ff.controller.selectedFacilityId);
        }
    }
}
