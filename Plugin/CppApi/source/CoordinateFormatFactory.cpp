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

CoordinateConversionOptions* CoordinateFormatFactory::createFormat(const QString& formatName, QObject* parent)
{
  CoordinateConversionOptions* option = new CoordinateConversionOptions(parent);
  option->setName(formatName);

  if (formatName.compare(CoordinateConversionConstants::DEGREES_DECIMAL_MINUTES_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDegreesDecimalMinutes);
  }
  else if (formatName.compare(CoordinateConversionConstants::USNG_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUsng);
    option->setPrecision(7);
    option->setAddSpaces(true);
  }
  else if (formatName.compare(CoordinateConversionConstants::UTM_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeUtm);
    option->setUtmConversionMode(CoordinateConversionOptions::UtmConversionMode::UtmConversionModeNorthSouthIndicators);
    option->setAddSpaces(true);
  }
  else if (formatName.compare(CoordinateConversionConstants::DEGREES_MINUTES_SECONDS_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDegreesMinutesSeconds);
    option->setDecimalPlaces(6);
  }
  else if (formatName.compare(CoordinateConversionConstants::MGRS_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs);
    option->setMgrsConversionMode(CoordinateConversionOptions::MgrsConversionMode::MgrsConversionModeAutomatic);
  }
  else if (formatName.compare(CoordinateConversionConstants::DECIMAL_DEGREES_FORMAT, Qt::CaseInsensitive) == 0)
  {
    option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
    option->setLatLonFormat(CoordinateConversionOptions::LatitudeLongitudeFormat::LatitudeLongitudeFormatDecimalDegrees);
  }
  else
  {
    delete option;
    return nullptr;
  }

  return option;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
