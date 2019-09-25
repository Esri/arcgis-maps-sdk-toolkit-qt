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
import Esri.CppArExample 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
        tracking: true
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
    }

    CppArExample {
        id: arSample
        arcGISArView: arcGISArView
        sceneView: sceneView
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

        onShowPointCloud: arSample.showPointCloud(visible);
        onShowPlanes: arSample.showPlanes(visible);

        onEmptySceneClicked: arSample.createEmptyScene();
        onStreetsSceneClicked: arSample.createStreetsScene();
        onImagerySceneClicked: arSample.createImageryScene();
        onFullScaleTestSceneClicked: arSample.createFullScaleTestScene();

        onPointCloudSceneClicked: arSample.createPointCloudScene();
        onYosemiteSceneClicked: arSample.createYosemiteScene();
        onBorderSceneClicked: arSample.createBorderScene();
        onBrestSceneClicked: arSample.createBrestScene();
        onBerlinSceneClicked: arSample.createBerlinScene();
        onTabletopTestSceneClicked: arSample.createTabletopTestScene();
    }
}
