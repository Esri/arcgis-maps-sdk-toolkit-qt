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

#include "CoordinateConversionOption.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

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

CoordinateConversionOption::CoordinateType CoordinateConversionOption::outputMode() const
{
  return m_outputMode;
}

void CoordinateConversionOption::setOutputMode(CoordinateType outputMode)
{
  m_outputMode = outputMode;
  emit outputModeChanged();
}

QString CoordinateConversionOption::name() const
{
  return m_name;
}

void CoordinateConversionOption::setName(const QString& name)
{
  m_name = name;
  emit nameChanged();
}

bool CoordinateConversionOption::addSpaces() const
{
  return m_addSpaces;
}

void CoordinateConversionOption::setAddSpaces(bool addSpaces)
{
  m_addSpaces = addSpaces;
  emit addSpacesChanged();
}

int CoordinateConversionOption::precision() const
{
  return m_precision;
}

void CoordinateConversionOption::setPrecision(int precision)
{
  m_precision = precision;
  emit precisionChanged();
}

int CoordinateConversionOption::decimalPlaces() const
{
  return m_decimalPlaces;
}

void CoordinateConversionOption::setDecimalPlaces(int decimalPlaces)
{
  m_decimalPlaces = decimalPlaces;
  emit decimalPlacesChanged();
}

MgrsConversionMode CoordinateConversionOption::mgrsConversionMode() const
{
  return m_mgrsConversionMode;
}

void CoordinateConversionOption::setMgrsConversionMode(MgrsConversionMode mgrsConversionMode)
{
  m_mgrsConversionMode = mgrsConversionMode;
  emit mgrsConversionModeChanged();
}

LatitudeLongitudeFormat CoordinateConversionOption::latLonFormat() const
{
  return m_latLonFormat;
}

void CoordinateConversionOption::setLatLonFormat(LatitudeLongitudeFormat latLonFormat)
{
  m_latLonFormat = latLonFormat;
  emit latLonFormatChanged();
}

UtmConversionMode CoordinateConversionOption::utmConversionMode() const
{
  return m_utmConversionMode;
}

void CoordinateConversionOption::setUtmConversionMode(UtmConversionMode utmConversionMode)
{
  m_utmConversionMode = utmConversionMode;
  emit utmConversionModeChanged();
}

GarsConversionMode CoordinateConversionOption::garsConvesrionMode() const
{
  return m_garsConvesrionMode;
}

void CoordinateConversionOption::setGarsConversionMode(GarsConversionMode conversionMode)
{
  m_garsConvesrionMode = conversionMode;
  emit garsConversionModeChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri