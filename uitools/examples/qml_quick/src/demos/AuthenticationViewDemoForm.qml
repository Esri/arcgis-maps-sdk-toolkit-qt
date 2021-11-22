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
    readonly property string exampleUrl: "https://www.arcgis.com/sharing/rest/content/items/e5039444ef3c48b8a8fdc9227f9be7c1/data"

    mapViewContents: Component {
//! [Set up Authentication View QML]
        MapView {
            Map {
                initUrl: exampleUrl
            }
            AuthenticationView {
                anchors.centerIn: parent
            }
        }
//! [Set up Authentication View QML]
    }
}
