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
import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.14
import Esri.ArcGISRuntime.Toolkit 100.14
import DemoApp 1.0

DemoPage {
    mapViewContents: Component {
        MapView {
            id: view
            FloorFilter {
                geoView: parent
                anchors {
                    left: parent.left
                    margins: 10
                    top: parent.top
                }
            }
            FloorFilterDemo {
                geoView: view
            }
        }
    }
}
