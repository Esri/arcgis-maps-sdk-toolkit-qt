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

import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISArToolkit 1.0

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "QmlArExample"

    // The origin camera is set when the scene is created. The origin camera sets to ArcGISArView is
    // this camera updated with the offset values returned by the calibration view.
    property Camera originCamera: null

    // Is true if the current scene is tabletop scene.
    property bool tabletop: false

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        tracking: true
        sceneView: sceneView
        locationDataSource: sceneLoader.item ? sceneLoader.item.locationDataSource : null
        translationFactor: sceneLoader.item ? sceneLoader.item.translationFactor : 1
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
        scene: sceneLoader.item ? sceneLoader.item.scene : null
        onMousePressed: arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen events
        opacity: calibrationView.visible ? 0.65 : 1.0
    }

    Loader {
        id: sceneLoader
        onLoaded: {
            originCamera = sceneLoader.item.originCamera;
        }
    }

    Column {
        anchors {
            right: parent.right
            left: parent.left
            margins: 10
            bottom: parent.bottom
            bottomMargin: 30
        }
        spacing: 5

        Message {
            id: waitingInitMessage
            width: parent.width
            visible: tabletop
            text: "Touch screen to place the tabletop scene..."
        }

        Message {
            id: waitingLocationMessage
            width: parent.width
            visible: arcGISArView.locationDataSource ? !arcGISArView.locationDataSource.started : false
            text: "Waiting for location..."
        }

        CalibrationView {
            id: calibrationView
            width: parent.width
            visible: false
            onTriggered: updateOriginCamera(latitude, longitude, altitude, heading);
        }
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
        onIgnoreTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Ignore;
        onInitialTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Initial;
        onContinuousTrackingClicked: arcGISArView.locationTrackingMode = ArEnums.Continuous;
        onCalibrationClicked: calibrationView.visible = !calibrationView.visible;
        onResetCalibrationClicked: calibrationView.reset();

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

        readonly property double emptySceneFactor: 0.0000001
        readonly property double streetsSceneFactor: 0.0000001
        readonly property double imagerySceneFactor: 0.0000001

        onEmptySceneClicked: changeScene("qrc:/qml/scenes/EmptyScene.qml",emptySceneFactor, true);
        onStreetsSceneClicked: changeScene("qrc:/qml/scenes/StreetsScene.qml", streetsSceneFactor, true);
        onImagerySceneClicked: changeScene("qrc:/qml/scenes/ImageryScene.qml", imagerySceneFactor, true);

        readonly property double pointCloundSceneFactor: 0.0001
        readonly property double yosemiteSceneFactor: 0.0001
        readonly property double borderSceneFactor: 0.0001
        readonly property double brestSceneFactor: 0.00001
        readonly property double berlinSceneFactor: 0.0001
        readonly property double tabletopTestSceneFactor: 0.0000001

        onPointCloudSceneClicked: changeScene("qrc:/qml/scenes/PointCloudScene.qml", pointCloundSceneFactor);
        onYosemiteSceneClicked: changeScene("qrc:/qml/scenes/YosemiteScene.qml", yosemiteSceneFactor);
        onBorderSceneClicked: changeScene("qrc:/qml/scenes/BorderScene.qml", borderSceneFactor);
        onBrestSceneClicked: changeScene("qrc:/qml/scenes/BrestScene.qml", brestSceneFactor);
        onBerlinSceneClicked: changeScene("qrc:/qml/scenes/BerlinScene.qml", berlinSceneFactor);
        onTabletopTestSceneClicked: changeScene("qrc:/qml/scenes/TabletopTestScene.qml",
                                                tabletopTestSceneFactor, false, sceneLoader.item.graphicsOverlay);

        function changeScene(sceneSource, factor, isTabletop = false, overlay = null) {
            tabletop = isTabletop;

            // Set or clear the graphics overlay
            if (overlay)
                sceneView.graphicsOverlays.append(overlay);
            else
                sceneView.graphicsOverlays.clear()

            // Stop tracking
            arcGISArView.stopTracking();

            // set the new scene
            sceneView.graphicsOverlays.clear();
            sceneLoader.source = sceneSource;
            arcGISArView.originCamera = originCamera;

            // Reset and start tracking
            if (arcGISArView.tracking)
                arcGISArView.resetTracking();
            else
                arcGISArView.startTracking();

            // Update calibration factor
            calibrationView.latitudeFactor = factor;
            calibrationView.longitudeFactor = factor;
            calibrationView.reset();
        }
    }

    // Update the origin camera using the values sent by the calibration view.
    // The originCamera is set when the scene is created.
    // The final origin camera sent to ArcGISArView is the sum of originCamera
    // and the offset values returned by the calibration view.
    function updateOriginCamera(latitude, longitude, altitude, heading) {
        if (!originCamera) {
            const newLocation = ArcGISRuntimeEnvironment.createObject(
                                  "Point", { y: latitude, x: longitude, z: altitude });
            const newCamera = ArcGISRuntimeEnvironment.createObject(
                                "Camera", { location: newLocation, heading: heading, pitch: 0.0, roll: 0.0 });

            // Set the origin camera.
            arcGISArView.originCamera = newCamera;
        }
        else {
            // Calculate the new parameters
            const oldLocation = originCamera.location;
            const newLatitude = oldLocation.y + latitude;
            const newLongitude = oldLocation.x + longitude;
            const newAltitude = oldLocation.z + altitude;
            const newHeading = originCamera.heading + heading;

            // Create a new origin camera
            const newLocation = ArcGISRuntimeEnvironment.createObject(
                                  "Point", { y: newLatitude, x: newLongitude, z: newAltitude });
            const newCamera = ArcGISRuntimeEnvironment.createObject(
                                "Camera", { location: newLocation, heading: newHeading,
                                    pitch: originCamera.pitch, roll: originCamera.roll });

            // Set the origin camera.
            arcGISArView.originCamera = newCamera;
        }
    }
}
