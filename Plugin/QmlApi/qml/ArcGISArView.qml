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

    // todo doc
    onTranslationFactorChanged: tmcc.translationFactor = translationFactor;

    // todo doc
    onRenderFrame: root.sceneView.renderFrame();

    // doc todo
    property TransformationMatrix identityTransformationMatrix: TransformationMatrix.createIdentityMatrix()
    property TransformationMatrix initialTransformationMatrix: identityTransformationMatrix

    onInitialTransformationChanged: {
        // Set the `initialTransformation` as the TransformationMatrix.identity - hit test matrix.
        const hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        initialTransformationMatrix = identityTransformationMatrix.subtractTransformation(hitMatrix);
    }

    // it's not possible to create the TransformationMatrix object directly in C++. This function
    // is used to create the TM object and assign it to the TMCC.
    onTransformationMatrixChanged: {
        const matrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        const finalMatrix = initialTransformationMatrix.addTransformation(matrix);
        tmcc.transformationMatrix = finalMatrix;
    }

    // it's not possible to call setFieldOfViewFromLensIntrinsics directly from the C++ code, due to
    // the orientation device enumeration. This function is used to converts the orientation (int) to
    // deviceOrientation (enum).
    onFieldOfViewChanged: {
        var deviceOrientation;
        switch(Screen.orientation) {
        case Qt.PortraitOrientation:
            deviceOrientation = Enums.DeviceOrientationPortrait;
            break;
        case Qt.LandscapeOrientation:
            deviceOrientation = Enums.DeviceOrientationLandscapeLeft;
            break;
        case Qt.InvertedPortraitOrientation:
            deviceOrientation = Enums.DeviceOrientationReversePortrait;
            break;
        case Qt.InvertedLandscapeOrientation:
            deviceOrientation = Enums.DeviceOrientationPortrait;
            break;
        default:
            deviceOrientation = Enums.DeviceOrientationPortrait;
            break;
        }

        sceneView.setFieldOfViewFromLensIntrinsics(xFocalLength, yFocalLength,
                                                   xPrincipal, yPrincipal,
                                                   xImageSize, yImageSize,
                                                   deviceOrientation);
    }

    // location update
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

    // Calculate the location using the hit result returned by the screenToLocation function.
    function screenToLocation(currentViewpointCamera,
                              quaternionX, quaternionY, quaternionZ, quaternionW,
                              translationX, translationY, translationZ)
    {
        const hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        const currentViewpointMatrix = currentViewpointCamera.transformationMatrix;
        const matrix = currentViewpointMatrix.addTransformation(hitMatrix);
        const camera = ArcGISRuntimeEnvironment.createObject("Camera", { transformationMatrix: matrix });
        return camera.location;
    }

    // Resets the device tracking and related properties.
    onResetTracking: {
        const camera = ArcGISRuntimeEnvironment.createObject("Camera");
        tmcc.originCamera = camera;

        initialTransformationMatrix = identityTransformationMatrix;

        tmcc.transformationMatrix = initialTransformationMatrix;
    }
}
