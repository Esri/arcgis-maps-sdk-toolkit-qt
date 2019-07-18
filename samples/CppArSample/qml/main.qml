
// Copyright 2019 ESRI
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
import QtQuick.Controls 2.2
import Esri.CppArSample 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
    }

    CppArSample {
        id: arSample
        arcGISArView: arcGISArView
        sceneView: sceneView
    }

    Column {
        anchors {
            top: parent.top
            left: parent.left
            margins: 5
        }
        spacing: 5
        Button {
            text: "Brest"
            onClicked: arSample.createBrestScene();
        }
        Button {
            text: "Berlin"
            onClicked: arSample.createBerlinScene();
        }
    }
}
