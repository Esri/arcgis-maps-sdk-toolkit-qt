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

#ifndef COORDINATECONVERSIONCONSTANTS_H
#define COORDINATECONVERSIONCONSTANTS_H

#include "ToolkitCommon.h"

#include <QString>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class TOOLKIT_EXPORT CoordinateConversionConstants
{
public:
  static const QString DECIMAL_DEGREES_FORMAT;
  static const QString DEGREES_DECIMAL_MINUTES_FORMAT;
  static const QString DEGREES_MINUTES_SECONDS_FORMAT;
  static const QString MGRS_FORMAT;
  static const QString USNG_FORMAT;
  static const QString UTM_FORMAT;
  static const QString GARS_FORMAT;
  static const QString GEOREF_FORMAT;
  static const QString LATLON;
  static const QString COORDINATE_FORMAT_PROPERTY;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONSTANTS_H
