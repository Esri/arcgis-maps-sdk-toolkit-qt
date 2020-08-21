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
#include "CoordinateOptionDefaults.h"

#include "CoordinateConversionConstants.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversionOption* createDecimalDegrees(QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::DECIMAL_DEGREES_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::LatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DecimalDegrees);
  return option;
}

CoordinateConversionOption* createDegreesDecimalMinutes(QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::DEGREES_DECIMAL_MINUTES_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::LatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DegreesDecimalMinutes);
  return option;
}

CoordinateConversionOption* createDegreesMinutesSeconds(QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::DEGREES_MINUTES_SECONDS_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::LatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DegreesMinutesSeconds);
  return option;
}

CoordinateConversionOption* createMgrs(MgrsConversionMode mode, QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::MGRS_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::Mgrs);
  option->setMgrsConversionMode(mode);
  return option;
}

CoordinateConversionOption* createUsng(int precision, bool useSpaces, QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::USNG_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::Usng);
  option->setPrecision(precision);
  option->setHasSpaces(useSpaces);
  return option;
}

CoordinateConversionOption* createUtm(UtmConversionMode mode, bool useSpaces, QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::UTM_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::Utm);
  option->setUtmConversionMode(mode);
  option->setHasSpaces(useSpaces);
  return option;
}

CoordinateConversionOption* createGars(GarsConversionMode mode, QObject* parent)
{
  auto option = new CoordinateConversionOption(parent);
  option->setName(CoordinateConversionConstants::GARS_FORMAT);
  option->setOutputMode(CoordinateConversionOption::CoordinateType::Gars);
  option->setGarsConversionMode(mode);
  return option;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
