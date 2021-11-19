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
import QtQuick 2.12
import QtQuick.Controls 2.12
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13
import DemoApp 1.0

DemoPage {
    sceneViewContents: Component {
        SceneView {
            id: view
            CoordinateConversion {
                id: gallery
                geoView: parent
                anchors {
                    right: parent.right
                    margins: 10
                    bottom: parent.attributionTop
                }
            }
            CoordinateConversionDemo {
                geoView: view
            }
        }
    }

    mapViewContents: Component {
        MapView {
            id: view
            CoordinateConversion {
                geoView: parent
                anchors {
                    right: parent.right
                    margins: 10
                    bottom: parent.attributionTop
                }
            }
            CoordinateConversionDemo {
                geoView: view
            }
        }
    }
}
