import QtQml 2.12

import Esri.ArcGISRuntime 100.7

QtObject {
    enum CoordinateType {
        Gars,
        GeoRef,
        LatLon,
        Mgrs,
        Usng,
        Utm
    }

    property string name : ""

    property int outputMode : CoordinateConversionOption.CoordinateType.Usng

    property bool addSpaces: true

    property int precision: 8

    property int decimalPlaces: 6

    property int mgrsConversionMode: Enums.MgrsConversionModeAutomatic

    property int latLonFormat: Enums.LatitudeLongitudeFormatDecimalDegrees

    property int utmConversionMode: Enums.UtmConversionModeLatitudeBandIndicators

    property int garsConversionMode: Enums.GarsConversionModeCenter

    function prettyPrint(point) {
        switch (outputMode) {
        case CoordinateConversionOption.CoordinateType.Gars:
            return CoordinateFormatter.toGars(point);
        case CoordinateConversionOption.CoordinateType.GeoRef:
            return CoordinateFormatter.toGeoRef(point, precision);
        case CoordinateConversionOption.CoordinateType.LatLon:
            return CoordinateFormatter.toLatitudeLongitude(point, latLonFormat, decimalPlaces);
        case CoordinateConversionOption.CoordinateType.Mgrs:
            return CoordinateFormatter.toMgrs(point, mgrsConversionMode, decimalPlaces, addSpaces);
        case CoordinateConversionOption.CoordinateType.Usng:
            return CoordinateFormatter.toUsng(point, precision, decimalPlaces);
        case CoordinateConversionOption.CoordinateType.Utm:
            return CoordinateFormatter.toUtm(point, utmConversionMode, addSpaces);
        default:
            return "";
        }
    }

    function pointFromString(point, spatialReference) {
        if (!spatialReference)
            console.warn("The spatial reference property is empty: conversions will fail.");

        switch (outputMode) {
        case CoordinateConversionOption.CoordinateType.Gars:
            return CoordinateFormatter.fromGars(point, spatialReference, garsConvesrionMode);
        case CoordinateConversionOption.CoordinateType.GeoRef:
            return CoordinateFormatter.fromGeoRef(point, spatialReference);
        case CoordinateConversionOption.CoordinateType.LatLon:
            return CoordinateFormatter.fromLatitudeLongitude(point, spatialReference);
        case CoordinateConversionOption.CoordinateType.Mgrs:
            return CoordinateFormatter.fromMgrs(point, spatialReference, mgrsConversionMode);
        case CoordinateConversionOption.CoordinateType.Usng:
            return CoordinateFormatter.fromUsng(point, spatialReference);
        case CoordinateConversionOption.CoordinateType.Utm:
            return CoordinateFormatter.fromUtm(point, spatialReference, utmConversionMode);
        default: return null;
        }
    }
}
