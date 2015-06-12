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

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import Esri.ArcGISRuntime 100.0

Item {
    id: fader
    
    property QtObject target: parent
    property bool enabled: true
    property int timeoutDuration: 3000
    property int animationDuration: 3000
    property real minumumOpacity: 0.35
    property real maximumOpacity: 1
    property string platform: Qt.platform.os

    Component.onCompleted: {
        if (platform !== "android" && platform !== "ios")
            if (enabled)
                start();
    }
    
    function start() {
        if (platform !== "android" && platform !== "ios"){
            if (!enabled)
                return;
            fadeTimer.stop();
            fadeAnimation.stop();
            target.opacity = maximumOpacity;
            fadeTimer.start();
        }
    }
    
    function stop() {
        if (platform !== "android" && platform !== "ios") {
            fadeTimer.stop();
            fadeAnimation.stop();
            target.opacity = maximumOpacity;
        }
    }
    
    onEnabledChanged: {
        if (platform !== "android" && platform !== "ios") {
            if (enabled)
                start();
            else
                stop();
        }
    }

    PropertyAnimation {
        id: fadeAnimation
        target: fader.target
        property: "opacity"
        to: minumumOpacity
        duration: animationDuration
        easing {
            type: Easing.OutCubic
        }
    }

    Timer {
        id: fadeTimer
        repeat: false
        interval: timeoutDuration
        onTriggered: {
            fadeAnimation.restart();
        }
    }
}
