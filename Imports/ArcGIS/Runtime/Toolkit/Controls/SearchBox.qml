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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0

Item {
    id: searchbox

    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property string searchTextInput: textInput.text
    property string featureTextInput: " "
    property bool featureTextVisibility: false

    signal search()
    signal clear()

    Rectangle {
        color: "lightgrey"
        anchors {
            fill: columnControls
            margins: -10 * displayScaleFactor
        }
        radius: 5 * displayScaleFactor
        border.color: "black"
        opacity: 0.77

        MouseArea {
            anchors.fill: parent
            onClicked: (mouse.accepted = true)
        }
    }

    Rectangle {
        id: rectangleBorder
        anchors.fill: parent
        color: "transparent"
        border {
            width: 0.5 * displayScaleFactor
            color: "black"
        }
    }

    Column {
        id: columnControls
        spacing: 10 * displayScaleFactor

        Row {
            id: rowControls
            spacing: 10 * displayScaleFactor

            //---------------------------------------------
            //Search text input field
            TextField {
                id: textInput
                width: 200 * displayScaleFactor
                height: 30 * displayScaleFactor
                font.pixelSize: 12 * displayScaleFactor
                focus: true
                text: "3575 Geary Blvd, San Francisco"

            }

            //----------------------------------------------
            //Search Button
            Image {
                id: imageSearchButton
                width: 22 * displayScaleFactor
                height: 22 * displayScaleFactor
                anchors.verticalCenter: rowControls.verticalCenter
                source: "images/search.png"

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        searchbox.search();
                    }
                }
            }

            //---------------------------------------
            //Clear Button
            Image {
                id: imageClearButton
                width: 22 * displayScaleFactor
                height: 22 * displayScaleFactor
                anchors.verticalCenter: rowControls.verticalCenter
                source: "images/remove.png"

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        textInput.text = ""
                        searchbox.clear();
                    }
                }
            }
        }

        //-------------------------------------------------------
        //Address Field (Visibility is set to false by default)
        Row {
            id: extrasText
            visible: featureTextVisibility

            Text {
                id: textFeature
                width: columnControls.width
                font {
                    pixelSize: 12 * displayScaleFactor
                }
                text: featureTextInput
                wrapMode: Text.WrapAnywhere
                elide: Text.ElideRight
                maximumLineCount: 2
            }
        }
    }
}

