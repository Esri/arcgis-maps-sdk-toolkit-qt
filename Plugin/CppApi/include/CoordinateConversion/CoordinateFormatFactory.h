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

#ifndef COORDINATEFORMATFACTORY_H
#define COORDINATEFORMATFACTORY_H

#include "ToolkitCommon.h"

class QObject;

namespace Esri
{
namespace ArcGISRuntime
{

enum class GarsConversionMode;
enum class MgrsConversionMode;
enum class UtmConversionMode;

namespace Toolkit
{

class CoordinateConversionOptions;

class TOOLKIT_EXPORT CoordinateFormatFactory
{
public:
  static CoordinateConversionOptions* createFormat(const QString& formatName, QObject* parent);

  static void setDegreesMinutesSecondsDecimalPlaces(int decimalPlaces);
  static int degreesMinutesSecondsDecimalPlaces();

  static void setUsngPrecision(int precision);
  static int usngPrecision();
  static void setUsngUseSpaces(bool useSpaces);
  static bool usngUseSpaces();

  static void setMgrsConversionMode(MgrsConversionMode conversionMode);
  static MgrsConversionMode mgrsConversionMode();

  static void setUtmConversionMode(UtmConversionMode conversionMode);
  static UtmConversionMode utmConversioMode();
  static void setUtmUseSpaces(bool useSpaces);
  static bool utmUseSpaces();

  static void setGarsConversionMode(GarsConversionMode conversionMode);
  static GarsConversionMode garsConversionMode();

private:
  static int DMS_DECIMALPLACES;
  static MgrsConversionMode MGRS_CONVERSION_MODE;
  static int USNG_PRECISION;
  static bool USNG_SPACES;
  static UtmConversionMode UTM_CONVERSION_MODE;
  static bool UTM_SPACES;
  static GarsConversionMode GARS_CONVERSION_MODE;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATEFORMATFACTORY_H
