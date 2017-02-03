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

StyleToolbar {
    id: navigationToolbar

    property real zoomRatio: 2
    property Envelope homeExtent
    readonly property int buttonZoomOut: 0x02
    readonly property int buttonPosition: 0x08
    readonly property int buttonZoomIn: 0x01
    readonly property int buttonHome: 0x04
    property string platform: Qt.platform.os
    property int buttons: buttonZoomIn + buttonZoomOut + buttonHome + buttonPosition

    Component.onCompleted: {
        if (!mapview && parent && parent.objectType && parent.objectType === "MapView"){
            mapview = parent;
            home.mapview = mapview
            zoomIn.mapview = mapview
            zoomOut.mapview = mapview
            currentLocation.mapview = mapview
        }

    }

    function stopFader() {
        zoomIn.fader.stop();
        home.fader.stop();
        zoomOut.fader.stop();
        currentLocation.fader.stop();
    }

    function startFader() {
        zoomIn.fader.start();
        home.fader.start();
        zoomOut.fader.start();
        currentLocation.fader.start();
    }

    //--------------------------------------------------------------------------
    // Zoom-In Button
    ZoomInButton {
        id: zoomIn

        onHoveredChanged: {
            if (platform !== "android" && platform !== "ios") {
                if (zoomIn.hovered)
                    stopFader();
                else
                    startFader();
            }
        }
        onClicked: {
            if (platform !== "android" && platform !== "ios")
                startFader();
        }
    }

    //--------------------------------------------------------------------------
    // Home Button
    HomeButton {
        id: home
        onHoveredChanged: {
            if (platform !== "android" && platform !== "ios") {
                if (home.hovered)
                    stopFader();
                else
                    startFader();
            }
        }
        onClicked: {
            if (platform !== "android" && platform !== "ios")
                startFader();
        }
    }

    //--------------------------------------------------------------------------
    // Zoom-Out Button
    ZoomOutButton {
        id: zoomOut

        onHoveredChanged: {
            if (platform !== "android" && platform !== "ios") {
                if (zoomOut.hovered)
                    stopFader();
                else
                    startFader();
            }
        }
        onClicked: {
            if (platform !== "android" && platform !== "ios")
                startFader();
        }
    }

    //--------------------------------------------------------------------------
    // LocationButton
    LocationButton {
        id: currentLocation

        onHoveredChanged: {
            if (platform !== "android" && platform !== "ios") {
                if (currentLocation.hovered)
                    stopFader();
                else
                    startFader();
            }
        }
        onClicked: {
            if (platform !== "android" && platform !== "ios")
                startFader();
        }
    }
}

