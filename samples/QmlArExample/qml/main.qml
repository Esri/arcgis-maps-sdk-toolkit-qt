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
    property Camera originCamera: sceneLoader.item ? sceneLoader.item.originCamera : null

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        sceneView: sceneView
        locationDataSource: sceneLoader.item ? sceneLoader.item.locationDataSource : null
        translationFactor: sceneLoader.item ? sceneLoader.item.translationFactor : 1

        // By default, start the tracking.
        Component.onCompleted: startTracking();
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
        scene: sceneLoader.item ? sceneLoader.item.scene : null
        onMousePressed: arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen events
    }

    Loader {
        id: sceneLoader
    }

    CalibrationView {
        id: calibrationView
        anchors.fill: parent
        visible: false
        onTriggered: updateOriginCamera(latitude, longitude, altitude, heading);
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
        onCalibrationClicked: calibrationView.visible = ! calibrationView.visible
        onResetCalibrationClicked: updateOriginCamera(0, 0, 0, 0);

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
        }
    }

    function updateOriginCamera(latitude, longitude, altitude, heading) {
        // Calculate the new parameters
        const oldLocation = originCamera.location;
        const x = oldLocation ? oldLocation.x + longitude : longitude;
        const y = oldLocation ? oldLocation.y + latitude : latitude;
        const z = oldLocation ? oldLocation.z + altitude : altitude;
        const h = originCamera.heading + heading;

        // Create a new origin camera
        const location = ArcGISRuntimeEnvironment.createObject("Point", { x: x , y: y , z: z });
        const newCamera = ArcGISRuntimeEnvironment.createObject(
                         "Camera", { location: location, heading: h, pitch: originCamera.pitch, roll: originCamera.roll });

        // Set the origin camera.
        arcGISArView.originCamera = newCamera;
    }
}
