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

import Esri.ArcGISRuntime 100.7

/*!
 * \qmltype CoordinateConversionOption
 * \inqmlmodule Esri.ArcGISRuntime.Toolkit
 * \since Esri.ArcGISRutime 100.8
 * \brief a CoordinateConversionOption is a collection of properties that
 * dictates how a Point should be converted to and from a string.
 * 
 * a CoordinateConversionOption is able to convert between Point <--> string
 * using the formatting options it is currently set with.
 */

QtObject {

    /*! \brief Fundamental coordinate format.*/
    enum CoordinateType {
        Gars,
        GeoRef,
        LatLon,
        Mgrs,
        Usng,
        Utm
    }

    /*!
     * \brief The user friendly name of this option.
     */
    property string name : ""

    /*!
     * \brief The conversion type of this option.
     */
    property int outputMode : CoordinateConversionOption.CoordinateType.Usng

    /*!
     * \brief Flags whether to add spaces in notation (if applicable).
     */
    property bool addSpaces: true

    /*!
     * \brief Precision of conversion format (if applicable).
     */
    property int precision: 8

    /*!
     * \brief The number of decimal spaces in the format (if applicable).
     */
    property int decimalPlaces: 6

    /*!
     * \brief The MGRS conversion mode (if applicable).
     */
    property int mgrsConversionMode: Enums.MgrsConversionModeAutomatic

    /*!
     * \brief The Latitude and Longitude format (if applicable).
     */
    property int latLonFormat: Enums.LatitudeLongitudeFormatDecimalDegrees

    /*!
     * \brief The UTM conversion mode (if applicable).
     */
    property int utmConversionMode: Enums.UtmConversionModeLatitudeBandIndicators

    /*!
     * \brief The GARS conversion mode (if applicable).
     */
    property int garsConversionMode: Enums.GarsConversionModeCenter

   /*!
    * \brief Converts a Point to a string based on the properties set in this
    * CoordinateConversionOption
    * 
    * \param point Point to convert to string.
    * \return string representation of point with the current formatting options
    * applied.
    */
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

   /*!
    * \brief Given a string called point, converts it to a Point using the
    * current formatting options applied.
    * 
    * This conversion has the chance of failing, in which case null is returned.
    * 
    * \param point string to conver to a Point.
    * \param spatialReference SpatialReference to use in conversion.
    * \return string as a Point. 
    */
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
