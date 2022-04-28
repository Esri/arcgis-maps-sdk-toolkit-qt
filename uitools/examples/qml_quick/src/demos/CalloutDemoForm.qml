// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.15
import Esri.ArcGISRuntime.Toolkit.CppApi 100.15

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
                imageUrl: "qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/map_pin.png"
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
