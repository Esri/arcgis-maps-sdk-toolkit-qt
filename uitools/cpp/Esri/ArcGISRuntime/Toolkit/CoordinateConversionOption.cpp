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
#include "CoordinateConversionOption.h"

// ArcGISRuntime headers
#include <CoordinateFormatter.h>
#include <Point.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption
  \inmodule ArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief a \c CoordinateConversionOption is a collection of properties that
  dictates how a \c Point should be converted to and from a string.
  
  A \c CoordinateConversionOption is able to convert between Point and QString
  using the formatting options it is currently set with.
 */

/*!
  \brief Constructor
  \list
   \li \a parent Owning parent QObject.
  \endlist
 */
CoordinateConversionOption::CoordinateConversionOption(QObject* parent):
  QObject(parent)
{
}

/*!
   \brief The destructor.
 */
CoordinateConversionOption::~CoordinateConversionOption()
{
}

/*!
  \brief Returns the conversion type of this option.
 */
CoordinateConversionOption::CoordinateType CoordinateConversionOption::outputMode() const
{
  return m_outputMode;
}

/*!
  \brief Sets the conversion type to \a outputMode.
 */
void CoordinateConversionOption::setOutputMode(CoordinateType outputMode)
{
  if (outputMode == m_outputMode)
    return;

  m_outputMode = outputMode;
  emit outputModeChanged();
}

/*!
 \brief Returns the user friendly name of this option.
 */
QString CoordinateConversionOption::name() const
{
  return m_name;
}

/*!
  \brief Set \c name to \a name.
 */
void CoordinateConversionOption::setName(const QString& name)
{
  if (name == m_name)
    return;

  m_name = name;
  emit nameChanged();
}

/*!
  \brief Returns flag whether to add spaces in notation (if applicable).
  */
bool CoordinateConversionOption::hasSpaces() const
{
  return m_hasSpaces;
}

 /*!
  \brief Set \c hasSpaces to \a hasSpaces.
  */
void CoordinateConversionOption::setHasSpaces(bool hasSpaces)
{
  if (m_hasSpaces == hasSpaces)
    return;

  m_hasSpaces = hasSpaces;
  emit hasSpacesChanged();
}

 /*!
  \brief Returns precision of conversion format (if applicable).
 */
int CoordinateConversionOption::precision() const
{
  return m_precision;
}

/*!
 \brief Set \c precision to \a precision.
 */
void CoordinateConversionOption::setPrecision(int precision)
{
  if (m_precision == precision)
    return;

  m_precision = precision;
  emit precisionChanged();
}

/*!
  \brief Returns the number of decimal spaces in the format (if applicable).
 */
int CoordinateConversionOption::decimalPlaces() const
{
  return m_decimalPlaces;
}

/*!
  \brief Set \c decimalPlaces to \a decimalPlaces.
 */
void CoordinateConversionOption::setDecimalPlaces(int decimalPlaces)
{
  if (m_decimalPlaces == decimalPlaces)
    return;

  m_decimalPlaces = decimalPlaces;
  emit decimalPlacesChanged();
}

/*!
 \brief Returns the MGRS conversion mode (if applicable).
 */
MgrsConversionMode CoordinateConversionOption::mgrsConversionMode() const
{
  return m_mgrsConversionMode;
}

/*!
 \brief Set \c mgrsConversionMode to \a mgrsConversionMode.
 */
void CoordinateConversionOption::setMgrsConversionMode(MgrsConversionMode mgrsConversionMode)
{
  if (m_mgrsConversionMode == mgrsConversionMode)
    return;

  m_mgrsConversionMode = mgrsConversionMode;
  emit mgrsConversionModeChanged();
}

/*!
  \brief Returns the Latitude and Longitude format (if applicable).
 */
LatitudeLongitudeFormat CoordinateConversionOption::latLonFormat() const
{
  return m_latLonFormat;
}

/*!
 * \brief Sets \c latLonFormat to \a latLonFormat.
 */
void CoordinateConversionOption::setLatLonFormat(LatitudeLongitudeFormat latLonFormat)
{
  if (m_latLonFormat == latLonFormat)
    return;

  m_latLonFormat = latLonFormat;
  emit latLonFormatChanged();
}

/*!
 * \brief Returns the UTM conversion mode (if applicable).
 */
UtmConversionMode CoordinateConversionOption::utmConversionMode() const
{
  return m_utmConversionMode;
}

/*!
 * \brief Sets \c utmConversionMode to \a utmConversionMode.
 */
void CoordinateConversionOption::setUtmConversionMode(UtmConversionMode utmConversionMode)
{
  if (m_utmConversionMode == utmConversionMode)
    return;

  m_utmConversionMode = utmConversionMode;
  emit utmConversionModeChanged();
}

/*!
 * \brief Returns the GARS conversion mode (if applicable).
 */
GarsConversionMode CoordinateConversionOption::garsConversionMode() const
{
  return m_garsConvesrionMode;
}

/*!
 * \brief Sets \c garsConversionMode to \a conversionMode.
 */
void CoordinateConversionOption::setGarsConversionMode(GarsConversionMode conversionMode)
{
  if (m_garsConvesrionMode == conversionMode)
    return;

  m_garsConvesrionMode = conversionMode;
  emit garsConversionModeChanged();
}

/*!
  \brief Converts a \c Point to a \c QString based on the properties set in this
  \c CoordinateConversionOption.
  
  \list
  \li \a point \c Point to convert to \c QString.
  \endlist

  Returns a representation of point with the current formatting options
  applied.
 */
QString CoordinateConversionOption::prettyPrint(const Point& point) const
{
  switch (outputMode())
  {
    case CoordinateType::Gars:
    {
      return CoordinateFormatter::toGars(point);
    }
    case CoordinateType::GeoRef:
    {
      return CoordinateFormatter::toGeoRef(point, precision());
    }
    case CoordinateType::LatLon:
    {
      const auto format = latLonFormat();
      return CoordinateFormatter::toLatitudeLongitude(point, format, decimalPlaces());
    }
    case CoordinateType::Mgrs:
    {
      const auto conversionMode = mgrsConversionMode();
      return CoordinateFormatter::toMgrs(point, conversionMode, decimalPlaces(), hasSpaces());
    }
    case CoordinateType::Usng:
    {
      return CoordinateFormatter::toUsng(point, precision(), decimalPlaces());
    }
    case CoordinateType::Utm:
    {
      const auto conversionMode = utmConversionMode();
      return CoordinateFormatter::toUtm(point, conversionMode, hasSpaces());
    }
    default: 
      return QString();
  }
}

/*!
  \brief Given a string called \a point, converts it to a \c Point using the
  current formatting options applied.
  
  This conversion has the chance of failing, in which case an invalid
  default-constructed Point will be returned.
  
  \list
  \li \a point \c QString to convert to a \c Point.
  \li \a spatialReference \c SpatialReference to use in conversion.
  \endlist

  Returns the string as a \c Point. 
 */
Point CoordinateConversionOption::pointFromString(const QString& point, const SpatialReference& spatialReference)
{
  if (spatialReference.isEmpty())
      qWarning("The spatial reference property is empty: conversions will fail.");

  switch (outputMode())
  {
  case CoordinateType::Gars:
  {
    return CoordinateFormatter::fromGars(point,
                                         spatialReference,
                                         garsConversionMode());
  }
  case CoordinateType::GeoRef:
  {
    return CoordinateFormatter::fromGeoRef(point,
                                           spatialReference);
  }
  case CoordinateType::LatLon:
  {
    return CoordinateFormatter::fromLatitudeLongitude(point,
                                                      spatialReference);
  }
  case CoordinateType::Mgrs:
  {
    return CoordinateFormatter::fromMgrs(point,
                                         spatialReference,
                                         mgrsConversionMode());
  }
  case CoordinateType::Usng:
  {
    return CoordinateFormatter::fromUsng(point,
                                         spatialReference);
  }
  case CoordinateType::Utm:
  {
    return CoordinateFormatter::fromUtm(point,
                                        spatialReference,
                                        utmConversionMode());
  }
  default: return Point();
  }
}

/*!
  \enum Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::CoordinateType
  \brief The target coordinate output format.
  \value Gars Output format is GARS.
  \value GeoRef Output format is GeoRef.
  \value LatLon Output format is LatLon.
  \value Mgrs Output format is MGRS.
  \value Usng Output format is USGN.
  \value Utm Output format is UTM.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::nameChanged()
  \brief Emits when \c name property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::outputModeChanged()
  \brief Emits when \c outputMode property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::hasSpacesChanged()
  \brief Emits when \c hasSpaces property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::precisionChanged()
  \brief Emits when \c precision property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::decimalPlacesChanged()
  \brief Emits when \c decimal property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::mgrsConversionModeChanged()
  \brief Emits when \c mgrsConversion property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::latLonFormatChanged()
  \brief Emits when \c latLonFormat property changes. 
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::utmConversionModeChanged()
  \brief Emits when \c utmConversionMode property changes. 
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::garsConversionModeChanged()
  \brief Emits when \c garsConversionMode property changes. 
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::name
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::name()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::outputMode
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::outputMode()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::hasSpaces
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::hasSpaces()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::precision
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::precision()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::decimalPlaces
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::decimalPlaces()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::mgrsConversionMode
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::mgrsConversionMode()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::latLonFormat
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::latLonFormat()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::utmConversionMode
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::utmConversionMode()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::garsConversionMode
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption::garsConversionMode()
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
