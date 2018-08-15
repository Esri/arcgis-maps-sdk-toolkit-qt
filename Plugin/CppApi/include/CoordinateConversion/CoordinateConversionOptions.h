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

#ifndef COORDINATECONVERSIONOPTIONS_H
#define COORDINATECONVERSIONOPTIONS_H

#include <QObject>
#include <QStringList>

#include "ToolkitCommon.h"
#include "GeometryTypes.h"

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
  void garsConversionModeChanged();

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

  GarsConversionMode garsConvesrionMode() const;
  void setGarsConversionMode(GarsConversionMode conversionMode);

private:
  QString m_name;
  CoordinateType m_outputMode = CoordinateTypeUsng;
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

Q_DECLARE_METATYPE(Esri::ArcGISRuntime::Toolkit::CoordinateConversionOptions::CoordinateType)

#endif // COORDINATECONVERSIONOPTIONS_H
