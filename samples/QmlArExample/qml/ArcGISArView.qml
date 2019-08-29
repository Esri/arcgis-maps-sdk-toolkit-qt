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
import QtQuick.Window 2.12
import Esri.ArcGISRuntime 100.6
import Esri.ArcGISArToolkit 1.0

ArcGISArViewInternal {
    id: root

    // This QML component is used to create the QML's TMCC object from the C++.
    transformationMatrixCameraController: TransformationMatrixCameraController {
    }

    // todo doc
    onTranslationFactorChanged: transformationMatrixCameraController.translationFactor = translationFactor;

    // todo doc
    onRenderFrame: sceneView.renderFrame();

    // it's not possible to create the TransformationMatrix object diretly in C++. This function
    // is used to create the TM object and assign it to the TMCC.
    onTransformationMatrixChanged: {
        var matrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);
        transformationMatrixCameraController.transformationMatrix = matrix;
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
        var location = ArcGISRuntimeEnvironment.createObject("Point", { x: latitude, y: longitude, z: altitude });
        console.log("====>>>> location", location);
        console.log("====>>>> tmcc.originCamera", tmcc.originCamera);

        if (tmcc.originCamera === null) {
            // create a new origin camera
            var camera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: location, heading: 0.0, pitch: 90.0, roll: 0.0 });
            console.log("====>>>> camera", camera);
            tmcc.originCamera = camera;
        } else {
            // update the origin camera
            var oldCamera = tmcc.originCamera;
            var newCamera = ArcGISRuntimeEnvironment.createObject(
                        "Camera", { location: location, heading: oldCamera.heading, pitch: oldCamera.pitch, roll: oldCamera.roll });
            console.log("====>>>> oldCamera, newCamera", oldCamera, newCamera);
            tmcc.originCamera = newCamera;
        }

        // todo: Reset the camera controller's transformationMatrix to its initial state, the Idenity matrix.
        //  cameraController.transformationMatrix = .identity
    }

    // Calculate the location using the hit result returned by the screenToLocation function.
    function screenToLocation(currentViewpointCamera,
                              quaternionX, quaternionY, quaternionZ, quaternionW,
                              translationX, translationY, translationZ)
    {
        var hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        var currentViewpointMatrix = currentViewpointCamera.transformationMatrix;
        var matrix = currentViewpointMatrix.addTransformation(hitMatrix);
        var camera = ArcGISRuntimeEnvironment.createObject("Camera", { transformationMatrix: matrix });
        return camera.location;
    }
}
