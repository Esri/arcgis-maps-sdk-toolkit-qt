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

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

// design changes:
// - add sensorStatus property + getter
// - add heading, latitude and longitude propreties + getters
// - add positionSource property + getter/setter

class /*TOOLKIT_EXPORT*/ LocationDataSource : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isStarted READ isStarted NOTIFY isStartedChanged)
  Q_PROPERTY(SensorStatus sensorStatus READ sensorStatus NOTIFY sensorStatusChanged)
  Q_PROPERTY(double heading READ heading NOTIFY headingChanged)
  Q_PROPERTY(double latitude READ latitude NOTIFY locationChanged)
  Q_PROPERTY(double longitude READ longitude NOTIFY locationChanged)
  Q_PROPERTY(QGeoPositionInfoSource* positionSource READ positionSource WRITE setPositionSource NOTIFY positionSourceChanged)

public:
  explicit LocationDataSource(QObject* parent = nullptr);
  ~LocationDataSource() override;

  bool isStarted() const;

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  void updateHeading(double heading);
  void updateLocation(/*Location*/ double location);

  // specific to Qt API?
  SensorStatus sensorStatus() const;
  double heading() const;
  double latitude() const;
  double longitude() const;

  QGeoPositionInfoSource* positionSource() const; // can be useful to use QNmeaPositionInfoSource or custom source
  void setPositionSource(QGeoPositionInfoSource* positionSource);

signals:
  void isStartedChanged();
  void sensorStatusChanged();
  void headingChanged(double heading);
  void locationChanged(double latitude, double longitude);
  void positionSourceChanged();

private:
  QGeoPositionInfoSource* m_positionSource = nullptr;
  bool m_isStarted = false;
  SensorStatus m_sensorStatus = SensorStatus::Stopped;
  double m_heading = 0.0;
  double m_latitude = 0.0;
  double m_longitude = 0.0;
  QMetaObject::Connection m_positionSourceConnection;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // LocationDataSource_H
