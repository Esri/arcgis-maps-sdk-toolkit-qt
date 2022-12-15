/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
import QtQuick.Controls
import QtQuick.Layouts
import Esri.ArcGISRuntime
import Esri.ArcGISRuntime.Toolkit
import DemoApp

DemoPage {
    mapViewContents: Component {
        MapView {
            id:view

            Callout {
                id: callout
                calloutData : view.calloutData //binding to parent. Any change is reflected on this
                accessoryButtonVisible: false
                implicitHeight: 100
                leaderPosition: Callout.LeaderPosition.Automatic
                maxWidth: 250
                background: Rectangle {
                    radius: 5
                    border.width: 2
                    border.color: "black"
                }
                palette.windowText: "black"
            }
            CalloutDemo {
                geoView: view
            }
        }
    }
}
