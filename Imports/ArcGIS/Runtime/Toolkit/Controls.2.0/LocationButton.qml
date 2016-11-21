/*******************************************************************************
 * Copyright 2012-2016 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import Esri.ArcGISRuntime 100.0

StyleButton {
    id: locationButton
    property bool isActive: mapview && mapview.locationDisplay.started
    property int maxModes: 4;
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property MapView mapview: null

    visible: mapview && mapview.locationDisplay
    iconSource: isActive ? modeImage(mapview.locationDisplay.autoPanMode) : "images/position-off.png"
    tooltip: qsTr("Location")

    MouseArea {
        anchors.fill: parent

        onPressAndHold: {
            if (mapview.locationDisplay.started){
                mapview.locationDisplay.stop();
                mapview.locationDisplay.autoPanMode = Enums.LocationDisplayAutoPanModeOff;
            }
        }

        onClicked: {
            if (mapview.locationDisplay.started)
                mapview.locationDisplay.autoPanMode = (mapview.locationDisplay.autoPanMode+1) % locationButton.maxModes;
            else {
                mapview.locationDisplay.positionSource.active = true;
                mapview.locationDisplay.start();
                mapview.locationDisplay.autoPanMode = Enums.LocationDisplayAutoPanModeOff;
            }
        }
    }

    function modeImage(mode) {
        switch (mode) {
        case Enums.LocationDisplayAutoPanModeOff :
            return "images/position-on.png";
        case Enums.LocationDisplayAutoPanModeDefault :
            return "images/position-autopan.png";
        case Enums.LocationDisplayAutoPanModeNavigation :
            return "images/position-navigation.png";
        case Enums.LocationDisplayAutoPanModeCompassNavigation :
            return "images/position-compass.png"
        }
    }

    QtObject {
        id: internal
        property real _size: size * displayScaleFactor
    }
}
