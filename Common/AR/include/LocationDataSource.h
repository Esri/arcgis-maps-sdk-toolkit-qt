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
#include "SensorStatus.h"

class QGeoPositionInfoSource;

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class LocationDataSource : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isStarted READ isStarted NOTIFY isStartedChanged)
  Q_PROPERTY(QGeoPositionInfoSource* positionSource READ positionSource WRITE setPositionSource NOTIFY positionSourceChanged)
  Q_PROPERTY(SensorStatus sensorStatus READ sensorStatus NOTIFY sensorStatusChanged)

public:
  explicit LocationDataSource(QObject* parent = nullptr);
  ~LocationDataSource() override;

  // properties
  bool isStarted() const;

  QGeoPositionInfoSource* positionSource() const;
  void setPositionSource(QGeoPositionInfoSource* positionSource);

  SensorStatus sensorStatus() const;

  // invokable methods
  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

signals:
  void headingChanged(double heading);
  void isStartedChanged();
  void locationChanged(double latitude, double longitude, double altitude);
  void positionSourceChanged();
  void sensorStatusChanged();

private:
  bool m_isStarted = false;
  QGeoPositionInfoSource* m_positionSource = nullptr;
  QMetaObject::Connection m_positionSourceConnection;
  SensorStatus m_sensorStatus = SensorStatus::Stopped;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // LocationDataSource_H
