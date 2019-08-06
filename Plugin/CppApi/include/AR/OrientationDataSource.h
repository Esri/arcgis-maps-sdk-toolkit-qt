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

class QRotationSensor; // QSensor?

namespace Esri
{
namespace ArcGISRuntime
{

// deisgn changes:
// -

namespace Toolkit
{

class /*TOOLKIT_EXPORT*/ OrientationDataSource : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool isStarted READ isStarted NOTIFY isStartedChanged)
  // getters for orientation? And for sensor status?

public:
  explicit OrientationDataSource(QObject* parent = nullptr);
  ~OrientationDataSource() override;

  bool isStarted() const;

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  void updateOrientation(double quaternionX, double quaternionY, double quaternionZ, double quaternionW); // not "quaternion" as designed

  // specific to Qt API?
  SensorStatus sensorStatus() const;
//  double quaternionX() const;
//  double quaternionY() const;
//  double quaternionZ() const;
//  double quaternionW() const;

  QRotationSensor* rotationSensor() const;
  void setRotationSensor(QRotationSensor* rotationSensor);

signals:
  void isStartedChanged();
  void sensorStatusChanged();
  void orientationChanged(double quaternionX, double quaternionY, double quaternionZ, double quaternionW);

  // specific to Qt API?
  void rotationSensorChanged();

private:
  QRotationSensor* m_rotationSensor = nullptr;
  bool m_isStarted = false;
  SensorStatus m_sensorStatus = SensorStatus::Stopped;
  QMetaObject::Connection m_rotationSensorConnection;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // OrientationDataSource_H
