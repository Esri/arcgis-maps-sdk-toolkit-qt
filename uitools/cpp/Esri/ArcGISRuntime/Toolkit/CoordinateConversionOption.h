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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H

// Qt headers
#include <QObject>

// ArcGISRuntime headers
#include <GeometryTypes.h>
#include <Point.h>
#include <SpatialReference.h>

namespace Esri::ArcGISRuntime::Toolkit {

class CoordinateConversionOption : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(CoordinateType outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged)
  Q_PROPERTY(bool hasSpaces READ hasSpaces WRITE setHasSpaces NOTIFY hasSpacesChanged)
  Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
  Q_PROPERTY(int decimalPlaces READ decimalPlaces WRITE setDecimalPlaces NOTIFY decimalPlacesChanged)
  Q_PROPERTY(MgrsConversionMode mgrsConversionMode READ mgrsConversionMode WRITE setMgrsConversionMode NOTIFY mgrsConversionModeChanged)
  Q_PROPERTY(LatitudeLongitudeFormat latLonFormat READ latLonFormat WRITE setLatLonFormat NOTIFY latLonFormatChanged)
  Q_PROPERTY(UtmConversionMode utmConversionMode READ utmConversionMode WRITE setUtmConversionMode NOTIFY utmConversionModeChanged)
  Q_PROPERTY(GarsConversionMode garsConversionMode READ garsConversionMode WRITE setGarsConversionMode NOTIFY garsConversionModeChanged)
  Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
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

  QString prettyPrint(const Point& point) const;

  Point pointFromString(const QString& point,
                        const SpatialReference& spatialReference);

  QString name() const;

  void setName(const QString& name);

  CoordinateType outputMode() const;

  void setOutputMode(CoordinateType outputMode);

  bool hasSpaces() const;

  void setHasSpaces(bool hasSpaces);

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

  GarsConversionMode garsConversionMode() const;

  void setGarsConversionMode(GarsConversionMode conversionMode);

signals:
  void nameChanged();

  void outputModeChanged();

  void hasSpacesChanged();

  void precisionChanged();

  void decimalPlacesChanged();

  void mgrsConversionModeChanged();

  void latLonFormatChanged();

  void utmConversionModeChanged();

  void garsConversionModeChanged();

private:
  QString m_name;
  CoordinateType m_outputMode = CoordinateType::Usng;
  bool m_hasSpaces = true;
  int m_precision = 8;
  int m_decimalPlaces = 6;
  MgrsConversionMode m_mgrsConversionMode = MgrsConversionMode::Automatic;
  LatitudeLongitudeFormat m_latLonFormat = LatitudeLongitudeFormat::DecimalDegrees;
  UtmConversionMode m_utmConversionMode = UtmConversionMode::LatitudeBandIndicators;
  GarsConversionMode m_garsConvesrionMode = GarsConversionMode::Center;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H
