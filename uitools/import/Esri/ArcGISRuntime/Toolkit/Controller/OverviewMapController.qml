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
import QtQml 2.12
import Esri.ArcGISRuntime 100.13

/*!
   \qmltype OverviewMapController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRutime 100.12
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief The controller part of a OverviewMap. This class handles the
    management of the inset MapView, including symbol styling and navigation.
 */

QtObject {
    id: overviewMapController

    /*!
      \qmlproperty GeoView geoView
      \brief The geoview the controller is listening for viewpoint changes.
     */
    property var geoView;

    /*!
      \qmlproperty bool navigationEnabled
      \brief When this property is \c{true}, the user is able to navigate the insetView, which
      in turn updates the geoView. When this property is \c{false}, the insetView does not respond
      to mouse/touch events. Defaults to \c{true}.
     */
    property bool navigationEnabled: true;

    /*!
      \qmlproperty real scaleFactor
      \brief Sets the amount to scale the OverviewMap compared to the geoView.
     */
    property real scaleFactor: 25;

    /*!
      \qmlproperty Symbol symbol
      \brief The symbol used to represent the GeoView's current viewpoint.

       If \l OverviewMapController::geoView is not set, then the symbol will
       return \c{nullptr} by default.

       If \l OverviewMapController::geoView is set, and a custom symbol was not applied,
       then a default will be provided.

       For MapView, the default is an empty SimpleFillSymbol with a 1 point red outline;
       for SceneView, it will be a red, cross-hair SimpleMarkerSymbol.
     */
    property Symbol symbol: {
        if (geoView instanceof MapView) {
            return internal.defaultMapSymbol
        } else if (geoView instanceof SceneView) {
            return internal.defaultSceneSymbol
        } else {
            return null;
        }
    }

    /*!
      \qmlproperty MapView insetView
      \brief MapView which represents an overview/inset of the current viewpoint of the geoView.
     */
    readonly property MapView insetView: MapView {
        // Default map to show in the inset.
        Map {
            initBasemapStyle: Enums.BasemapStyleArcGISTopographic
        }

        // The Graphic shown on the insetView showing the GeoView viewpoint.
        GraphicsOverlay {
            Graphic {
                id: reticle
                symbol: overviewMapController.symbol
            }
        }

        // By default the inset does not have attribution visible.
        attributionTextVisible: false

        // If an inset viewpoint change is driven by user navigation, then
        // update the geoView viewpoint if and when applicable.
        onViewpointChanged: {
            if (navigating && !internal.updateInsetViewpointTaskInProgress) {
                if (geoView instanceof MapView) {
                    let v = currentViewpointCenter;
                    let newV = internal.viewpoint.createObject(null, {
                                                                   center: v.center,
                                                                   targetScale:  v.targetScale / overviewMapController.scaleFactor,
                                                                   rotation: v.rotation
                                                               });
                    internal.updateGeoViewpointTaskInProgress = true;
                    geoView.setViewpointAndSeconds(newV, 0);
                } else if (geoView instanceof SceneView) {
                    let v = currentViewpointCenter;
                    let newV = internal.viewpoint.createObject(null, {
                                                                   center: v.center,
                                                                   targetScale:  v.targetScale / overviewMapController.scaleFactor
                                                               });
                    internal.updateGeoViewpointTaskInProgress = true;
                    geoView.setViewpointAndSeconds(newV, 0);
                }
            }
        }

        onSetViewpointCompleted: {
            internal.updateInsetViewpointTaskInProgress = false;
        }

        onKeyPressed: {
            // Disable all key preesses
            key.accepted = true;
        }
        onKeyReleased: {
            // Disable all key releases
            key.accepted = true;
        }
        onMouseWheelChanged: {
            // Disable all wheel events if navigation is not enabled.
            if (!navigationEnabled)
                wheel.accepted = true;
        }
        onMouseClicked: {
            // Disable mouse clicks if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
        onMouseDoubleClicked: {
            // Disable mouse double clicks if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
        onMousePositionChanged: {
            // Disable mouse position changes if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
        onMousePressedAndHeld: {
            // Disable mouse press and hold if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
        onMousePressed: {
            // Disable mouse press, if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
        onMouseReleased: {
            // Disable mouse release if navigation is not enabled.
            if (!navigationEnabled)
                mouse.accepted = true;
        }
    }

    property QtObject internal: QtObject {
        // Keeps track of the progress of custom (non-navigaton) updates to the insetView viewpoint.
        property bool updateInsetViewpointTaskInProgress: false;

        // Keeps track of the progress of custom (non-navigaton) updates to the geoView viewpoint.
        property bool updateGeoViewpointTaskInProgress: false;

        // Track geoView changes, and update the insetView as applicable.
        // Note that we apply slightly different calculations between MapView and SceneView.
        // 1. SceneView navigation does not care about rotations, while the MapView does.
        // 2. The Reticle geometry is the visible area when the geoView is a MapView. Otherwise it is the
        //    viewpoint center for a SceneView geoView.
        property Connections geoViewConnections: Connections {
            target: geoView
            function onViewpointChanged() {
                if (geoView instanceof MapView) {
                    reticle.geometry = geoView.visibleArea;
                    if (geoView.navigating && !internal.updateGeoViewpointTaskInProgress) {
                        let v = geoView.currentViewpointCenter;
                        let newV = internal.viewpoint.createObject(null, {rotation: v.rotation});
                        internal.updateInsetViewpointTaskInProgress = true;
                        insetView.setViewpointAndSeconds(newV, 0);
                    }
                } else if (geoView instanceof SceneView) {
                    let v = geoView.currentViewpointCenter;
                    if (v) { // Sometimes currentViewpointCenter can come back as null.
                        reticle.geometry = v.center;
                        if (geoView.navigating && !internal.updateGeoViewpointTaskInProgress) {
                            let newV = internal.viewpoint.createObject(null);
                            internal.updateInsetViewpointTaskInProgress = true;
                            insetView.setViewpointAndSeconds(newV, 0);
                        }
                    }
                } else {
                    console.error("Unknown GeoView type passed to OverViewMapController.");
                }
            }

            function onSetViewpointCompleted(succeeded) {
                internal.updateGeoViewpointTaskInProgress = false;
            }
        }

        // Default reticle symbol for SceneView.
        property SimpleMarkerSymbol defaultSceneSymbol: SimpleMarkerSymbol {
            style: Enums.SimpleMarkerSymbolStyleCross
            color: "red"
            size: 16.0
        }

        // Default reticle symbol for MapView.
        property SimpleFillSymbol defaultMapSymbol: SimpleFillSymbol {
            style: Enums.SimpleFillSymbolStyleNull
            SimpleLineSymbol {
                style: Enums.SimpleLineSymbolStyleSolid
                color: "red"
                width: 1.0
            }
        }

        // Component for creating new Viewpoints on the fly.
        property Component viewpoint: Component {
            ViewpointCenter {
                center: geoView.currentViewpointCenter.center
                targetScale: geoView.currentViewpointCenter.targetScale * overviewMapController.scaleFactor
            }
        }
    }
}
