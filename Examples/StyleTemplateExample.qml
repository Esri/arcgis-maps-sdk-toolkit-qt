/*******************************************************************************
 * Copyright 2012-2014 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtPositioning 5.2
import ArcGIS.Runtime 10.3
import QtQuick.Window 2.2
import ArcGIS.Runtime.Toolkit.Controls 1.0

Rectangle {
    width: 500
    height: 400
    property real size: 40
    property int panDistance : 1000000
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    QtObject {
        id: internal
        property real _size: size * displayScaleFactor
    }

    Map {
        id: map
        anchors.fill: parent
        wrapAroundEnabled: true
        mapPanningByMagnifierEnabled: true
        magnifierOnPressAndHoldEnabled: true
        zoomByPinchingEnabled: true

        positionDisplay {
            positionSource: PositionSource {
            }
        }

        ArcGISTiledMapServiceLayer {
            url: "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer"
        }

        StyleToolbar {
            anchors {
                top: navi.bottom
                right: parent.right
                rightMargin: 10
            }
            orientation: "landscape"
            columns: orientation === "portrait" ? 1 : 5
            rows: orientation === "portrait" ? 5 : 1
            rowSpacing: orientation === "landscape" ? 0 : 1 * displayScaleFactor

            //--------------------------------------------------------------------------
            // Rotate Counter Clockwise Button
            StyleButton {
                id: buttonRotateCounterClockwise
                iconSource: "images/rotate_clockwise.png"
                width: internal._size
                height: width

                onClicked: {
                    fader.start();
                    map.mapRotation -= 22.5;
                }
            }

            //--------------------------------------------------------------------------
            // Rotate Clockwise Button
            StyleButton {
                id: buttonRotateClockwise
                iconSource: "images/rotate_counter_clockwise.png"
                width: internal._size
                height: width

                onClicked: {
                    fader.start();
                    map.mapRotation += 22.5;
                }
            }

            //--------------------------------------------------------------------------
            // Info Button
            StyleButton {
                id: infoButton
                iconSource: "images/info.png"
                width: internal._size
                height: width

                onClicked: {
                    fader.start();
                    Qt.openUrlExternally("https://developers.arcgis.com/qt")
                }
            }
        }

        //--------------------------------------------------------------------------
        // Pan North Button
        StyleButton {
            id: moveNorth
            iconSource:"images/arrow.png"
            anchors {
                top: parent.top
                left: parent.left
                topMargin: 20
                leftMargin: 15
            }

            onHoveredChanged: {
                if (moveNorth.hovered)
                    moveNorth.fader.stop();
                else
                    moveNorth.fader.start();
            }

            onClicked: {
                var extent = map.extent;
                extent.yMin += panDistance
                map.panTo(extent)
            }
        }

        //--------------------------------------------------------------------------
        // Pan South Button
        StyleButton {
            id: moveSouth
            iconSource:"images/arrow.png"
            rotation: 180
            anchors {
                top: moveNorth.bottom
                left: parent.left
                topMargin: 10
                leftMargin: 15
            }

            onHoveredChanged: {
                if (moveSouth.hovered)
                    moveSouth.fader.stop();
                else
                    moveSouth.fader.start();
            }

            onClicked: {
                var extent = map.extent;
                extent.yMin -= panDistance;
                map.panTo(extent)
            }
        }

        //--------------------------------------------------------------------------
        // Pan West Button
        StyleButton {
            id: moveWest
            iconSource:"images/arrow.png"
            rotation: 270
            anchors {
                top: moveSouth.bottom
                left: parent.left
                topMargin: 10
                leftMargin: 15
            }

            onHoveredChanged: {
                if (moveWest.hovered)
                    moveWest.fader.stop();
                else
                    moveWest.fader.start();
            }

            onClicked: {
                var extent = map.extent;
                extent.xMin -= panDistance;
                map.panTo(extent)
            }
        }

        //--------------------------------------------------------------------------
        // Pan East Button
        StyleButton {
            id: moveEast
            iconSource:"images/arrow.png"
            rotation: 90
            anchors {
                top: moveWest.bottom
                left: parent.left
                topMargin: 10
                leftMargin: 15
            }

            onHoveredChanged: {
                if (moveEast.hovered)
                    moveEast.fader.stop();
                else
                    moveEast.fader.start();
            }

            onClicked: {
                var extent = map.extent;
                extent.xMin += panDistance;
                map.panTo(extent)
            }
        }

        NavigationToolbar {
            id: navi
            anchors {
                top: parent.top
                right: parent.right
                rightMargin: 10
                topMargin: 25
            }
        }
    }
}
