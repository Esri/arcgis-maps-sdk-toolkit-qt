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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtPositioning 5.2
import Esri.ArcGISRuntime 100.0
import Esri.ArcGISRuntime.Toolkit.Controls 2.0

Rectangle {
    width: 500
    height: 400

    MapView {
        id: mapview
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeDisabled
        magnifierEnabled: true
        zoomByPinchingEnabled: true
        allowMagnifierToPanMap: true
        rotationByPinchingEnabled: true

        Map {
           BasemapNationalGeographic {}
        }

        // set the location display's position source
        locationDisplay {
            positionSource: PositionSource {
                active: true
            }
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
