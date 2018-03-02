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
