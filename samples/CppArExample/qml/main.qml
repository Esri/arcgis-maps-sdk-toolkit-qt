/*******************************************************************************
 *  Copyright 2012-2019 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

import QtQuick 2.12
import QtQuick.Controls 2.2
import Esri.CppArExample 1.0
import Esri.ArcGISArToolkit 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    // Create AR
    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
        tracking: true
    }

    // Create SceneQuickView here, and create its Scene etc. in C++ code
    SceneView {
        id: sceneView
        anchors.fill: parent
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    CppArExample {
        id: arSample
        arcGISArView: arcGISArView
        sceneView: sceneView
    }

    // Used for screenToLocation test.
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: false
        onClicked: arSample.screenToLocation(mouseX, mouseY);
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
        onScreenToLocationClicked: mouseArea.enabled = !mouseArea.enabled;

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
