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

import QtQuick
import QtQuick.Controls
import Esri.CppArExample
import Esri.ArcGISArToolkit

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
        property bool transparentMode: calibrationView.visible || waitingInitMessage.visible || waitingLocationMessage.visible
        opacity: transparentMode ? 0.35 : 1.0
    }

    // Declare the C++ instance which creates the scene etc. and supply the view
    CppArExample {
        id: arSample
        arcGISArView: arcGISArView
        sceneView: sceneView
    }

    Column {
        anchors {
            right: parent.right
            left: parent.left
            margins: 10
            bottom: parent.bottom
            bottomMargin: 60
        }
        spacing: 5

        Message {
            id: waitingInitMessage
            width: parent.width
            visible: arSample.tabletopMode && arSample.waitingForInitialization
            text: "Touch screen to place the tabletop scene..."
        }

        Message {
            id: waitingLocationMessage
            width: parent.width
            visible: !arSample.tabletopMode && arSample.waitingForInitialization
            text: "Waiting for location..."
        }

        CalibrationView {
            id: calibrationView
            width: parent.width
            visible: false
            onTriggered: arSample.updateOriginCamera(latitude, longitude, altitude, heading);
        }

        ClippingView {
            id: clippingView
            width: parent.width
            visible: false
            onClippingDistanceChanged: arSample.setClippingDistance(clippingDistance);
        }
    }

    SettingsWindow {
        anchors {
            top: parent.top
            left: parent.left
            margins: 5
        }

        // tracking options
        onStartTrackingClicked: arcGISArView.startTracking();
        onStopTrackingClicked: arcGISArView.stopTracking();
        onResetTrackingClicked: arcGISArView.resetTracking();
        onIgnoreTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Ignore;
        onInitialTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Initial;
        onContinuousTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Continuous;
        onClippingClicked: clippingView.visible = !clippingView.visible
        onCalibrationClicked: calibrationView.visible = !calibrationView.visible
        onResetCalibrationClicked: calibrationView.reset();
        onScreenToLocationClicked: arSample.screenToLocationMode = !arSample.screenToLocationMode

        // debug options
        onShowPointCloud: arSample.showPointCloud(visible);
        onShowPlanes: arSample.showPlanes(visible);

        // full scale scenes
        readonly property double emptySceneFactor: 0.0000001
        readonly property double streetsSceneFactor: 0.0000001
        readonly property double imagerySceneFactor: 0.0000001
        readonly property double fullScaleTestSceneFactor: 0.0000001

        onEmptySceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(emptySceneFactor);
            arSample.createEmptyScene();
        }
        onStreetsSceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(streetsSceneFactor);
            arSample.createStreetsScene();
        }
        onImagerySceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(imagerySceneFactor);
            arSample.createImageryScene();
        }
        onFullScaleTestSceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(fullScaleTestSceneFactor);
            arSample.createFullScaleTestScene();
        }

        // tabletop scenes
        readonly property double pointCloundSceneFactor: 0.0001
        readonly property double brestSceneFactor: 0.00001
        readonly property double berlinSceneFactor: 0.0001
        readonly property double tabletopTestSceneFactor: 0.0000001

        onBrestSceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(brestSceneFactor);
            arSample.createBrestScene();
        }
        onBerlinSceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(berlinSceneFactor);
            arSample.createBerlinScene();
        }
        onTabletopTestSceneClicked: {
            calibrationView.visible = false;
            setCalibrationFactors(tabletopTestSceneFactor);
            arSample.createTabletopTestScene();
        }

        // helper for calibration factor
        function setCalibrationFactors(factor) {
            calibrationView.latitudeFactor = factor;
            calibrationView.longitudeFactor = factor;
        }
    }
}
