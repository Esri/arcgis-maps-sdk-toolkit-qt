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

        onShowPointCloud: {
            if (visible)
              arcGISArView.pointCloudColor = Qt.rgba(0.39, 0.39, 1.0, 1.0);
            else
              arcGISArView.pointCloudColor = "";
        }

        onShowPlanes: {
            if (visible)
              arcGISArView.planeColor = Qt.rgba(1.0, 0.0, 0.0, 0.039);
            else
              arcGISArView.planeColor = "";
        }

        onEmptySceneClicked: changeScene("qrc:/qml/scenes/EmptyScene.qml");
        onStreetsSceneClicked: changeScene("qrc:/qml/scenes/StreetsScene.qml");
        onImagerySceneClicked: changeScene("qrc:/qml/scenes/ImageryScene.qml");

        onPointCloudSceneClicked: changeScene("qrc:/qml/scenes/PointCloudScene.qml");
        onYosemiteSceneClicked: changeScene("qrc:/qml/scenes/YosemiteScene.qml");
        onBorderSceneClicked: changeScene("qrc:/qml/scenes/BorderScene.qml");
        onBrestSceneClicked: changeScene("qrc:/qml/scenes/BrestScene.qml");
        onBerlinSceneClicked: changeScene("qrc:/qml/scenes/BerlinScene.qml");
        onTabletopTestSceneClicked: {
            changeScene("qrc:/qml/scenes/TabletopTestScene.qml");
            sceneView.graphicsOverlays.append(sceneLoader.item.graphicsOverlay);
        }

        function changeScene(sceneSource) {
            // stop tracking
            arcGISArView.stopTracking();

            // set the new scene
            sceneLoader.source = sceneSource;
            sceneView.graphicsOverlays.clear();

            // reset and start tracking
            if (arcGISArView.tracking)
                arcGISArView.resetTracking();
            else
                arcGISArView.startTracking();
        }
    }
}
