// Copyright 2022 ESRI
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
import Esri.ArcGISRuntime.Toolkit 100.15

DemoPage {
    mapViewContents: Component {
        MapView {
            id: view

            Map {
                PortalItem {
                   itemId: "16f1b8ba37b44dc3884afc8d5f454dd2"
                }
            }

            BookmarksView {
                id: bookmarksView
                geoView: view
                anchors {
                    left: parent.left
                    top: parent.top
                    margins: 10
                }
            }
        }
    }
}
