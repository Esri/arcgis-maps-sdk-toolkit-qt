// Copyright 2016 ESRI
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
#include "CoordinateConversionController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Solutions
{

using MgrsConversionMode = CoordinateConversionOptions::MgrsConversionMode;
using LatitudeLongitudeFormat = CoordinateConversionOptions::LatitudeLongitudeFormat;
using UtmConversionMode = CoordinateConversionOptions::UtmConversionMode;

CoordinateConversionOptions::CoordinateConversionOptions(QObject* parent):
  QObject(parent)
{
}

CoordinateConversionOptions::~CoordinateConversionOptions()
{
}

CoordinateConversionOptions::CoordinateType CoordinateConversionOptions::outputMode() const
{
  return m_outputMode;
}

void CoordinateConversionOptions::setOutputMode(CoordinateType outputMode)
{
  m_outputMode = outputMode;
  emit outputModeChanged();
}

QString CoordinateConversionOptions::name() const
{
  return m_name;
}

void CoordinateConversionOptions::setName(const QString& name)
{
  m_name = name;
  emit nameChanged();
}

bool CoordinateConversionOptions::addSpaces() const
{
  return m_addSpaces;
}

void CoordinateConversionOptions::setAddSpaces(bool addSpaces)
{
  m_addSpaces = addSpaces;
  emit addSpacesChanged();
}

int CoordinateConversionOptions::precision() const
{
  return m_precision;
}

void CoordinateConversionOptions::setPrecision(int precision)
{
  m_precision = precision;
  emit precisionChanged();
}

int CoordinateConversionOptions::decimalPlaces() const
{
  return m_decimalPlaces;
}

void CoordinateConversionOptions::setDecimalPlaces(int decimalPlaces)
{
  m_decimalPlaces = decimalPlaces;
  emit decimalPlacesChanged();
}

MgrsConversionMode CoordinateConversionOptions::mgrsConversionMode() const
{
  return m_mgrsConversionMode;
}

void CoordinateConversionOptions::setMgrsConversionMode(MgrsConversionMode mgrsConversionMode)
{
  m_mgrsConversionMode = mgrsConversionMode;
  emit mgrsConversionModeChanged();
}

LatitudeLongitudeFormat CoordinateConversionOptions::latLonFormat() const
{
  return m_latLonFormat;
}

void CoordinateConversionOptions::setLatLonFormat(LatitudeLongitudeFormat latLonFormat)
{
  m_latLonFormat = latLonFormat;
  emit latLonFormatChanged();
}

UtmConversionMode CoordinateConversionOptions::utmConversionMode() const
{
  return m_utmConversionMode;
}

void CoordinateConversionOptions::setUtmConversionMode(UtmConversionMode utmConversionMode)
{
  m_utmConversionMode = utmConversionMode;
  emit utmConversionModeChanged();
}

void CoordinateConversionOptions::listAppend(QQmlListProperty<CoordinateConversionOptions>* property, CoordinateConversionOptions* value)
{
  static_cast<CoordinateConversionController*>(property->object)->addOption(value);
}

CoordinateConversionOptions* CoordinateConversionOptions::listAt(QQmlListProperty<CoordinateConversionOptions>* property, int index)
{
  return static_cast<QList<CoordinateConversionOptions*>*>(property->data)->value(index);
}

int CoordinateConversionOptions::listCount(QQmlListProperty<CoordinateConversionOptions>* property)
{
  return static_cast<QList<CoordinateConversionOptions*>*>(property->data)->count();
}

void CoordinateConversionOptions::listClear(QQmlListProperty<CoordinateConversionOptions>* property)
{
  static_cast<CoordinateConversionController*>(property->object)->clearOptions();
}

CoordinateConversionOptions::CoordinateType CoordinateConversionOptions::stringToCoordinateType(const QString& type) const
{
  if (type == "Gars")
    return CoordinateType::CoordinateTypeGars;
  else if (type == "GeoRef")
    return CoordinateType::CoordinateTypeGeoRef;
  else if (type == "LatLon")
    return CoordinateType::CoordinateTypeLatLon;
  else if (type == "Mgrs")
    return CoordinateType::CoordinateTypeMgrs;
  else if (type == "Usng")
    return CoordinateType::CoordinateTypeUsng;
  else if (type == "Utm")
    return CoordinateType::CoordinateTypeUtm;

  return CoordinateType::CoordinateTypeLatLon;
}

QString CoordinateConversionOptions::coordinateTypeToString(CoordinateType type) const
{
  switch (type)
  {
  case CoordinateType::CoordinateTypeGars:
    return "Gars";
  case CoordinateType::CoordinateTypeGeoRef:
    return "GeoRef";
  case CoordinateType::CoordinateTypeLatLon:
    return "LatLon";
  case CoordinateType::CoordinateTypeMgrs:
    return "Mgrs";
  case CoordinateType::CoordinateTypeUsng:
    return "Usng";
  case CoordinateType::CoordinateTypeUtm:
    return "Utm";
  default: {}
  }

  return QString();
}

QStringList CoordinateConversionOptions::coordinateTypeNames() const
{
  return QStringList() << QStringLiteral("Gars")
                       << QStringLiteral("GeoRef")
                       << QStringLiteral("LatLon")
                       << QStringLiteral("Mgrs")
                       << QStringLiteral("Usng")
                       << QStringLiteral("Utm");
}

} // Solutions
} // ArcGISRuntime
} // Esri
