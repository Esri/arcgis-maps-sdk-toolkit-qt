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

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit 100.15

DemoPage {
    property real xClickLoc
    property real yClickLoc

    mapViewContents: Component {
//! [Set up Callout QML]
        MapView {
            id: view
            Map {
                initBasemapStyle: Enums.BasemapStyleArcGISDarkGray
            }
            calloutData {
                imageUrl: "qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin-tear.svg"
                title: "This is the location callout"
                detail: "x: " + xClickLoc + " y: " + yClickLoc
            }

            Callout{
                id:callout
                calloutData: view.calloutData
                accessoryButtonVisible: false
            }
            onMouseClicked : {
                if (calloutData.visible) {
                    callout.dismiss();
                } else {
                    xClickLoc = mouse.mapPoint.x.toFixed(2);
                    yClickLoc = mouse.mapPoint.y.toFixed(2);
                    calloutData.location = mouse.mapPoint
                    callout.showCallout()
                }
            }
        }
//! [Set up Callout QML]
    }
}
