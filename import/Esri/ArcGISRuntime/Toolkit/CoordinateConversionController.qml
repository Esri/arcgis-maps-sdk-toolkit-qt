import QtQml 2.12

import Esri.ArcGISRuntime 100.7

QtObject {
    id: controller

    signal currentPointChanged(Point point)

    property var geoView: null;

    property double zoomToDistance: 1500.0

    property bool inPickingMode: false

    property list<CoordinateConversionResult> results

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
            addSpaces: true
        },
        CoordinateConversionOption {
            name: "UTM"
            outputMode: CoordinateConversionOption.Utm
            utmConversionMode: Enums.UtmConversionModeNorthSouthIndicators
            addSpaces: true
        },
        CoordinateConversionOption {
            name: "GARS"
            outputMode: CoordinateConversionOption.Gars
            garsConversionMode: Enums.GarsConversionModeCenter
        }
    ]

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

    function currentPoint() {
        return internal.currentPoint;
    }

    function screenCoordinate() {
        if (!geoView)
            return Qt.point(-1, -1);

        let point = geoView.locationToScreen(internal.currentPoint);
        if (point && geoView.geoViewType === Enums.GeoViewTypeSceneView)
            return point.screenPoint;
        else
            return point;
    }

    function setCurrentPoint(point, option = undefined) {
        if (point instanceof String || typeof point == 'string') {
            if (option !== undefined) {
                let sp = geoView ?  geoView.spatialReference : undefined;
                internal.currentPoint = option.pointFromString(point, sp);
            } else {
                internal.currentPoint = null;
            }
        } else {
            internal.currentPoint = point;
        }
        currentPointChanged(internal.currentPoint);
    }

    function addNewCoordinateResultForOption(option) {
        let result = internal.resultBuilder.createObject(this, { type: option });
        currentPointChanged.connect(result.updateCoordinatePoint);
        result.updateCoordinatePoint(internal.currentPoint);
        internal.results.push(result);
        results = internal.results;
    }

    function removeCoordinateResultAtIndex(index) {
        internal.results.splice(index, 1);
        results = internal.results;
    }

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

    function forceUpdateCoordinates() {
        currentPointChanged(internal.currentPoint);
    }
}
