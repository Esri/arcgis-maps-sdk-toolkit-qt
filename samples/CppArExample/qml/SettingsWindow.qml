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
    signal ignoreTrackingClicked()
    signal initialTrackingClicked()
    signal continuousTrackingClicked()
    signal calibrationClicked()
    signal resetCalibrationClicked()

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
                            text: "Ignore tracking"
                            onClicked: ignoreTrackingClicked();
                        }
                        Button {
                            text: "Initial tracking"
                            onClicked: initialTrackingClicked();
                        }
                        Button {
                            text: "Continuous tracking"
                            onClicked: continuousTrackingClicked();
                        }
                        Button {
                            text: "Calibration"
                            onClicked: {
                                settings.visible = false;
                                calibrationClicked();
                            }
                        }
                        Button {
                            text: "Reset calibration"
                            onClicked: {
                                settings.visible = false;
                                resetCalibrationClicked();
                            }
                        }
                    }
                }
                GroupBox {
                    title: "Debug mode"
                    Column {
                        spacing: 5
                        Button {
                            text: "Show point cloud"
                            onClicked: showPointCloud(true);
                        }
                        Button {
                            text: "Hide point cloud"
                            onClicked: showPointCloud(false);
                        }
                        Button {
                            text: "Show planes"
                            onClicked: showPlanes(true);
                        }
                        Button {
                            text: "Hide planes"
                            onClicked: showPlanes(false);
                        }
                    }
                }
            }

            Column {
                spacing: 5
                GroupBox {
                    title: "Full scale scenes"
                    Column {
                        spacing: 5
                        Button {
                            text: "Empty"
                            onClicked: {
                                settings.visible = false;
                                emptySceneClicked();
                            }
                        }
                        Button {
                            text: "Streets"
                            onClicked: {
                                settings.visible = false;
                                streetsSceneClicked();
                            }
                        }
                        Button {
                            text: "Imagery"
                            onClicked: {
                                settings.visible = false;
                                imagerySceneClicked();
                            }
                        }
                        Button {
                            text: "Test"
                            onClicked: {
                                settings.visible = false;
                                fullScaleTestSceneClicked();
                            }
                        }
                    }
                }

                GroupBox {
                    title: "Tabletop scenes"
                    Column {
                        spacing: 5
                        Button {
                            text: "Point cloud"
                            onClicked: {
                                settings.visible = false;
                                pointCloudSceneClicked();
                            }
                        }
                        Button {
                            text: "Yosemite"
                            onClicked: {
                                settings.visible = false;
                                yosemiteSceneClicked();
                            }
                        }
                        Button {
                            text: "Border"
                            onClicked: {
                                settings.visible = false;
                                borderSceneClicked();
                            }
                        }
                        Button {
                            text: "Brest"
                            onClicked: {
                                settings.visible = false;
                                brestSceneClicked();
                            }
                        }
                        Button {
                            text: "Berlin"
                            onClicked: {
                                settings.visible = false;
                                berlinSceneClicked();
                            }
                        }
                        Button {
                            text: "Test"
                            onClicked: {
                                settings.visible = false;
                                tabletopTestSceneClicked();
                            }
                        }
                    }
                }
            }
        }
    }
}
