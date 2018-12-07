/*******************************************************************************
 *  Copyright 2012-2018 Esri
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
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Esri.ArcGISRuntime.Toolkit.CppApi 100.3

/*!
    \qmltype ArcGISCompass
    \ingroup ToolArcGISCompass
    \inqmlmodule Esri.ArcGISRuntime.Toolkit.Controls
    \since Esri.ArcGISRutime 100.2
    \brief The user interface for the ArcGIS Compass tool.
    \sa {ArcGIS Compass Tool}
*/

Item {
    /*!
      \qmlproperty real scaleFactor
      \brief The scale factor used for sizing UI elements.
      \internal

      Pixel density and screen resolution varies greatly between different
      devices and operating systems. This property allows your app to specify
      the width and height of UI elements so that the sizes appear similar
      (relative to screen size) across devices. Here is an example of how to
      use this property.

      \code
         ArcGISCompass {
            width: 100 * scaleFactor
            height: 100 * scaleFactor
            ...
         }
      \endcode
     */
    property real scaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" || Qt.platform.os === "linux" ? 96 : 72)

    /*!
      \qmlproperty bool autoHideCompass
      \brief Whether the compass should be hidden when north is at the top
      of the display.

      The default value is \c true.
     */
    property bool autoHideCompass: true

    // ArcGISCompassController must be registered as a QML type in C++ code
    ArcGISCompassController {
        id: controller
        objectName: "arcGISCompassController"
        autoHide: autoHideCompass
    }

    height: 32 * scaleFactor
    width: 32 * scaleFactor
    opacity: 0.85

    Image {
        anchors.fill: parent
        source: "images/compass.png"
        fillMode: Image.PreserveAspectFit
        rotation: -1 * controller.heading

        // When zooming in a Scene, the heading will adjust by a miniscule amount. Using the < comparison rather than === prevents flickering while zooming
        visible: controller.autoHide && (controller.heading < 1e-05 || controller.heading === 360) ? false : true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                controller.heading = 0;
            }
        }
    }
}
