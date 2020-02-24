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
#ifndef COORDINATECONVERSIONOPTION_H
#define COORDINATECONVERSIONOPTION_H

// Qt headers
#include <QObject>

// ArcGISRuntime headers
#include <GeometryTypes.h>

namespace Esri
{
namespace ArcGISRuntime
{

// Forward declaration.
class Point;

namespace Toolkit
{

class CoordinateConversionOption : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(CoordinateType outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged)
  Q_PROPERTY(bool addSpaces READ addSpaces WRITE setAddSpaces NOTIFY addSpacesChanged)
  Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
  Q_PROPERTY(int decimalPlaces READ decimalPlaces WRITE setDecimalPlaces NOTIFY decimalPlacesChanged)
  Q_PROPERTY(MgrsConversionMode mgrsConversionMode READ mgrsConversionMode WRITE setMgrsConversionMode NOTIFY mgrsConversionModeChanged)
  Q_PROPERTY(LatitudeLongitudeFormat latLonFormat READ latLonFormat WRITE setLatLonFormat NOTIFY latLonFormatChanged)
  Q_PROPERTY(UtmConversionMode utmConversionMode READ utmConversionMode WRITE setUtmConversionMode NOTIFY utmConversionModeChanged)

public:
  enum CoordinateType
  {
    Gars,
    GeoRef,
    LatLon,
    Mgrs,
    Usng,
    Utm
  };
  Q_ENUM(CoordinateType)

public:
  Q_INVOKABLE CoordinateConversionOption(QObject* parent = nullptr);
  ~CoordinateConversionOption() override;

  QString name() const;
  void setName(const QString& name);
  Q_SIGNAL void nameChanged();

  CoordinateType outputMode() const;
  void setOutputMode(CoordinateType outputMode);
  Q_SIGNAL void outputModeChanged();

  bool addSpaces() const;
  void setAddSpaces(bool addSpaces);
  Q_SIGNAL void addSpacesChanged();

  int precision() const;
  void setPrecision(int precision);
  Q_SIGNAL void precisionChanged();

  int decimalPlaces() const;
  void setDecimalPlaces(int decimalPlaces);
  Q_SIGNAL void decimalPlacesChanged();

  MgrsConversionMode mgrsConversionMode() const;
  void setMgrsConversionMode(MgrsConversionMode mgrsConversionMode);
  Q_SIGNAL void mgrsConversionModeChanged();

  LatitudeLongitudeFormat latLonFormat() const;
  void setLatLonFormat(LatitudeLongitudeFormat latLonFormat);
  Q_SIGNAL void latLonFormatChanged();

  UtmConversionMode utmConversionMode() const;
  void setUtmConversionMode(UtmConversionMode utmConversionMode);
  Q_SIGNAL void utmConversionModeChanged();

  GarsConversionMode garsConvesrionMode() const;
  void setGarsConversionMode(GarsConversionMode conversionMode);
  Q_SIGNAL void garsConversionModeChanged();

  Q_INVOKABLE QString prettyPrint(const Point& point) const;

  Q_INVOKABLE Point pointFromString(const QString& point, const SpatialReference& spatialReference);

private:
  QString m_name;
  CoordinateType m_outputMode = CoordinateType::Usng;
  bool m_addSpaces = true;
  int m_precision = 8;
  int m_decimalPlaces = 6;

  MgrsConversionMode m_mgrsConversionMode = MgrsConversionMode::Automatic;
  LatitudeLongitudeFormat m_latLonFormat = LatitudeLongitudeFormat::DecimalDegrees;
  UtmConversionMode m_utmConversionMode = UtmConversionMode::LatitudeBandIndicators;
  GarsConversionMode m_garsConvesrionMode = GarsConversionMode::Center;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONOPTION_H
