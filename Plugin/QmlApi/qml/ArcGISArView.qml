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
import QtQuick.Window 2.12
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISArToolkit 1.0

ArcGISArViewInternal {
    id: root

    // This QML component is used to create the QML's TMCC object from the C++.
    transformationMatrixCameraController: TransformationMatrixCameraController {
        id: tmcc
    }

    // Update the translation factor.
    onTranslationFactorChanged: tmcc.translationFactor = translationFactor;

    // Render the scene.
    onRenderFrame: root.sceneView.renderFrame();

    // Update the initial transformation, using the hit matrix.
    property TransformationMatrix initialTransformationMatrix: null

    onInitialTransformationChanged: {
        // Set the `initialTransformation` as the TransformationMatrix.identity - hit test matrix.
        const hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        const identityTransformationMatrix = TransformationMatrix.createIdentityMatrix();
        initialTransformationMatrix = identityTransformationMatrix.subtractTransformation(hitMatrix);
    }

    // It's not possible to create the TransformationMatrix object directly in C++. This function
    // is used to create the TM object and assign it to the TMCC.
    onTransformationMatrixChanged: {
        const matrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        if (!initialTransformationMatrix)
            initialTransformationMatrix = TransformationMatrix.createIdentityMatrix();

        const finalMatrix = initialTransformationMatrix.addTransformation(matrix);
        tmcc.transformationMatrix = finalMatrix;
    }

    // Cast from Qt's screen orientation to ArcGIS Runtime's screen orientation.
    function toDeviceOrientation(orientation) {
        switch(Screen.orientation) {
        case Qt.PortraitOrientation:
            return Enums.DeviceOrientationPortrait;
        case Qt.LandscapeOrientation:
            return Enums.DeviceOrientationLandscapeLeft;
        case Qt.InvertedPortraitOrientation:
            return Enums.DeviceOrientationReversePortrait;
        case Qt.InvertedLandscapeOrientation:
            return Enums.DeviceOrientationPortrait;
        default:
            return Enums.DeviceOrientationPortrait;
        }
    }

    // It's not possible to call setFieldOfViewFromLensIntrinsics directly from the C++ code, due to
    // the orientation device enumeration. This function is used to converts the orientation (int) to
    // deviceOrientation (enum).
    onFieldOfViewChanged: {
        const deviceOrientation = toDeviceOrientation(Screen.orientation);
        sceneView.setFieldOfViewFromLensIntrinsics(xFocalLength, yFocalLength,
                                                   xPrincipal, yPrincipal,
                                                   xImageSize, yImageSize,
                                                   deviceOrientation);
    }

    // Update the location.
    onLocationChanged: {
        const location = ArcGISRuntimeEnvironment.createObject("Point", { y: latitude, x: longitude, z: altitude });
        if (tmcc.originCamera === null) {
            // create a new origin camera
            const camera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: location, heading: 0.0, pitch: 90.0, roll: 0.0 });
            tmcc.originCamera = camera;
        } else {
            // update the origin camera
            const oldCamera = tmcc.originCamera;
            const newCamera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: location, heading: oldCamera.heading, pitch: 90.0, roll: 0.0 });
            tmcc.originCamera = newCamera;
        }
    }

    // Update the heading.
    onHeadingChanged: {
        if (tmcc.originCamera === null) {
            // create a new origin camera
            const location = ArcGISRuntimeEnvironment.createObject("Point", { x: 0.0, y: 0.0, z: 600.0 });
            const camera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: location, heading: heading, pitch: 90.0, roll: 0.0 });
            tmcc.originCamera = camera;
        } else {
            // update the origin camera
            const oldCamera = tmcc.originCamera;
            const newCamera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: oldCamera.location, heading: heading, pitch: 90.0, roll: 0.0 });
            tmcc.originCamera = newCamera;
        }
    }

    // Resets the device tracking and related properties.
    onResetTracking: {
        const camera = ArcGISRuntimeEnvironment.createObject("Camera");
        tmcc.originCamera = camera;

        initialTransformationMatrix = TransformationMatrix.createIdentityMatrix();
        tmcc.transformationMatrix = TransformationMatrix.createIdentityMatrix();
    }

    /*!
      \brief Gets the location in the real world space corresponding to the screen point \a screenPoint.
     */
    function screenToLocation(x, y) {
        const hitTest = root.screenToLocation(x, y);

        const hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                            hitTest[0], hitTest[1], hitTest[2], hitTest[3], // quaternionX, quaternionY, quaternionZ, quaternionW
                            hitTest[4], hitTest[5], hitTest[6]); // translationX, translationY, translationZ

        const currentViewpointMatrix = currentViewpointCamera.transformationMatrix;
        const matrix = currentViewpointMatrix.addTransformation(hitMatrix);
        const camera = ArcGISRuntimeEnvironment.createObject("Camera", { transformationMatrix: matrix });
        return camera.location;
    }
}
