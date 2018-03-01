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

#ifndef COORDINATECONVERSIONOPTIONS_H
#define COORDINATECONVERSIONOPTIONS_H

#include <QObject>
#include <QStringList>

#include "ToolkitCommon.h"
#include "GeometryTypes.h"
#include <QQmlListProperty>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class TOOLKIT_EXPORT CoordinateConversionOptions : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(CoordinateType outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged)
  Q_PROPERTY(bool addSpaces READ addSpaces WRITE setAddSpaces NOTIFY addSpacesChanged)
  Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
  Q_PROPERTY(int decimalPlaces READ decimalPlaces WRITE setDecimalPlaces NOTIFY decimalPlacesChanged)

  Q_PROPERTY(MgrsConversionMode mgrsConversionMode READ mgrsConversionMode WRITE setMgrsConversionMode NOTIFY mgrsConversionModeChanged)
  Q_PROPERTY(LatitudeLongitudeFormat latLonFormat READ latLonFormat WRITE setLatLonFormat NOTIFY latLonFormatChanged)
  Q_PROPERTY(UtmConversionMode utmConversionMode READ utmConversionMode WRITE setUtmConversionMode NOTIFY utmConversionModeChanged)

  Q_PROPERTY(QStringList coordinateTypeNames READ coordinateTypeNames CONSTANT)

  Q_ENUMS(CoordinateType GarsConversionMode MgrsConversionMode UtmConversionMode LatitudeLongitudeFormat)

public:
  enum CoordinateType
  {
    CoordinateTypeGars,
    CoordinateTypeGeoRef,
    CoordinateTypeLatLon,
    CoordinateTypeMgrs,
    CoordinateTypeUsng,
    CoordinateTypeUtm
  };

  enum GarsConversionMode
  {
    GarsConversionModeLowerLeft = (int)Esri::ArcGISRuntime::GarsConversionMode::LowerLeft,
    GarsConversionModeCenter    = (int)Esri::ArcGISRuntime::GarsConversionMode::Center
  };

  enum MgrsConversionMode
  {
    MgrsConversionModeAutomatic      = (int)Esri::ArcGISRuntime::MgrsConversionMode::Automatic,
    MgrsConversionModeNew180InZone01 = (int)Esri::ArcGISRuntime::MgrsConversionMode::New180InZone01,
    MgrsConversionModeNew180InZone60 = (int)Esri::ArcGISRuntime::MgrsConversionMode::New180InZone60,
    MgrsConversionModeOld180InZone01 = (int)Esri::ArcGISRuntime::MgrsConversionMode::Old180InZone01,
    MgrsConversionModeOld180InZone60 = (int)Esri::ArcGISRuntime::MgrsConversionMode::Old180InZone60
  };

  enum UtmConversionMode
  {
    UtmConversionModeLatitudeBandIndicators = (int)Esri::ArcGISRuntime::UtmConversionMode::LatitudeBandIndicators,
    UtmConversionModeNorthSouthIndicators   = (int)Esri::ArcGISRuntime::UtmConversionMode::NorthSouthIndicators
  };

  enum LatitudeLongitudeFormat
  {
    LatitudeLongitudeFormatDecimalDegrees        = (int)Esri::ArcGISRuntime::LatitudeLongitudeFormat::DecimalDegrees,
    LatitudeLongitudeFormatDegreesDecimalMinutes = (int)Esri::ArcGISRuntime::LatitudeLongitudeFormat::DegreesDecimalMinutes,
    LatitudeLongitudeFormatDegreesMinutesSeconds = (int)Esri::ArcGISRuntime::LatitudeLongitudeFormat::DegreesMinutesSeconds
  };

  // static qml methods
  Q_INVOKABLE CoordinateType stringToCoordinateType(const QString& type) const;
  Q_INVOKABLE QString coordinateTypeToString(CoordinateType type) const;
  Q_INVOKABLE QStringList coordinateTypeNames() const;

signals:
  void nameChanged();
  void outputModeChanged();
  void addSpacesChanged();
  void precisionChanged();
  void decimalPlacesChanged();
  void mgrsConversionModeChanged();
  void latLonFormatChanged();
  void utmConversionModeChanged();

public:
  CoordinateConversionOptions(QObject* parent = nullptr);
  ~CoordinateConversionOptions();

  QString name() const;
  void setName(const QString& name);

  CoordinateType outputMode() const;
  void setOutputMode(CoordinateType outputMode);

  bool addSpaces() const;
  void setAddSpaces(bool addSpaces);

  int precision() const;
  void setPrecision(int precision);

  int decimalPlaces() const;
  void setDecimalPlaces(int decimalPlaces);

  MgrsConversionMode mgrsConversionMode() const;
  void setMgrsConversionMode(MgrsConversionMode mgrsConversionMode);

  LatitudeLongitudeFormat latLonFormat() const;
  void setLatLonFormat(LatitudeLongitudeFormat latLonFormat);

  UtmConversionMode utmConversionMode() const;
  void setUtmConversionMode(UtmConversionMode utmConversionMode);

  static CoordinateConversionOptions* listAt(QQmlListProperty<CoordinateConversionOptions>* property, int index);
  static int listCount(QQmlListProperty<CoordinateConversionOptions>* property);
  static void listClear(QQmlListProperty<CoordinateConversionOptions>* property);

private:
  QString m_name;
  CoordinateType m_outputMode = CoordinateTypeUsng;
  bool m_addSpaces = true;
  int m_precision = 8;
  int m_decimalPlaces = 6;

  MgrsConversionMode m_mgrsConversionMode = MgrsConversionModeAutomatic;
  LatitudeLongitudeFormat m_latLonFormat = LatitudeLongitudeFormatDecimalDegrees;
  UtmConversionMode m_utmConversionMode = UtmConversionModeLatitudeBandIndicators;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::CoordinateType)
Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::GarsConversionMode)
Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::MgrsConversionMode)
Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::UtmConversionMode)
Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::LatitudeLongitudeFormat)

#endif // COORDINATECONVERSIONOPTIONS_H
