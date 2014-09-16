/*******************************************************************************
 * Copyright 2012-2014 Esri
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
import ArcGIS.Runtime.AppKit 1.0
import ArcGIS.Runtime 10.3

Button {
    id: positionButton

    property bool isActive: map && map.positionDisplay.positionSource && map.positionDisplay.positionSource.active
    property int maxModes: map.positionDisplay.isCompassAvailable ? 4 : 3;

    visible: buttons & buttonPosition && map && map.positionDisplay.positionSource
    width: internal._size
    height: width
    iconSource: isActive ? modeImage(map.positionDisplay.mode) : "images/position-off.png"
    tooltip: qsTr("Location")

    MouseArea {
        anchors.fill: parent

        onPressAndHold: {
            fader.start();

            if (map.positionDisplay.positionSource.active) {
                map.positionDisplay.positionSource.active = false;
            }
        }

        onClicked: {
            fader.start();

            if (map.positionDisplay.positionSource.active) {
                map.positionDisplay.mode = (map.positionDisplay.mode + 1) % positionButton.maxModes;
            } else {
                map.positionDisplay.positionSource.active = true;
                map.positionDisplay.mode = 1;
            }
        }
    }

    function modeImage(mode) {
        switch (mode) {
        case 0 :
            return "images/position-on.png";

        case 1 :
            return "images/position-autopan.png";

        case 2 :
            return "images/position-navigation.png";

        case 3 :
            return "images/position-compass.png"
        }
    }
}
