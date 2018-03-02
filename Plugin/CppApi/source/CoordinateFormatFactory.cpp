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
#include "CoordinateConversionConstants.h"
#include "CoordinateFormatFactory.h"

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
    option->setLatLonFormat(LatitudeLongitudeFormat::DegreesDecimalMinutes);
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
