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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtPositioning 5.2
import ArcGIS.Runtime 10.25
import ArcGIS.Runtime.Toolkit.Controls 1.0

Rectangle {
    width: 500
    height: 400

    Map {
        id: map
        anchors.fill: parent
        wrapAroundEnabled: true
        mapPanningByMagnifierEnabled: true
        magnifierOnPressAndHoldEnabled: true
        zoomByPinchingEnabled: true
        positionDisplay {
            positionSource: PositionSource {
            }
        }

        ArcGISTiledMapServiceLayer {
            url: "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer"
        }

        NavigationToolbar {
            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }
    }
}
