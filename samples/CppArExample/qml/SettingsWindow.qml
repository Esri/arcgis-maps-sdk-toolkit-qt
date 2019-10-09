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

Item {
    id: root
    width: 50
    height: 50

    signal startTrackingClicked()
    signal stopTrackingClicked()
    signal resetTrackingClicked()
    signal calibrationClicked()
    signal screenToLocationClicked()

    signal showPointCloud(bool visible)
    signal showPlanes(bool visible)

    signal emptySceneClicked()
    signal streetsSceneClicked()
    signal imagerySceneClicked()
    signal fullScaleTestSceneClicked()

    signal pointCloudSceneClicked()
    signal yosemiteSceneClicked()
    signal borderSceneClicked()
    signal brestSceneClicked()
    signal berlinSceneClicked()
    signal tabletopTestSceneClicked()

    Image {
        id: settingsButton
        anchors.fill: parent
        source: "qrc:///Resources/settings.png"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                settings.visible = !settings.visible;
            }
        }
    }

    Rectangle {
        id: settings
        color: "lightgrey"
        anchors.top: settingsButton.bottom
        visible: false
        width: controls.width + 10
        height: controls.height + 10

        Row {
            id: controls
            anchors {
                top: parent.top
                left: parent.left
                margins: 5
            }
            spacing: 5

            Column {
                spacing: 5
                GroupBox {
                    title: "AR view"
                    Column {
                        spacing: 5
                        Button {
                            text: "Start tracking"
                            onClicked: startTrackingClicked();
                        }
                        Button {
                            text: "Stop tracking"
                            onClicked: stopTrackingClicked();
                        }
                        Button {
                            text: "Reset"
                            onClicked: resetTrackingClicked();
                        }
                        Button {
                            text: "Calibration"
                            onClicked: calibrationClicked();
                            enabled: false
                        }
                        Button {
                            text: "Screen to location"
                            onClicked: screenToLocationClicked();
                        }
                    }
                }
                GroupBox {
                    title: "Debug Mode"
                    Column {
                        spacing: 5
                        Button {
                            text: "Show point cloud"
                            onClicked: showPointCloud(true)
                        }
                        Button {
                            text: "Hide point cloud"
                            onClicked: showPointCloud(false)
                        }
                        Button {
                            text: "Show planes"
                            onClicked: showPlanes(true)
                        }
                        Button {
                            text: "Hide planes"
                            onClicked: showPlanes(false)
                        }
                    }
                }
            }

            Column {
                spacing: 5
                GroupBox {
                    title: "Full Scale Scenes"
                    Column {
                        spacing: 5
                        Button {
                            text: "Empty"
                            onClicked: emptySceneClicked();
                        }
                        Button {
                            text: "Streets"
                            onClicked: streetsSceneClicked();
                        }
                        Button {
                            text: "Imagery"
                            onClicked: imagerySceneClicked();
                        }
                        Button {
                            text: "Test"
                            onClicked: fullScaleTestSceneClicked();
                        }
                    }
                }

                GroupBox {
                    title: "Tabletop Scenes"
                    Column {
                        spacing: 5
                        Button {
                            text: "Point Cloud"
                            onClicked: pointCloudSceneClicked();
                        }
                        Button {
                            text: "Yosemite"
                            onClicked: yosemiteSceneClicked();
                        }
                        Button {
                            text: "Border"
                            onClicked: borderSceneClicked();
                        }
                        Button {
                            text: "Brest"
                            onClicked: brestSceneClicked();
                        }
                        Button {
                            text: "Berlin"
                            onClicked: berlinSceneClicked();
                        }
                        Button {
                            text: "Test"
                            onClicked: tabletopTestSceneClicked();
                        }
                    }
                }
            }
        }
    }
}
