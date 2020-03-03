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

CoordinateConversionOption::CoordinateConversionOption(QObject* parent):
  QObject(parent),
  m_outputMode(CoordinateType::Usng),
  m_addSpaces(true),
  m_precision(8),
  m_decimalPlaces(6),
  m_mgrsConversionMode(MgrsConversionMode::Automatic),
  m_latLonFormat(LatitudeLongitudeFormat::DecimalDegrees),
  m_utmConversionMode(UtmConversionMode::LatitudeBandIndicators),
  m_garsConvesrionMode(GarsConversionMode::Center)
{
}

/*!
   \brief The destructor.
 */
CoordinateConversionOption::~CoordinateConversionOption()
{
}

CoordinateConversionOption::CoordinateType CoordinateConversionOption::outputMode() const
{
  return m_outputMode;
}

void CoordinateConversionOption::setOutputMode(CoordinateType outputMode)
{
  if (outputMode == m_outputMode)
    return;

  m_outputMode = outputMode;
  emit outputModeChanged();
}

QString CoordinateConversionOption::name() const
{
  return m_name;
}

void CoordinateConversionOption::setName(const QString& name)
{
  if (name == m_name)
    return;

  m_name = name;
  emit nameChanged();
}

bool CoordinateConversionOption::addSpaces() const
{
  return m_addSpaces;
}

void CoordinateConversionOption::setAddSpaces(bool addSpaces)
{
  if (m_addSpaces == addSpaces)
    return;

  m_addSpaces = addSpaces;
  emit addSpacesChanged();
}

int CoordinateConversionOption::precision() const
{
  return m_precision;
}

void CoordinateConversionOption::setPrecision(int precision)
{
  if (m_precision == precision)
    return;

  m_precision = precision;
  emit precisionChanged();
}

int CoordinateConversionOption::decimalPlaces() const
{
  return m_decimalPlaces;
}

void CoordinateConversionOption::setDecimalPlaces(int decimalPlaces)
{
  if (m_decimalPlaces == decimalPlaces)
    return;

  m_decimalPlaces = decimalPlaces;
  emit decimalPlacesChanged();
}

MgrsConversionMode CoordinateConversionOption::mgrsConversionMode() const
{
  return m_mgrsConversionMode;
}

void CoordinateConversionOption::setMgrsConversionMode(MgrsConversionMode mgrsConversionMode)
{
  if (m_mgrsConversionMode == mgrsConversionMode)
    return;

  m_mgrsConversionMode = mgrsConversionMode;
  emit mgrsConversionModeChanged();
}

LatitudeLongitudeFormat CoordinateConversionOption::latLonFormat() const
{
  return m_latLonFormat;
}

void CoordinateConversionOption::setLatLonFormat(LatitudeLongitudeFormat latLonFormat)
{
  if (m_latLonFormat == latLonFormat)
    return;

  m_latLonFormat = latLonFormat;
  emit latLonFormatChanged();
}

UtmConversionMode CoordinateConversionOption::utmConversionMode() const
{
  return m_utmConversionMode;
}

void CoordinateConversionOption::setUtmConversionMode(UtmConversionMode utmConversionMode)
{
  if (m_utmConversionMode == utmConversionMode)
    return;

  m_utmConversionMode = utmConversionMode;
  emit utmConversionModeChanged();
}

GarsConversionMode CoordinateConversionOption::garsConvesrionMode() const
{
  return m_garsConvesrionMode;
}

void CoordinateConversionOption::setGarsConversionMode(GarsConversionMode conversionMode)
{
  if (m_garsConvesrionMode == conversionMode)
    return;

  m_garsConvesrionMode = conversionMode;
  emit garsConversionModeChanged();
}

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
      return CoordinateFormatter::toMgrs(point, conversionMode, decimalPlaces(), addSpaces());
    }
    case CoordinateType::Usng:
    {
      return CoordinateFormatter::toUsng(point, precision(), decimalPlaces());
    }
    case CoordinateType::Utm:
    {
      const auto conversionMode = utmConversionMode();
      return CoordinateFormatter::toUtm(point, conversionMode, addSpaces());
    }
    default: 
      return QString();
  }
}

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
                                         garsConvesrionMode());
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

} // Toolkit
} // ArcGISRuntime
} // Esri
