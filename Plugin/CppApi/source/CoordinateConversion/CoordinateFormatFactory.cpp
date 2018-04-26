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

#include "CoordinateFormatFactory.h"

// toolkit headers
#include "CoordinateConversionConstants.h"
#include "CoordinateConversionOptions.h"

// C++ API headers
#include "GeodatabaseTypes.h"

// Qt headers
#include <QObject>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

int CoordinateFormatFactory::DMS_DECIMALPLACES = 6;
int CoordinateFormatFactory::USNG_PRECISION = 7;
bool CoordinateFormatFactory::USNG_SPACES = true;
MgrsConversionMode CoordinateFormatFactory::MGRS_CONVERSION_MODE = MgrsConversionMode::Automatic;
UtmConversionMode CoordinateFormatFactory::UTM_CONVERSION_MODE = UtmConversionMode::NorthSouthIndicators;
bool CoordinateFormatFactory::UTM_SPACES = true;
GarsConversionMode CoordinateFormatFactory::GARS_CONVERSION_MODE = GarsConversionMode::Center;

CoordinateConversionOptions* CoordinateFormatFactory::createFormat(const QString& formatName, QObject* parent)
{
  CoordinateConversionOptions* option = new CoordinateConversionOptions(parent);
  option->setName(formatName);

  if (formatName.compare(CoordinateConversionConstants::DEGREES_DECIMAL_MINUTES_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(LatitudeLongitudeFormat::DegreesDecimalMinutes);
  }
  else if (formatName.compare(CoordinateConversionConstants::USNG_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUsng);
    option->setPrecision(usngPrecision());
    option->setAddSpaces(usngUseSpaces());
  }
  else if (formatName.compare(CoordinateConversionConstants::UTM_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUtm);
    option->setUtmConversionMode(utmConversioMode());
    option->setAddSpaces(utmUseSpaces());
  }
  else if (formatName.compare(CoordinateConversionConstants::DEGREES_MINUTES_SECONDS_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(LatitudeLongitudeFormat::DegreesMinutesSeconds);
    option->setDecimalPlaces(degreesMinutesSecondsDecimalPlaces());
  }
  else if (formatName.compare(CoordinateConversionConstants::MGRS_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs);
    option->setMgrsConversionMode(mgrsConversionMode());
  }
  else if (formatName.compare(CoordinateConversionConstants::DECIMAL_DEGREES_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(LatitudeLongitudeFormat::DecimalDegrees);
  }
  else if (formatName.compare(CoordinateConversionConstants::GARS_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeGars);
    option->setGarsConversionMode(garsConversionMode());
  }
  else
  {
    delete option;
    return nullptr;
  }

  return option;
}

void CoordinateFormatFactory::setDegreesMinutesSecondsDecimalPlaces(int decimalPlaces)
{
  DMS_DECIMALPLACES = decimalPlaces;
}

int CoordinateFormatFactory::degreesMinutesSecondsDecimalPlaces()
{
  return DMS_DECIMALPLACES;
}

void CoordinateFormatFactory::setUsngPrecision(int precision)
{
  USNG_PRECISION = precision;
}

int CoordinateFormatFactory::usngPrecision()
{
  return USNG_PRECISION;
}

void CoordinateFormatFactory::setUsngUseSpaces(bool useSpaces)
{
  USNG_SPACES = useSpaces;
}

bool CoordinateFormatFactory::usngUseSpaces()
{
  return USNG_SPACES;
}

void CoordinateFormatFactory::setMgrsConversionMode(MgrsConversionMode conversionMode)
{
  MGRS_CONVERSION_MODE = conversionMode;
}

MgrsConversionMode CoordinateFormatFactory::mgrsConversionMode()
{
  return MGRS_CONVERSION_MODE;
}

void CoordinateFormatFactory::setUtmConversionMode(UtmConversionMode conversionMode)
{
  UTM_CONVERSION_MODE = conversionMode;
}

UtmConversionMode CoordinateFormatFactory::utmConversioMode()
{
  return UTM_CONVERSION_MODE;
}

void CoordinateFormatFactory::setUtmUseSpaces(bool useSpaces)
{
  UTM_SPACES = useSpaces;
}

bool CoordinateFormatFactory::utmUseSpaces()
{
  return UTM_SPACES;
}

void CoordinateFormatFactory::setGarsConversionMode(GarsConversionMode conversionMode)
{
  GARS_CONVERSION_MODE = conversionMode;
}

GarsConversionMode CoordinateFormatFactory::garsConversionMode()
{
  return GARS_CONVERSION_MODE;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
