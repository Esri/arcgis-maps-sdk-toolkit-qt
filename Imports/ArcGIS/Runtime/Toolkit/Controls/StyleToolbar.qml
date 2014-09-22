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
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import ArcGIS.Runtime 10.3

GridLayout {
    id: templateToolbar

    property Map map: null
    property real size: 40
    property color color: "#4C4C4C"
    property color disabledColor: "#E5E6E7"
    property color hoveredColor: "#E1F0FB"
    property color pressedColor: "#90CDF2"
    property color backgroundColor: "#F7F8F8"
    property color focusBorderColor: "#AADBFA"
    property color borderColor: "#CBCBCB"
    property string orientation: "portrait"
    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)

    // Default orientation Properties which can be overwritten at
    // the time of defining the StyleToolbar in your qml file.
    columns: orientation === "portrait" ? 1 : 4
    rows: orientation === "portrait" ? 4 : 1
    rowSpacing: orientation === "landscape" ? 0 : 1 * displayScaleFactor
    columnSpacing: orientation === "portrait" ? 0 : 1 * displayScaleFactor

}
