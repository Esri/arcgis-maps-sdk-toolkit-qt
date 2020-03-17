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
import QtQml 2.12

/*!
 * \qmltype NorthArrowController
 * \inqmlmodule Esri.ArcGISRuntime.Toolkit
 * \since Esri.ArcGISRutime 100.8
 * \brief In MVC architecture, this is the controller for the corresponding
 * NorthArrow view.
 * 
 * This controller calculates the current heading from a GeoView, and allows
 * the NorthArrow to apply a given heading to the GeoView.
 */
QtObject {
    id: controller

    /*!
    * \qmlproperty double heading
    * \brief The calculated heading of this controller.
    */
    readonly property alias heading: internal.heading;

   /*!
    * \brief The GeoView object this Controller uses.
    */
    property var geoView;

    /*!
     * \brief Set the heading by rotating the MapView or SceneView camera to the
     * given heading.
     */
    function setHeading(newHeading) {
        if (geoView === null)
            return;

        if (geoView.mapRotation !== undefined) {
            // This is a MapView.
            geoView.setViewpointRotation(newHeading);
        }
        else if (geoView.currentViewpointCamera !== undefined) {
            // This is a SceneView.
            let camera = geoView.currentViewpointCamera;
            let newCamera = camera.rotateTo(newHeading, camera.pitch, camera.roll);
            geoView.setViewpointCamera(newCamera);
        }
    }

    // Keep the heading read only for external consumers.
    property QtObject internal: QtObject {
        id: internal
        property double heading: NaN;

        property Binding mapBinding: Binding {
            when: geoView !== null && geoView.mapRotation !== undefined
            target: internal
            property: "heading"
            value: geoView.mapRotation
        }

        property Binding sceneBinding: Binding {
            when: geoView !== null && geoView.currentViewpointCamera !== undefined
            target: internal
            property: "heading"
            value: geoView.currentViewpointCamera ? geoView.currentViewpointCamera.heading : NaN
        }

        property Binding nullBinding: Binding {
            when: geoView === null
            target: internal
            property: "heading"
            value: NaN
        }
    }
}
