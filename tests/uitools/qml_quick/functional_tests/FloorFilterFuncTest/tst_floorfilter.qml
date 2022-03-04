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
    color: "cyan"

    MouseArea {
        id: area
        anchors.fill: parent
    }

    property bool bar: true

    Item {
        id: i
    }

    Component {
        id: ffComponent
        FloorFilter {

        }
    }

    //    FloorFilter {
    //        id: ff

    //        //geoView: viewSingle
    //    }
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
        //    MapView {
        //        id: viewMulti
        //        Map {
        //            id: mapMulti
        //            item: PortalItem {
        //                itemId: "49520a67773842f1858602735ef538b5"
        //                portal: Portal {
        //                    Component.onCompleted: load()
        //                    onErrorChanged: {
        //                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
        //                            console.log(error.message);
        //                        }
        //                    }
        //                    credential: viewerCredentialIndoors
        //                    url: "https://indoors.maps.arcgis.com/"
        //                }
        //            }
        //        }
        //    }

        //    MapView {
        //        id: viewSingle
        //        Map {
        //            id: mapSingle
        //            item: PortalItem {
        //                itemId: "f133a698536f44c8884ad81f80b6cfc7"
        //                portal: Portal {
        //                    Component.onCompleted: load()
        //                    onErrorChanged: {
        //                        if (loadStatus === Enums.LoadStatusFailedToLoad) {
        //                            console.log(error.message);
        //                        }
        //                    }
        //                    url: "https://www.arcgis.com"
        //                }
        //            }
        //        }
        //    }

        // 1 site, 1 facility, 3 levels

        //        Component {
        //            id: ffSingleComponent

        //        }

        // more sites, more facilities
        Component {
            id: ffMultiComponent
            FloorFilter {
                geoView: viewMulti
            }
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

            mouseClick(collapser);
            compare("chevrons-left.svg", Utils.urlFileName(collapser.icon.source.toString()))
            mouseClick(collapser);

            // move ff until its center point is < parent center
            while(ff.x + ff.width / 2 < ff.parent.width / 2){
                compare(Qt.LeftToRight,  gridOuterWrapper.layoutDirection)
                compare("chevrons-right.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(200);
            }
            // move ff until x-axis-end of parent
            while(ff.x < ff.parent.width){
                compare(Qt.RightToLeft,  gridOuterWrapper.layoutDirection)
                compare("chevrons-left.svg", Utils.urlFileName(collapser.icon.source.toString()))
                ff.x += 20;
                wait(200);
            }

            // reset the ff to top left
            ff.x = 0;
            ff.y = 0;
            // same as what done above, but for y-axis
            while(ff.y + ff.y / 2 < ff.parent.height / 2){
                compare(Grid.AlignTop, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20
                wait(100)
            }
            while(ff.y < ff.parent.height) {
                compare(Grid.AlignBottom, gridOuterWrapper.verticalItemAlignment);
                ff.y += 20;
                wait(100);
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
            compare(internal.currentVisibileListView, FloorFilter.VisibleListView.Site);
            verify(listView);
            compare(true, listView.visible);
            var backToSite = Utils.findChild(ff, "backToSite");
            // visible but with width size of 0
            compare(true, backToSite.visible);
            compare(0, backToSite.width);

            wait(1000);
        }

        function test_zextra() {
            var ff = createTemporaryObject(ffComponent, foo);
            wait(1000)
        }

    }
}
