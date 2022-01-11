
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

    property var geoView

    //refresh()?
    property string selectedLevelId

    property string selectedFacilityId: "Select a Facility"

    property string selectedSiteId: "Select a Site"

    property ListModel floors: ListModel {
        ListElement {
            shortName: "F1"
        }
        ListElement {
            shortName: "F2"
        }
        ListElement {
            shortName: "FF"
        }
    }

    property ListModel facilities: ListModel {
        ListElement {
            name: "T1"
        }
        ListElement {
            name: "T2"
        }
    }

    property ListModel sites: ListModel {
        ListElement {
            name: "A1"
        }
        ListElement {
            name: "A2"
        }
    }

    // not directly binding. update it when facilities is changed, otherwise the binding is broken everytime the filter method is called.
    property ListModel filteredFacilities: ListModel {}

    property ListModel filteredSites: ListModel {}

    onFacilitiesChanged: {
        copyFacilities()
    }

    onSelectedFacilityIdChanged: {
        onSelectedChanged()
    }
    onSelectedLevelIdChanged: onSelectedChanged()
    onSelectedSiteIdChanged: onSelectedChanged()

    function onSelectedChanged() {}

    function filterFacilities(query) {
        filteredFacilities.clear()
        console.log(facilities.count)
        for (var i = 0; i < facilities.count; ++i) {
            const facility = facilities.get(i)
            if (facility.name.includes(query))
                filteredFacilities.append(facility)
        }
    }

    function copyFacilities() {
        for (var i = 0; i < facilities.count; ++i) {
            let facility = facilities.get(i)
            filteredFacilities.append(facility)
        }
    }

    Component.onCompleted: {
        //copy all the intial facilities into the filteredfacilites
        copyFacilities()
    }
}
