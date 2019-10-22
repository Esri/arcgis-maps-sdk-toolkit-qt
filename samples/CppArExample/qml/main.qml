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
            visible: arSample.tabletopMode && arSample.waitingInitialization
            text: "Touch screen to place the tabletop scene..."
        }

        Message {
            id: waitingLocationMessage
            width: parent.width
            visible: !arSample.tabletopMode && arSample.waitingInitialization
            text: "Waiting for location..."
        }

        CalibrationView {
            id: calibrationView
            width: parent.width
            visible: false
            onTriggered: arSample.updateOriginCamera(latitude, longitude, altitude, heading);
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
            setCalibrationFactors(emptySceneFactor);
            arSample.createEmptyScene();
        }
        onStreetsSceneClicked: {
            setCalibrationFactors(streetsSceneFactor);
            arSample.createStreetsScene();
        }
        onImagerySceneClicked: {
            setCalibrationFactors(imagerySceneFactor);
            arSample.createImageryScene();
        }
        onFullScaleTestSceneClicked: {
            setCalibrationFactors(fullScaleTestSceneFactor);
            arSample.createFullScaleTestScene();
        }

        // tabletop scenes
        readonly property double pointCloundSceneFactor: 0.0001
        readonly property double yosemiteSceneFactor: 0.0001
        readonly property double borderSceneFactor: 0.0001
        readonly property double brestSceneFactor: 0.00001
        readonly property double berlinSceneFactor: 0.0001
        readonly property double tabletopTestSceneFactor: 0.0000001

        onYosemiteSceneClicked: {
            setCalibrationFactors(yosemiteSceneFactor);
            arSample.createYosemiteScene();
        }
        onBorderSceneClicked: {
            setCalibrationFactors(borderSceneFactor);
            arSample.createBorderScene();
        }
        onBrestSceneClicked: {
            setCalibrationFactors(brestSceneFactor);
            arSample.createBrestScene();
        }
        onBerlinSceneClicked: {
            setCalibrationFactors(berlinSceneFactor);
            arSample.createBerlinScene();
        }
        onTabletopTestSceneClicked: {
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
