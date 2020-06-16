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
import Esri.ArcGISRuntime 100.8

/*!
   \qmltype CoordinateConversionController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRutime 100.8
   \brief In MVC architecture, this is the controller for the 
   CoordinateConversion view.
 */

QtObject {
    id: controller

    /*!
      \qmlproperty GeoView geoView
      \brief The GeoView for this tool. Should be a SceneView or a MapView.
     */
    property var geoView: null;

    /*!
       \brief Distance between camera and current point when zoomToCurrentPoint
       is invoked.
       
       If geoView is a SceneView, then this value represents the distance
       between the camera and the point returned by currentPoint when 
       zoomToCurrentPoint is called.
       
       If geoView is a MapView, this value has no effect on zoom calculations.
       
       The distance is in the units of the GeoView's SpatialReference.
       
       Defaults to 1500.
     */
    property double zoomToDistance: 1500.0

    /*!
       \brief Whether this controller is actively listening for mouse
       events.
       
       When listening for mouse events, the Controller will update the
       current point to whatever point is returned by a click event on the
       GeoView.
       
       When true this controller is listening to mouse events on the GeoView
       and will actively update the CurrentPoint.
       When false this controller is not listening to mouse events on the
       GeoView.
     */
    property bool inPickingMode: false

    /*!
       \brief List of textual representations of the current point in different 
       formats.
     */
    property list<CoordinateConversionResult> results

    /*! \brief Emitted when the currentPoint has changed. */
    signal currentPointChanged(Point point)

    /*!
      \brief Known list of available coordinate conversion formats which can be
      consumed to generate different textual representations of the same point.
    */
    property list<CoordinateConversionOption> formats: [
        CoordinateConversionOption {
            name: "DD"
            outputMode: CoordinateConversionOption.LatLon
            latLonFormat: Enums.LatitudeLongitudeFormatDecimalDegrees
        },
        CoordinateConversionOption {
            name: "DDM"
            outputMode: CoordinateConversionOption.LatLon
            latLonFormat: Enums.LatitudeLongitudeFormatDegreesDecimalMinutes
        },
        CoordinateConversionOption {
            name: "DMS"
            outputMode: CoordinateConversionOption.LatLon
            latLonFormat: Enums.LatitudeLongitudeFormatDegreesMinutesSeconds
        },
        CoordinateConversionOption {
            name: "MGRS"
            outputMode: CoordinateConversionOption.Mgrs
            mgrsConversionMode: Enums.MgrsConversionModeAutomatic
        },
        CoordinateConversionOption {
            name: "USNG"
            outputMode: CoordinateConversionOption.Usng
            precision: 7
            hasSpaces: true
        },
        CoordinateConversionOption {
            name: "UTM"
            outputMode: CoordinateConversionOption.Utm
            utmConversionMode: Enums.UtmConversionModeNorthSouthIndicators
            hasSpaces: true
        },
        CoordinateConversionOption {
            name: "GARS"
            outputMode: CoordinateConversionOption.Gars
            garsConversionMode: Enums.GarsConversionModeCenter
        }
    ]

    /*!
      \brief Returns the current point.
      
      The current point represents the one point that all elements within
      the conversionResults list-model are tied to.
    */
    function currentPoint() {
        return internal.currentPoint;
    }

    /*!
      \brief Converts the current point held by this controller as a 2D point 
      relative to the current window.
      
      Returns the point returned by currentPoint as a screen coordinate. 
    */
    function screenCoordinate() {
        if (!geoView)
            return Qt.point(-1, -1);

        let point = geoView.locationToScreen(internal.currentPoint);
        if (point && geoView.geoViewType === Enums.GeoViewTypeSceneView)
            return point.screenPoint;

        return point;
    }

    /*!
      \brief Set the current point to point. This updates all textual
      representations owned by this controller.

      \list
      \li \a point new point to convert.
      \li \a option Option dictating hints on how to convert the string to a
      point.
      \endlist
    */
    function setCurrentPoint(point, option = undefined) {
        if (point instanceof String || typeof point == 'string') {
            // Point is  a string, so convert to a point using the option param.
            if (option !== undefined) {
                let sp = geoView ?  geoView.spatialReference : undefined;
                internal.currentPoint = option.pointFromString(point, sp);
            } else {
                internal.currentPoint = null;
            }
        } else {
            // Point is a regular point, just update the interal point.
            internal.currentPoint = point;
        }
        // Point has changed, update!
        currentPointChanged(internal.currentPoint);
    }

    /*!
      \brief Given option, generates a new result.
      
      A new CoordianteConversionResult is added to the list-model returned by
      coordinateResults. This Result is tied to the conversion parameters as
      given by option.
      
      \list
      \li \a option option to generate result for.
      \endlist
    */
    function addNewCoordinateResultForOption(option) {
        let result = internal.resultBuilder.createObject(this, { type: option });
        currentPointChanged.connect(result.updateCoordinatePoint);
        result.updateCoordinatePoint(internal.currentPoint);
        internal.results.push(result);
        // Regenerate results list.
        results = internal.results;
    }

    /*!
      \brief Removes a given CoordinateConversionResult at index.
      
      This function is for deleting results in the list-model returned by
      coordinateREsults.
      
      \list
      \li \a index index of a given CoordinateConversionResult in the list-model
      returned by coordinateResults.
      \endlist
    */
    function removeCoordinateResultAtIndex(index) {
        internal.results.splice(index, 1);
        // Regenerate results list.
        results = internal.results;
    }

    /*!
      \brief Updates the GeoView's camera to point to the current point's
      location on the map or scene.
      \sa zoomToDistance
    */
    function zoomToCurrentPoint() {
        if (geoView === null)
            return;

        if (geoView.currentViewpointCamera !== undefined) {
            // This is a SceneView.
            let camera = geoView.currentViewpointCamera;
            let newCamera = ArcGISRuntimeEnvironment.createObject("Camera", {
                "location": internal.currentPoint,
                "distance": zoomToDistance,
                "heading": camera.heading,
                "roll": camera.roll,
                "pitch": camera.pitch
            });
            geoView.setViewpointCameraAndSeconds(newCamera, 1.0);
        }
        else if (geoView.mapRotation !== undefined) {
            // This is a MapView.
            let newViewpoint = ArcGISRuntimeEnvironment.createObject(
                "ViewpointCenter", {
                "center": internal.currentPoint,
                "targetScale": geoView.currentViewpointCenter.targetScale
            });
            geoView.setViewpointAndSeconds(newViewpoint, 1.0);
        }
    }

    /*! \internal */
    property QtObject internal: QtObject {
        property Point currentPoint: null
        property Component resultBuilder: Qt.createComponent("CoordinateConversionResult.qml")
        property var results: [] // Results list

        property Connections geoViewConnections: Connections {
            target: controller.geoView
            onMouseClicked: {
                if (!inPickingMode)
                    return;

                if (geoView.screenToBaseSurface !== undefined) {
                    // Is SceneView
                    setCurrentPoint(geoView.screenToBaseSurface(mouse.x, mouse.y));
                }
                else if (geoView.screenToLocation !== undefined) {
                    // Is MapView
                    setCurrentPoint(geoView.screenToLocation(mouse.x, mouse.y));
                }
            }
        }
    }
}
