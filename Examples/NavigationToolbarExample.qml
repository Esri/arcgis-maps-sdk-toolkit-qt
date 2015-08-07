/*******************************************************************************
 * Copyright 2012-2015 Esri
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

import QtPositioning 5.3
import QtQuick 2.3
import QtQuick.Controls 1.2
import ArcGIS.Runtime 10.26
import ArcGIS.Runtime.Toolkit.Controls 1.0

Rectangle {
    width: 500
    height: 400

    Map {
        id: mainMap
        anchors.fill: parent
        wrapAroundEnabled: true
        focus: true

        ArcGISTiledMapServiceLayer {
            url: "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer"
        }

        PositionSource {
            id: position
        }

        NavigationToolbar {
            id: navigationToolbar
            map: mainMap
            anchors {
                top: parent.top
                left: parent.left
                margins: 10
            }
        }

        onStatusChanged: {
            if (status === Enums.MapStatusReady) {
                mainMap.positionDisplay.positionSource = position;
            }
        }
    }
}
