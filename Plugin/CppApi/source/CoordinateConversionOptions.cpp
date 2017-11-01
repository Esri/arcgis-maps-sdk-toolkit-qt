// Copyright 2017 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "CoordinateConversionOptions.h"
#include "CoordinateConversionController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions
  \ingroup ToolCoordinateConversion
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.2
  \brief Options for converting between geographic coordinate notations.

  \table
  \header
    \li Supported notation
    \li Options (valid range)
  \row
    \li {1, 3} Latitude-longitude in degrees
    \li \l name
  \row
    \li \l latLonFormat
  \row
    \li \l decimalPlaces (0 to 16)
  \row
    \li GARS (Global Area Reference System)
    \li \l name
  \row
    \li {1, 2} GEOREF (World Geographic Reference System)
    \li \l name
  \row
    \li \l precision (0 to 9)
  \row
    \li {1, 4} MGRS (Military Grid Reference System)
    \li \l name
  \row
    \li \l mgrsConversionMode
  \row
    \li \l precision (0 to 8)
  \row
    \li \l addSpaces
  \row
    \li {1, 3} USNG (United States National Grid)
    \li \l name
  \row
    \li \l precision (0 to 8)
  \row
    \li \l addSpaces
  \row
    \li {1 ,3} UTM (Universal Transverse Mercator)
    \li \l name
  \row
    \li \l utmConversionMode
  \row
    \li \l addSpaces
  \endtable

  For more information see the API documentation for
  \l {http://../../../cpp/api-reference/esri-arcgisruntime-coordinateformatter.html} {CoordinateFormatter}.
 */

using MgrsConversionMode = CoordinateConversionOptions::MgrsConversionMode;
using LatitudeLongitudeFormat = CoordinateConversionOptions::LatitudeLongitudeFormat;
using UtmConversionMode = CoordinateConversionOptions::UtmConversionMode;

static const QString s_gars   = QStringLiteral("Gars");
static const QString s_georef = QStringLiteral("GeoRef");
static const QString s_latlon = QStringLiteral("LatLon");
static const QString s_mgrs   = QStringLiteral("Mgrs");
static const QString s_usng   = QStringLiteral("Usng");
static const QString s_utm    = QStringLiteral("Utm");

CoordinateConversionOptions::CoordinateConversionOptions(QObject* parent):
  QObject(parent)
{
}

CoordinateConversionOptions::~CoordinateConversionOptions()
{
}

/*!
  \qmlproperty CoordinateType CoordinateConversionOptions::outputMode

  \brief Gets the coordinate format this option will be output in.

  The default value is \l {CoordinateType} {CoordinateTypeUsng}.
 */
CoordinateConversionOptions::CoordinateType CoordinateConversionOptions::outputMode() const
{
  return m_outputMode;
}

/*!
  \brief Sets the coordinate format this option will be output in to \a outputMode.

  The default value is \l {CoordinateType} {CoordinateTypeUsng}.
 */
void CoordinateConversionOptions::setOutputMode(CoordinateType outputMode)
{
  m_outputMode = outputMode;
  emit outputModeChanged();
}

/*!
  \qmlproperty string CoordinateConversionOptions::name

  \brief Gets the name of the option.
 */
QString CoordinateConversionOptions::name() const
{
  return m_name;
}

/*!
  \brief Sets the name of the option to \a name.
 */
void CoordinateConversionOptions::setName(const QString& name)
{
  m_name = name;
  emit nameChanged();
}

/*!
  \qmlproperty bool CoordinateConversionOptions::addSpaces

  \brief Gets whether the notation output should use spaces.

  \note This option only applies to the following notation types:
  Mgrs, Usng and Utm.

  \note The default value is \c true.
 */
bool CoordinateConversionOptions::addSpaces() const
{
  return m_addSpaces;
}

/*!
  \brief Sets whether the notation output should use spaces to \a addSpaces.

  \note This option only applied to the following notation types:
  Mgrs, Usng and Utm.

  \note The default value is \c true.
 */
void CoordinateConversionOptions::setAddSpaces(bool addSpaces)
{
  m_addSpaces = addSpaces;
  emit addSpacesChanged();
}

/*!
  \qmlproperty int CoordinateConversionOptions::precision

  \brief Gets the precision for the option.

  \note This option only applies to the following notation types:
  GeoRef (0 to 9), MGRS (0 to 8), and USNG (0 to 8).

  \note The default value is \c 8.
 */
int CoordinateConversionOptions::precision() const
{
  return m_precision;
}

/*!
  \brief Sets the precision for the option to \a precision.

  \note This option only applies to the following notation types:
  GeoRef (0 to 9), MGRS (0 to 8), and USNG (0 to 8).

  The default value is \c 8.
 */
void CoordinateConversionOptions::setPrecision(int precision)
{
  m_precision = precision;
  emit precisionChanged();
}

/*!
  \qmlproperty int CoordinateConversionOptions::decimalPlaces

  \brief Gets the decimal places for the option.

  This option only applies to the LatLon (LatitudeLongitude) format.
  Possible values are \c 0 to \c 16. The default value is \c 6.
 */
int CoordinateConversionOptions::decimalPlaces() const
{
  return m_decimalPlaces;
}

/*!
  \brief Sets the decimal places for the option to \a decimalPlaces.

  This option only applies to the LatLon (LatitudeLongitude) format.
  Possible values are \c 0 to \c 16. The default value is 6.
 */
void CoordinateConversionOptions::setDecimalPlaces(int decimalPlaces)
{
  m_decimalPlaces = decimalPlaces;
  emit decimalPlacesChanged();
}

/*!
  \qmlproperty MgrsConversionMode CoordinateConversionOptions::mgrsConversionMode

  \brief Gets MGRS conversion mode.

  This option only applies to the MGRS format. The default value is
  \l {MgrsConversionMode} {\c MgrsConversionModeAutomatic}.
 */
MgrsConversionMode CoordinateConversionOptions::mgrsConversionMode() const
{
  return m_mgrsConversionMode;
}

/*!
  \brief Sets Mgrs conversion mode to \a mgrsConversionMode.

  This option only applies to the MGRS format. The default value is
  \l {MgrsConversionMode} {\c MgrsConversionModeAutomatic}.
 */
void CoordinateConversionOptions::setMgrsConversionMode(MgrsConversionMode mgrsConversionMode)
{
  m_mgrsConversionMode = mgrsConversionMode;
  emit mgrsConversionModeChanged();
}

/*!
  \qmlproperty LatitudeLongitudeFormat CoordinateConversionOptions::latLonFormat

  \brief Gets the latitude longitude format.

  \note This option only applies to the LatLon (LatitudeLongitude) format.

  \note The default value is DecimalDegrees.
 */
LatitudeLongitudeFormat CoordinateConversionOptions::latLonFormat() const
{
  return m_latLonFormat;
}

/*!
  \brief Sets the latitude longitude format to \a latLonFormat.

  \note This option only applies to the LatLon (LatitudeLongitude) format.

  \note The default value is DecimalDegrees.
 */
void CoordinateConversionOptions::setLatLonFormat(LatitudeLongitudeFormat latLonFormat)
{
  m_latLonFormat = latLonFormat;
  emit latLonFormatChanged();
}

/*!
  \qmlproperty UtmConversionMode CoordinateConversionOptions::utmConversionMode

  \brief Gets the Utm conversion mode.

  \note This option only applies to the Utm format.

  \note The default value is LatitudeBandIndicators.
 */
UtmConversionMode CoordinateConversionOptions::utmConversionMode() const
{
  return m_utmConversionMode;
}

/*!
  \brief Sets the Utm conversion mode to \a utmConversionMode.

  \note This option only applies to the Utm format.

  \note The default value is LatitudeBandIndicators.
 */
void CoordinateConversionOptions::setUtmConversionMode(UtmConversionMode utmConversionMode)
{
  m_utmConversionMode = utmConversionMode;
  emit utmConversionModeChanged();
}

/*!
  \internal
 */
void CoordinateConversionOptions::listAppend(QQmlListProperty<CoordinateConversionOptions>* property, CoordinateConversionOptions* value)
{
  static_cast<CoordinateConversionController*>(property->object)->addOption(value);
}

/*!
  \internal
 */
CoordinateConversionOptions* CoordinateConversionOptions::listAt(QQmlListProperty<CoordinateConversionOptions>* property, int index)
{
  return static_cast<QList<CoordinateConversionOptions*>*>(property->data)->value(index);
}

/*!
  \internal
 */
int CoordinateConversionOptions::listCount(QQmlListProperty<CoordinateConversionOptions>* property)
{
  return static_cast<QList<CoordinateConversionOptions*>*>(property->data)->count();
}

/*!
  \internal
 */
void CoordinateConversionOptions::listClear(QQmlListProperty<CoordinateConversionOptions>* property)
{
  static_cast<CoordinateConversionController*>(property->object)->clearOptions();
}

/*!
  \qmlmethod CoordinateType CoordinateConversionOptions::stringToCoordinateType(string type)

  \brief Converts the input string type to a CoordinateType enum value
 */
CoordinateConversionOptions::CoordinateType CoordinateConversionOptions::stringToCoordinateType(const QString& type) const
{
  if (type == s_gars)
    return CoordinateType::CoordinateTypeGars;
  else if (type == s_georef)
    return CoordinateType::CoordinateTypeGeoRef;
  else if (type == s_latlon)
    return CoordinateType::CoordinateTypeLatLon;
  else if (type == s_mgrs)
    return CoordinateType::CoordinateTypeMgrs;
  else if (type == s_usng)
    return CoordinateType::CoordinateTypeUsng;
  else if (type == s_utm)
    return CoordinateType::CoordinateTypeUtm;

  return CoordinateType::CoordinateTypeLatLon;
}

/*!
  \qmlmethod string CoordinateConversionOptions::coordinateTypeToString(CoordinateType type)

  \brief Converts the input enum value CoordinateType to a string representation.
 */
QString CoordinateConversionOptions::coordinateTypeToString(CoordinateType type) const
{
  switch (type)
  {
  case CoordinateType::CoordinateTypeGars:
    return s_gars;
  case CoordinateType::CoordinateTypeGeoRef:
    return s_georef;
  case CoordinateType::CoordinateTypeLatLon:
    return s_latlon;
  case CoordinateType::CoordinateTypeMgrs:
    return s_mgrs;
  case CoordinateType::CoordinateTypeUsng:
    return s_usng;
  case CoordinateType::CoordinateTypeUtm:
    return s_utm;
  default: {}
  }

  return QString();
}

/*!
  \qmlproperty list<string> CoordinateConversionOptions::coordinateTypeNames

  \brief A list of strings of the supported coordinate types.
 */
QStringList CoordinateConversionOptions::coordinateTypeNames() const
{
  return QStringList() << s_gars
                       << s_georef
                       << s_latlon
                       << s_mgrs
                       << s_usng
                       << s_utm;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
