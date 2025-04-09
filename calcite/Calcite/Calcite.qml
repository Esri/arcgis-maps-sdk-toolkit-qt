
/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
pragma Singleton

import QtQuick


/*!
 \qmltype Calcite
 \brief Naming conventions and colors taken directly from
    \l{https://developers.arcgis.com/calcite-design-system/foundations/colors}
    {Calcite colors and themes}.
 */
QtObject {
    // Unknown was added it to more accurately match Application.styleHints.colorScheme
    // This is an internal implementation detail.
    // https://doc.qt.io/qt-6/qt.html#ColorScheme-enum
    enum Theme {
    Unknown,
    Light,
    Dark
}

    property int theme: honorSystemTheme ? Application.styleHints.colorScheme : Calcite.Theme.Light

    property bool honorSystemTheme: true

    readonly property color offWhite: "#F8F8F8"

    readonly property color brand: theme === Calcite.Theme.Light ? "#007AC2" : "#009AF2"

    readonly property color brandHover: theme === Calcite.Theme.Light ? "#00619B" : "#007AC2"

    readonly property color brandPress: theme === Calcite.Theme.Light ? "#004874" : "#00619B"

    readonly property color background: theme === Calcite.Theme.Light ? offWhite : "#353535"

    readonly property color foreground1: theme === Calcite.Theme.Light ? "#FFFFFF" : "#2B2B2B"

    readonly property color foreground2: theme === Calcite.Theme.Light ? "#F3F3F3" : "#202020"

    readonly property color foreground3: theme === Calcite.Theme.Light ? "#EAEAEA" : "#151515"

    readonly property color text1: theme === Calcite.Theme.Light ? "#151515" : "#FFFFFF"

    readonly property color text2: theme === Calcite.Theme.Light ? "#4A4A4A" : "#BFBFBF"

    readonly property color text3: theme === Calcite.Theme.Light ? "#6A6A6A" : "#9F9F9F"

    readonly property color textInverse: theme === Calcite.Theme.Light ? "#FFFFFF" : "#151515"

    readonly property color textLink: theme === Calcite.Theme.Light ? "#00619B" : "#00A0FF"

    readonly property color border1: theme === Calcite.Theme.Light ? "#CACACA" : "#4A4A4A"

    readonly property color border2: theme === Calcite.Theme.Light ? "#DFDFDF" : "#404040"

    readonly property color border3: theme === Calcite.Theme.Light ? "#EAEAEA" : "#353535"

    readonly property color borderInput: theme === Calcite.Theme.Light ? "#949494" : "#757575"

    readonly property color info: theme === Calcite.Theme.Light ? "#00619B" : "#00A0FF"

    readonly property color success: theme === Calcite.Theme.Light ? "#35AC46" : "#36DA43"

    readonly property color warning: theme === Calcite.Theme.Light ? "#EDD317" : "#FFC900"

    readonly property color danger: theme === Calcite.Theme.Light ? "#D83020" : "#FE583E"

    readonly property color dangerHover: theme === Calcite.Theme.Light ? "#A82B1E" : "#FF0015"

    readonly property color dangerPress: theme === Calcite.Theme.Light ? "#7C1D13" : "#D90012"
}
