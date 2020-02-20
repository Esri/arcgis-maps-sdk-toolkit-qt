#include "CoordinateOptionDefaults.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversionOption* createDecimalDegrees(QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("DD");
  option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DecimalDegrees);
  return option;
}

CoordinateConversionOption* createDegreesDecimalMinutes(QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("DDM");
  option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DegreesDecimalMinutes);
  return option;
}

CoordinateConversionOption* createDegreesMinutesSeconds(QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("DD");
  option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeLatLon);
  option->setLatLonFormat(LatitudeLongitudeFormat::DegreesMinutesSeconds);
  return option;
}

CoordinateConversionOption* createMgrs(MgrsConversionMode mode, QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("MGRS");
  option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeMgrs);
  option->setMgrsConversionMode(MgrsConversionMode::Automatic);
  return option;
}

CoordinateConversionOption* createUsgs(int precision, bool useSpaces, QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("USGS");
  option->setPrecision(precision);
  option->setAddSpaces(useSpaces);
  return option;
}

CoordinateConversionOption* createUtm(UtmConversionMode mode, bool useSpaces, QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("UTM");
  option->setUtmConversionMode(mode);
  option->setAddSpaces(useSpaces);
  return option;
}

CoordinateConversionOption* createGars(GarsConversionMode mode, QObject* parent)
{
  auto option = new CoordinateConversionOptions(parent);
  option->setName("GARS");
  option->setOutputMode(CoordinateConversionOptions::CoordinateType::CoordinateTypeGars);
  option->setGarsConversionMode(mode);
  return option;
}

} // Toolkit
} // ArcGISRuntime
} // Esri