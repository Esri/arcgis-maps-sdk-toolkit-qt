
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

import QtQuick.Controls 2.2
import Esri.CppArSample 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    CppArSample {
        id: model // TODO: not sure "model" is a good name. And it's useless to set the id for this item
        arcGISArView: arcGISArView
        sceneView: sceneView
    }
}
