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

QtObject {
    id: controller

    property var geoView;

    //refresh()?

    property string selectedLevelId;

    property string selectedFacilityId;

    property string selectedSiteId;

    property ListModel floors: ListModel {
        ListElement {
            name: "F1"
        }
        ListElement {
            name: "F2"
        }
        ListElement {
            name: "FF"
        }
    };

    property ListModel facilities : ListModel {
        ListElement {
            name: "Facility1"
        }
        ListElement {
            name: "Facility2"
        }
    }

    onSelectedFacilityIdChanged: onSelectedChanged()
    onSelectedLevelIdChanged: onSelectedChanged()
    onSelectedSiteIdChanged: onSelectedChanged()

    function onSelectedChanged() {
        console.log(controller.selectedFacilityId);
    }

    function createFloorElement() {

    }


}