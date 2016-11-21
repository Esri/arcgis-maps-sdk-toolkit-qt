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
    id: homeButton

    //must be the same spatial reference as the MapView
    property Envelope homeExtent
    property MapView mapview: null

    iconSource: "images/home.png"
    tooltip: qsTr("Home")

    onClicked: {
        if(mapview){
            if(homeExtent)
                mapview.setViewpointGeometry(homeExtent);
            else
                mapview.setViewpointGeometry(mapview.map.initialViewpoint.extent);
        }
    }

    QtObject {
        id: internal
        property real _size: size * displayScaleFactor
    }
}
