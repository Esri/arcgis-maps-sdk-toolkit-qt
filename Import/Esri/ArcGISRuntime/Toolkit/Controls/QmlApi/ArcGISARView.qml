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
import Esri.ArcGISRuntime 100.6

Item {
    id: root

    // properties
    // ARCNView?
    property Camera originCamera: internalCamera
    property alias sceneView: internalSceneView
    property real translationTransformationFactor: 1.0

    property bool renderVideoFeed: true // constructor?

    // methods
    function arScreenToLocation(screenPoint) {} // MapPoint f(Point)
    function resetTracking() {} // void f(void)
    function resetUsingLocationService() {} // bool f(void)
    function resetUsingSpacialAnchor() {} // bool f(void)
    function startTracking() {} // void f(void). Should return bool?
    function stopTracking() {} // void f(void)

    // internals
    SceneView {
        id: internalSceneView
    }

    Camera {
        id: internalCamera
    }
}


