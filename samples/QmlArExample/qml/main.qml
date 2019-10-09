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

    property bool screenToLocationMode: false

    ArcGISArView {
        id: arcGISArView
        anchors.fill: parent
        tracking: true
        sceneView: sceneView
        originCamera: sceneLoader.item ? sceneLoader.item.originCamera : null
        locationDataSource: sceneLoader.item ? sceneLoader.item.locationDataSource : null
        translationFactor: sceneLoader.item ? sceneLoader.item.translationFactor : 1
    }

    SceneView {
        id: sceneView
        anchors.fill: parent
        scene: sceneLoader.item ? sceneLoader.item.scene : null

        // If m_screenToLocationMode is true, create and place a 3D sphere in the scene. Otherwise set the
        // initial transformation based on the screen position.
        onMousePressed: {
            // If "screenToLocation" mode is enabled.
            if (screenToLocationMode)
            {
                // Get the real world location for screen point from AR view.
                const point = arcGISArView.screenToLocation(mouse.x, mouse.y);
                if (!point)
                    return;

                // Get or create graphic overlay
                var graphicsOverlay = graphicsOverlays.get(0);
                if (!graphicsOverlay)
                {
                  graphicsOverlay = ArcGISRuntimeEnvironment.createObject("GraphicsOverlay");
                  graphicsOverlays.append(graphicsOverlay);
                }

                // Create and place a graphic at the real world location.
                const sphereParams = {
                    style: Enums.SimpleMarkerSceneSymbolStyleSphere, color: "yellow",
                    width: 0.2520, height: 0.25, depth: 0.25,
                    anchorPosition: Enums.SceneSymbolAnchorPositionBottom };
                const sphere = ArcGISRuntimeEnvironment.createObject("SimpleMarkerSceneSymbol", sphereParams);

                const sphereGraphicParams = { geometry: point, symbol: sphere };
                const sphereGraphic = ArcGISRuntimeEnvironment.createObject("Graphic", sphereGraphicParams);

                graphicsOverlay.graphics.append(sphereGraphic);
            }
            else
            {
                // Set the initial transformation corresponding to the screen point.
                arcGISArView.setInitialTransformation(mouse.x, mouse.y); // for touch screen events
            }
        }
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
        onScreenToLocationClicked: screenToLocationMode = !screenToLocationMode;

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
