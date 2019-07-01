
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
//

import QtQuick 2.6
import QtQuick.Controls 2.2
import QtMultimedia 5.8
import Esri.ArSample 1.0
//TODO: import Esri.ArcGISRuntime.Toolkit.Controls.CppApi 100.6

Item {
    id: root

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    ArSample {
        id: model // TODO: not sure "model" is a good name. And it's useless to set the id for this item
        arcGISArView: arcGISArView
        sceneView: sceneView
    }
}
