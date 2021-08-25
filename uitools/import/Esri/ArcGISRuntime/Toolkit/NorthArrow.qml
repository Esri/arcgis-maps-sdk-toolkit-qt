/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.12

/*!
   \qmltype NorthArrow
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \ingroup ArcGISQtToolkitUiQmlViews
   \since Esri.ArcGISRuntime 100.10
   \brief The NorthArrow displays a compass overlaid on the GeoView, with the
   compass heading matching the current rotation of the MapView, or Camera
   heading of the SceneView. 
   Double clicking on the NorthArrow triggers the heading of the connected
   GeoView to be orientainted to 0.
 */

Item {
    id: northArrow

    /*!
       \qmlproperty GeoView geoView
       \brief The GeoView for this tool. Should be a SceneView or a MapView.
     */
    property var geoView;

    /*!
       \qmlproperty bool autoHide
       \brief When autohide is true, this view is only visible when the heading
       is a non-zero value.
     */
    property bool autoHide: false

    /*!
      \qmlproperty NorthArrowController controller
      \brief the Controller handles connections writing/reading to the GeoView.
    */
    property var controller: NorthArrowController { }

    Binding {
        target: controller
        property: "geoView"
        value: northArrow.geoView
    }

    Binding {
        target: compassImage
        property: "rotation"
        value: isNaN(controller.heading) ? 0 : -controller.heading
    }

    Image {
        id: compassImage
        anchors.fill: parent
        source: "images/compass.png"
        fillMode: Image.PreserveAspectFit
        visible: autoHide && (controller.heading  % 360 < 1e-05) ? false : true
    }

    MouseArea {
        anchors.fill: parent
        onDoubleClicked: {
            if (geoView)
                controller.setHeading(0); // Rotate back to north.
        }
    }
}
