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

import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISArToolkit 1.0

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "QmlArExample"

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
        originCamera: sceneLoader.item ? sceneLoader.item.originCamera : null
        locationDataSource: sceneLoader.item ? sceneLoader.item.locationDataSource : null
        translationFactor: sceneLoader.item ? sceneLoader.item.translationFactor : 1
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
        scene: sceneLoader.item ? sceneLoader.item.scene : null
        onMousePressed: arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen event
    }

    Loader {
        id: sceneLoader
    }

    SettingsWindow {
        anchors {
            top: parent.top
            left: parent.left
            margins: 5
        }

        onStartTrackingClicked: arcGISArView.startTracking();
        onStopTrackingClicked: arcGISArView.stopTracking();
        onResetTrackingClicked: arcGISArView.resetTracking();
        // onCalibrationClicked: not implemented
        onEmptySceneClicked: sceneLoader.source = "qrc:/qml/scenes/EmptyScene.qml";
        onStreetsSceneClicked: sceneLoader.source = "qrc:/qml/scenes/StreetsScene.qml";
        onImagerySceneClicked: sceneLoader.source = "qrc:/qml/scenes/ImageryScene.qml";
        onPointCloudSceneClicked: sceneLoader.source = "qrc:/qml/scenes/PointCloudScene.qml";
        onYosemiteSceneClicked: sceneLoader.source = "qrc:/qml/scenes/YosemiteScene.qml";
        onBorderSceneClicked: sceneLoader.source = "qrc:/qml/scenes/BorderScene.qml";
        onBrestSceneClicked: sceneLoader.source = "qrc:/qml/scenes/BrestScene.qml";
        onBerlinSceneClicked: sceneLoader.source = "qrc:/qml/scenes/BerlinScene.qml";
        onTabletopTestSceneClicked: {
            sceneLoader.source = "qrc:/qml/scenes/TabletopTestScene.qml";
            sceneView.graphicsOverlays.clear();
            sceneView.graphicsOverlays.append(sceneLoader.item.graphicsOverlay);
        }
    }
}
