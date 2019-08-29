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
    signal pointCloudSceneClicked()
    signal yosemiteSceneClicked()
    signal borderSceneClicked()
    signal brestSceneClicked()
    signal berlinSceneClicked()

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

            GroupBox {
                title: "Scenes"
                Column {
                    spacing: 5
                    Button {
                        text: "Empty - Full Scale"
                        onClicked: emptySceneClicked();
                    }
                    Button {
                        text: "Streets - Full Scale"
                        onClicked: streetsSceneClicked();
                    }
                    Button {
                        text: "Imagery - Full Scale"
                        onClicked: imagerySceneClicked();
                    }
                    Button {
                        text: "Point Cloud - Tabletop"
                        onClicked: pointCloudSceneClicked();
                    }
                    Button {
                        text: "Yosemite - Tabletop"
                        onClicked: yosemiteSceneClicked();
                    }
                    Button {
                        text: "US Mexico Border - Tabletop"
                        onClicked: borderSceneClicked();
                    }
                    Button {
                        text: "Brest - Tabletop"
                        onClicked: brestSceneClicked();
                    }
                    Button {
                        text: "Berlin - Tabletop"
                        onClicked: berlinSceneClicked();
                    }
                }
            }
        }
    }
}
