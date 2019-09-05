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

Item {
    id: root
    width: 50
    height: 50

    property bool tracking: false
    signal startTrackingClicked()
    signal stopTrackingClicked()
    signal resetTrackingClicked()
    signal calibrationClicked()
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

            GroupBox {
                title: "AR view"
                Column {
                    spacing: 5
                    Label {
                        text: "Tracking status: " + tracking ? "on" : "off"
                    }
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
