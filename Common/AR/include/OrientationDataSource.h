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

#ifndef OrientationDataSource_H
#define OrientationDataSource_H

#include <QObject>
#include "SensorStatus.h"

class QRotationSensor;

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

class OrientationDataSource : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isStarted READ isStarted NOTIFY isStartedChanged)
  Q_PROPERTY(QRotationSensor* rotationSensor READ rotationSensor WRITE setRotationSensor NOTIFY rotationSensorChanged)
  Q_PROPERTY(SensorStatus sensorStatus READ sensorStatus NOTIFY sensorStatusChanged)

public:
  explicit OrientationDataSource(QObject* parent = nullptr);
  ~OrientationDataSource() override;

  bool isStarted() const;

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  SensorStatus sensorStatus() const;

  QRotationSensor* rotationSensor() const;
  void setRotationSensor(QRotationSensor* rotationSensor);

signals:
  void isStartedChanged();
  void orientationChanged(double quaternionX, double quaternionY, double quaternionZ, double quaternionW);
  void rotationSensorChanged();
  void sensorStatusChanged();

private:
  QRotationSensor* m_rotationSensor = nullptr;
  bool m_isStarted = false;
  SensorStatus m_sensorStatus = SensorStatus::Stopped;
  QMetaObject::Connection m_rotationSensorConnection;
};

} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace

#endif // OrientationDataSource_H
