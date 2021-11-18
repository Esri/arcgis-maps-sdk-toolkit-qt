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
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13

DemoPage {
    property real xClickLoc
    property real yClickLoc

    mapViewContents: Component {
        //! [Set up Callout QML]
        MapView {
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
                calloutData :  parent.calloutData
                calloutHeight: 150
                calloutWidth: 250
                accessoryButtonHidden: true
                leaderPosition: leaderPositionEnum.Automatic

                property Point location;
            }
            onMouseClicked : {
                if (callout.calloutVisible) {
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
