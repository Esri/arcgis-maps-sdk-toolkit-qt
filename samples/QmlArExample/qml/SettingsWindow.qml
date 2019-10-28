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

    property alias buttonWidth: referenceButton.width

    signal startTrackingClicked()
    signal stopTrackingClicked()
    signal resetTrackingClicked()
    signal ignoreTrackingClicked()
    signal initialTrackingClicked()
    signal continuousTrackingClicked()
    signal calibrationClicked()
    signal resetCalibrationClicked()
    signal screenToLocationClicked()

    signal showPointCloud(bool visible)
    signal showPlanes(bool visible)

    signal emptySceneClicked()
    signal streetsSceneClicked()
    signal imagerySceneClicked()
    signal fullScaleTestSceneClicked()

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
                            width: buttonWidth
                            text: "Start tracking"
                            onClicked: startTrackingClicked();
                        }
                        Button {
                            width: buttonWidth
                            text: "Stop tracking"
                            onClicked: stopTrackingClicked();
                        }
                        Button {
                            width: buttonWidth
                            text: "Reset"
                            onClicked: resetTrackingClicked();
                        }
                        Button {
                            width: buttonWidth
                            text: "Ignore tracking"
                            onClicked: ignoreTrackingClicked();
                        }
                        Button {
                            width: buttonWidth
                            text: "Initial tracking"
                            onClicked: initialTrackingClicked();
                        }
                        Button {
                            id: referenceButton
                            text: "Continuous tracking"
                            onClicked: continuousTrackingClicked();
                        }
                        Button {
                            width: buttonWidth
                            text: "Calibration"
                            onClicked: {
                                settings.visible = false;
                                calibrationClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Reset calibration"
                            onClicked: {
                                settings.visible = false;
                                resetCalibrationClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Screen to location"
                            onClicked: {
                                settings.visible = false;
                                screenToLocationClicked();
                            }
                        }
                    }
                }
                GroupBox {
                    title: "Debug mode"
                    Column {
                        spacing: 5
                        Button {
                            width: buttonWidth
                            text: "Show point cloud"
                            onClicked: showPointCloud(true);
                        }
                        Button {
                            width: buttonWidth
                            text: "Hide point cloud"
                            onClicked: showPointCloud(false);
                        }
                        Button {
                            width: buttonWidth
                            text: "Show planes"
                            onClicked: showPlanes(true);
                        }
                        Button {
                            width: buttonWidth
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
                            width: buttonWidth
                            text: "Empty"
                            onClicked: {
                                settings.visible = false;
                                emptySceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Streets"
                            onClicked: {
                                settings.visible = false;
                                streetsSceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Imagery"
                            onClicked: {
                                settings.visible = false;
                                imagerySceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
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
                            width: buttonWidth
                            text: "Yosemite"
                            onClicked: {
                                settings.visible = false;
                                yosemiteSceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Border"
                            onClicked: {
                                settings.visible = false;
                                borderSceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Brest"
                            onClicked: {
                                settings.visible = false;
                                brestSceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
                            text: "Berlin"
                            onClicked: {
                                settings.visible = false;
                                berlinSceneClicked();
                            }
                        }
                        Button {
                            width: buttonWidth
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
