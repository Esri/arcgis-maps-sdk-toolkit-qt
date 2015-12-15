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
import QtQuick.Window 2.2
import Esri.ArcGISRuntime 100.00

Item {
    id: northArrow

    property alias image: image
    property MapView mapview: null
    property real size: 40
    property bool resetOnClick: true
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    signal clicked()

    implicitWidth: 40
    implicitHeight: 40
    width: internal._size
    height: width

    Component.onCompleted: {
        if (!mapview && parent) //&& parent.objectType && parent.objectType === "MapView")
            mapview = parent;
    }

    QtObject {
        id: internal

        property real _size: size * displayScaleFactor
    }

    Image {
        id: image
        anchors.fill: parent
        source: "images/NorthArrow.png"
        rotation: mapview != null ? -mapview.mapRotation : 0
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            if (mapview && resetOnClick)
                mapview.setViewpointRotation(0);
            northArrow.clicked();
        }
    }
}
