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
import Esri.ArcGISRuntime 100.6

QtObject {
    id: root

    // This QML component is used to create the QML's TMCC object from the C++.
    TransformationMatrixCameraController {
        id: tmcc
    }

    //
    // it's not possible to create the TransformationMatrix object diretly in C++. This function
    // is used to create the TM object and assign it to the TMCC.
    function createAndSetTransformationMatrix(quaternionX, quaternionY, quaternionZ, quaternionW,
                                              translationX, translationY, translationZ)
    {
        var matrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);
        root.transformationMatrix = matrix;
    }

    // it's not possible to call setFieldOfViewFromLensIntrinsics directly from the C++ code, due to
    // the orientation device enumeration. This function is used to converts the orientation (int) to
    // deviceOrientation (enum).
    function setFieldOfViewFromLensIntrinsics(sceneView,
                                              xFocalLength, yFocalLength,
                                              xPrincipal, yPrincipal,
                                              xImageSize, yImageSize,
                                              orientation)
    {
        var deviceOrientation;
        switch(orientation) {
          case 0:
            deviceOrientation = Enums.DeviceOrientationPortrait;
            break;
          case 1:
            deviceOrientation = Enums.DeviceOrientationLandscapeLeft;
            break;
          case 2:
            deviceOrientation = Enums.DeviceOrientationReversePortrait;
            break;
          case 3:
            deviceOrientation = Enums.DeviceOrientationPortrait;
            break;
          default:
            deviceOrientation = Enums.DeviceOrientationLandscapeRight;
            break;
        }

        sceneView.setFieldOfViewFromLensIntrinsics(xFocalLength, yFocalLength,
                                                   xPrincipal, yPrincipal,
                                                   xImageSize, yImageSize,
                                                   deviceOrientation);
    }

    // Calculate the location using the hit result returned by the screenToLocation function.
    function screenToLocation(currentViewpointCamera,
                              quaternionX, quaternionY, quaternionZ, quaternionW,
                              translationX, translationY, translationZ) {

        var hitMatrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);

        var currentViewpointMatrix = currentViewpointCamera.transformationMatrix;
        var matrix = currentViewpointMatrix.addTransformation(hitMatrix);
        var camera = ArcGISRuntimeEnvironment.createObject("Camera", { transformationMatrix: matrix });
        return camera.location;
    }
}
