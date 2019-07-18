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

import Esri.ArcGISRuntime 100.6

TransformationMatrixCameraController {
    id: root

    function createAndSetTransformationMatrix(quaternionX, quaternionY, quaternionZ, quaternionW,
                                              translationX, translationY, translationZ)
    {
        var matrix = TransformationMatrix.createWithQuaternionAndTranslation(
                    quaternionX, quaternionY, quaternionZ, quaternionW,
                    translationX, translationY, translationZ);
        root.transformationMatrix = matrix;
    }
}
