/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
import QtQuick

SequentialAnimation {
    id: anim
    property int delay: 0;
    property var target;
    PauseAnimation {
        duration: anim.delay
    }
    SequentialAnimation {
        loops: Animation.Infinite
        alwaysRunToEnd: true
        ParallelAnimation {
            PropertyAnimation {
                target: anim.target
                properties: "height"
                from:  rects.rectHeight
                to: rects.rectMaxHeight
                duration: 400
                easing.type: Easing.InSine
            }
            PropertyAnimation {
                target: anim.target
                properties: "opacity"
                from:  0.3
                to: 1.0
                duration: 400
                easing.type: Easing.InSine
            }
        }
        ParallelAnimation {
            PropertyAnimation {
                target: anim.target
                properties: "height"
                from: rects.rectMaxHeight
                to: rects.rectHeight
                duration: 400
                easing.type: Easing.OutSine
            }
            PropertyAnimation {
                target: anim.target
                properties: "opacity"
                from:  1.0
                to: 0.3
                duration: 400
                easing.type: Easing.OutSine
            }
        }
    }
}
