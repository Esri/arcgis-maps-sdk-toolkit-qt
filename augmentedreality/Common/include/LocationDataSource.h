/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

#ifndef LocationDataSource_H
#define LocationDataSource_H

#include <QObject>
#include "ArEnums.h"

class QCompass;
class QGeoPositionInfoSource;

Q_MOC_INCLUDE(QCompass)
Q_MOC_INCLUDE("QGeoPositionInfoSource")

namespace Esri::ArcGISRuntime::Toolkit {

class LocationDataSource : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QGeoPositionInfoSource* geoPositionSource READ geoPositionSource
             WRITE setGeoPositionSource NOTIFY geoPositionSourceChanged)
  Q_PROPERTY(QCompass* compass READ compass WRITE setCompass NOTIFY compassChanged)
  Q_PROPERTY(bool started READ isStarted NOTIFY isStartedChanged)
  Q_PROPERTY(ArEnums::SensorStatus sensorStatus READ sensorStatus NOTIFY sensorStatusChanged)
  Q_PROPERTY(ArEnums::LocationTrackingMode locationTrackingMode READ locationTrackingMode
             WRITE setLocationTrackingMode NOTIFY locationTrackingModeChanged)

public:
  explicit LocationDataSource(QObject* parent = nullptr);
  ~LocationDataSource() override;

  // properties
  QGeoPositionInfoSource* geoPositionSource() const;
  void setGeoPositionSource(QGeoPositionInfoSource* geoPositionSource);

  QCompass* compass() const;
  void setCompass(QCompass* compass);

  bool isStarted() const;
  ArEnums::SensorStatus sensorStatus() const;

  ArEnums::LocationTrackingMode locationTrackingMode() const;
  void setLocationTrackingMode(ArEnums::LocationTrackingMode locationTrackingMode);

  // invokable methods
  Q_INVOKABLE void start();
  Q_INVOKABLE void start(ArEnums::LocationTrackingMode locationTrackingMode);
  Q_INVOKABLE void stop();

signals:
  void geoPositionSourceChanged();
  void compassChanged();

  void locationChanged(double latitude, double longitude, double altitude);
  void headingChanged(double heading);

  void isStartedChanged();
  void sensorStatusChanged();

  void locationTrackingModeChanged();

private:
  Q_DISABLE_COPY(LocationDataSource)

  void updateObjectsAndConnections();

  QGeoPositionInfoSource* m_geoPositionSource = nullptr;
  QCompass* m_compass = nullptr;

  QMetaObject::Connection m_geoPositionSourceConnection;
  QMetaObject::Connection m_compassConnection;

  bool m_isStarted = false;
  ArEnums::SensorStatus m_sensorStatus = ArEnums::SensorStatus::Stopped;

  ArEnums::LocationTrackingMode m_locationTrackingMode = ArEnums::LocationTrackingMode::Initial;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // LocationDataSource_H
