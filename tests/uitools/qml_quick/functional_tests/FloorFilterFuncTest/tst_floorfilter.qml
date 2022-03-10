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

import Esri.ArcGISRuntime 100.14
import Esri.ArcGISRuntime.Toolkit 100.14
import Esri.ArcGISRuntime.Toolkit.Controller 100.14
import utils 1.0

Rectangle {
    id: foo
    width: 640; height: 480

    MapView {
        id: viewMulti
        anchors.fill: parent
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

    Component {
        id: ffComponent
        FloorFilter {

        }
    }

    Component {
        id: ffMultiComponent
        FloorFilter {
            geoView: viewMulti
        }
    }

    TestCase {
        id: floorFilterFuncTest
        name: "FloorFilterFuncTest"
        when: windowShown
        Credential {
            id: viewerCredentialIndoors
            // need to add CREDENTIAL_USERNAME and CREDENTIAL_PASSWORD to the Environment variables.
            username: credential_username
            password: credential_password
        }

        function test_collapserDefault() {
            var ff = createTemporaryObject(ffComponent, foo);
            var collapser = Utils.findChild(ff, "collapser");
            var zoom = Utils.findChild(ff, "zoom");
            compare(collapser.text, "Collapse");
            compare(collapser.display, AbstractButton.IconOnly);
            compare(zoom.text, "Zoom to");
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
            var ff = createTemporaryObject(ffComponent, foo);
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            var gridOuterWrapper = Utils.findChild(ff, "gridOuterWrapper");
            var collapser = Utils.findChild(ff, "collapser");
            mouseClick(buildingMenuButton);
            // set to top left
            ff.x = 0;
            ff.y = 0;

            // move ff until its center point is < parent center
            while(ff.x + ff.width / 2 < ff.parent.width / 2){
                compare(Qt.LeftToRight,  gridOuterWrapper.layoutDirection)
                compare("chevrons-right.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(50);
            }
            // move ff until x-axis-end of parent
            while(ff.x < ff.parent.width){
                compare(Qt.RightToLeft,  gridOuterWrapper.layoutDirection)
                compare("chevrons-left.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(50);
            }

            // reset the ff to top left
            ff.x = 0;
            ff.y = 0;
            // same as what done above, but for y-axis
            while(ff.y + ff.y / 2 < ff.parent.height / 2){
                compare(Grid.AlignTop, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20
                wait(50)
            }
            while(ff.y < ff.parent.height) {
                compare(Grid.AlignBottom, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20;
                wait(50);
            }
        }

        // click floorfilter button and open the site/facility listview
        function test_floorfilterOpen() {
            var ff = createTemporaryObject(ffComponent, foo);
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            var internal = Utils.findChild(ff, "internal");
            var listView = Utils.findChild(ff, "listView");
            verify(internal)
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Site);
            compare(false, listView.visible);
            mouseClick(buildingMenuButton);
            compare(true, buildingMenuButton.checked);
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Site);
            verify(listView);
            compare(true, listView.visible);
            var backToSite = Utils.findChild(ff, "backToSite");
            // visible but with width size of 0
            compare(true, backToSite.visible);
            compare(0, backToSite.width);

            wait(1000);
        }

        function test_listViewCloser() {
            var ff = createTemporaryObject(ffComponent, foo);
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            compare(true, listView.visible);
            compare(true, buildingMenuButton.checked);
            var closerListView = Utils.findChild(ff, "closerListView");
            verify(closerListView);
            mouseClick(closerListView);
            compare(false, listView.visible);
            compare(false, buildingMenuButton.checked);
        }

        SignalSpy {
            id: spyDone
            signalName: "loaded"
        }

        function test_filter() {
            var ff = createTemporaryObject(ffMultiComponent, foo);
            spyDone.target = ff.controller;
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                spyDone.wait(10000);
            verify(ff.controller.floorManager.loadStatus === Enums.LoadStatusLoaded);
            verify(ff.controller.floorManager.levels.length > 0)
            wait(3000)
            verify(listView.model.count > 0);
            var searchTextField = findChild(ff, "searchTextField");
            verify(searchTextField)
            searchTextField.text = "esri redlands learning center";
            compare(1, listView.model.count);
            var noResultsFoundLabel = Utils.findChild(ff, "noResultsFoundLabel");
            compare(false, noResultsFoundLabel.visible);
            searchTextField.text = "asd";
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
            var ff = createTemporaryObject(ffMultiComponent, foo);
            spyDone.target = ff.controller;
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                spyDone.wait(10000);
            verify(listView.model.count > 0);
            var siteLabel = Utils.findChild(ff, "siteLabel");
            compare("Select the Site", siteLabel.text);
            // click the first element in the list view at central x position and 5 y axis
            var internal = Utils.findChild(ff, "internal");
            compare(-1, internal.selectedSiteIdx);
            console.log(listView.padding, listView.spacing, listView.margin);
            // select site
            mouseClick(listView, 25, 25);
            compare(0, internal.selectedSiteIdx);
            //tryCompare(internal, "selectedSiteIdx", 0, 5000, "never set the site idx");
            compare("Esri Redlands Learning Center", siteLabel.text);
            var backToSite = Utils.findChild(ff, "backToSite");
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
            compare(0, internal.selectedFacilityIdx);
        }

        // check all sites functionality, flickable/repeater and select levels.
        function test_allSites_levels() {
            // todo: checks on delegate text (composed by facility + new line + parent site)
            var ff = createTemporaryObject(ffMultiComponent, foo);
            spyDone.target = ff.controller;
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                spyDone.wait(10000);
            var showAllFacilities = Utils.findChild(ff, "showAllFacilities");
            // show all facilities
            mouseClick(showAllFacilities, showAllFacilities.width / 2, showAllFacilities.height / 2, Qt.LeftButton, Qt.NoModifier, 100);
            verify(listView.model.count > 0);
            var internal = Utils.findChild(ff, "internal");
            compare(FloorFilter.VisibleListView.Facility, internal.currentVisibileListView);
            // search for 3 level facility
            var searchTextField = findChild(ff, "searchTextField");
            searchTextField.text = "red o";
            // select first facility
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(16, internal.selectedFacilityIdx);
            var siteLabel = Utils.findChild(ff, "siteLabel");
            var facilityLabel = Utils.findChild(ff, "facilityLabel");
            compare("Esri Redlands Main Campus", siteLabel.text);
            compare("RED O", facilityLabel.text);
            // check repeater
            var repeater = Utils.findChild(ff, "repeater");
            compare(3, repeater.count);
            var flickable = Utils.findChild(ff, "flickable");
            // check the height of the flickable is number of items * buttonheight
            // wait that the delegate sets the buttonHeight once it is completed.
            tryCompare(flickable, "height", 2 * repeater.buttonHeight, 50);
            // check the flickable scrollable height is 3 * buttonHeight
            compare(120, flickable.contentHeight);

            // select first level and check visiblity, text on gui, text on controller and checked status
            var wrapper = Utils.findChild(ff, "wrapper");
            mouseClick(flickable, 10, 10);
            compare("1000.US01.MAIN.O3", ff.controller.selectedLevelId);
            compare("O3", wrapper.children[0].text);
            verify(wrapper.children[0].checked);
            verify(ff.controller.selectedLevel.visible === true);
            // select second level
            mouseClick(flickable, 10, 50);
            compare("1000.US01.MAIN.O2", ff.controller.selectedLevelId);
            compare("O2", wrapper.children[1].text);
            verify(wrapper.children[1].checked);
            verify(ff.controller.selectedLevel.visible === true);
            // select third level. have to scroll down (negative means scroll down, positive means scroll up). 120 is 15 degrees of scoll (enough for our needs).
            mouseWheel(flickable, 10, 10, 0, -120, Qt.NoButton);
            wait(1000);
            // select the third level, which now is at second position
            mouseClick(flickable, 10, 50);
            wait(100)
            compare("1000.US01.MAIN.O1", ff.controller.selectedLevelId);
            compare("O1", wrapper.children[2].text);
            verify(wrapper.children[2].checked);
            // check the other levels are not checked anymore
            verify(!wrapper.children[0].checked);
            verify(!wrapper.children[1].checked);
            verify(ff.controller.selectedLevel.visible === true);
        }

        // zoom button enabled or not. Check if it changes viewpoint by spying on setViewpointCompleted
        function test_zoom() {
            var ff = createTemporaryObject(ffMultiComponent, foo);
            spyDone.target = ff.controller;
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                spyDone.wait(10000);
            var zoom = Utils.findChild(ff, "zoom");
            compare(false, zoom.enabled);
            // select a site
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(false, zoom.enabled);
            // back to siteview
            var backToSite = Utils.findChild(ff, "backToSite");
            mouseClick(backToSite);
            compare(true, zoom.enabled);
            wait(1000);
            // pan mapview
            mouseDrag(viewMulti, viewMulti.width / 2, viewMulti.height / 2, 30, 0);
            wait(1000);
            // spy on the setViewpoint
            var spyViewpoint = createTemporaryQmlObject('import QtQuick 2.0; SignalSpy { target: viewMulti; signalName : "setViewpointCompleted" }', foo);
            mouseClick(zoom);
            wait(spyViewpoint);
            wait(1000);
            // select site again, go to facility view
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            compare(false, zoom.enabled);
            mouseDrag(viewMulti, viewMulti.width / 2, viewMulti.height / 2, 30, 0);
            wait(1000);
            // select a facility
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            spyViewpoint.clear();
            mouseClick(zoom);
            spyViewpoint.wait();
        }

        function test_close() {
            var ff = createTemporaryObject(ffMultiComponent, foo);
            spyDone.target = ff.controller;
            var buildingMenuButton = Utils.findChild(ff, "buildingMenuButton");
            // show the listview
            mouseClick(buildingMenuButton);
            var listView = Utils.findChild(ff, "listView");
            var showAllFacilities = Utils.findChild(ff, "showAllFacilities");
            // if floorManager already loaded, skip the wait. Otherwise wait for the signal
            if(ff.controller.floorManager != null ? ff.controller.floorManager.loadStatus !== Enums.LoadStatusLoaded : true)
                spyDone.wait(10000);
            mouseClick(showAllFacilities);
            // select first all facility (red a)
            mouseClick(listView, 25, 25, Qt.LeftButton, Qt.NoModifier, 100);
            var closer = Utils.findChild(ff, "closer");
            verify(closer);
            var itemSelectedButton = Utils.findChild(ff, "itemSelectedButton");
            compare(false, itemSelectedButton.visible);
            compare(true, closer.visible);
            // close the floorfilter: collapses the levels and only show the current selected
            // have to use 10ms delay, otherwise the click is done befor that the closer is available
            mouseClick(closer, closer.width / 2, closer.height / 2, Qt.LeftButton, Qt.NoModifier, 10);
            var flickable = Utils.findChild(ff, "flickable");
            compare(false, closer.visible);
            compare(false, flickable.visible);
            compare(true, itemSelectedButton.visible);
            compare("A1", itemSelectedButton.text);
            // check the not collapsed itemSelectedButton
            var collapser = Utils.findChild(ff, "collapser");
            mouseClick(collapser);
            compare("Level 1", itemSelectedButton.text);
            // reopen the floorfilter, showing all the levels available
            mouseClick(itemSelectedButton);
            compare(true, flickable.visible);
            compare(true, closer.visible);
            compare(false, itemSelectedButton.visible);
        }
    }
}
