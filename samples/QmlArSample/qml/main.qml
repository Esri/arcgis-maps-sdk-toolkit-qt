
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
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISARView 1.0

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "QmlArSample"

    Component.onCompleted: arcGISArView.startTracking();

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
        originCamera: sceneLoader.item ? sceneLoader.item.camera : null
        translationFactor: sceneLoader.item ? sceneLoader.item.translationFactor : 1
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
        spaceEffect: Enums.SpaceEffectTransparent
        atmosphereEffect: Enums.AtmosphereEffectNone
        scene: sceneLoader.item ? sceneLoader.item.scene : null
    }

    Loader {
        id: sceneLoader
        source: "qrc:/qml/BrestScene.qml";
    }

    Column {
        anchors {
            top: parent.top
            left: parent.left
            margins: 5
        }
        spacing: 5
        Button {
            text: "Reset"
            onClicked: arcGISArView.resetTracking();
        }
        Button {
            text: "Brest"
            onClicked: sceneLoader.source = "qrc:/qml/BrestScene.qml";
        }
        Button {
            text: "Berlin"
            onClicked: sceneLoader.source = "qrc:/qml/BerlinScene.qml";
        }
    }
}
