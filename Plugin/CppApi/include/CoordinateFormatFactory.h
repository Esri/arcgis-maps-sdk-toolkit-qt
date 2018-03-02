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
