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

#include "OrientationDataSource.h"
#include <QRotationSensor>

/*!
  \class Esri::ArcGISRuntime::Toolkit::OrientationDataSource
  \ingroup ArcGISQtAr
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Updates the orientation device using the internal sensors.
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
OrientationDataSource::OrientationDataSource(QObject* parent) :
  QObject(parent)
{
}

/*!
   \brief The destructor.
 */
OrientationDataSource::~OrientationDataSource()
{
}

/*!
  \brief Returns \c true if the orientation data source is started.
 */
bool OrientationDataSource::isStarted() const
{
  return m_isStarted;
}

/*!
  \brief Starts the orientation data source.
 */
void OrientationDataSource::start()
{
  // create a default source if necessary
  if (!m_rotationSensor)
    setRotationSensor(new QRotationSensor(this));

  // start
  m_rotationSensor->start();

  // update isStarted and sensorStatus properties
  m_isStarted = true;
  m_sensorStatus = SensorStatus::Started;
  emit isStartedChanged();
  emit sensorStatusChanged();
}

/*!
  \brief Stops the orientation data source.
 */
void OrientationDataSource::stop()
{
  if (m_rotationSensor)
    m_rotationSensor->stop();

  // update isStarted and sensorStatus properties
  m_isStarted = false;
  m_sensorStatus = SensorStatus::Stopped;
  emit isStartedChanged();
  emit sensorStatusChanged();
}

/*!
  \brief Gets the sensor status.
 */
SensorStatus OrientationDataSource::sensorStatus() const
{
  return m_sensorStatus;
}

/*!
  \brief Gets the rotation sensor.
 */
QRotationSensor* OrientationDataSource::rotationSensor() const
{
  return m_rotationSensor;
}

/*!
  \brief Sets the rotation sensor to \a rotationSensor.
 */
void OrientationDataSource::setRotationSensor(QRotationSensor* rotationSensor)
{
  if (m_rotationSensor == rotationSensor || !rotationSensor)
    return;

  m_rotationSensor = rotationSensor;

  disconnect(m_rotationSensorConnection);
  m_rotationSensorConnection = connect(rotationSensor, &QRotationSensor::readingChanged, this, [this]()
  {
    QRotationReading* reading = m_rotationSensor->reading();
    emit orientationChanged(reading->x(), reading->y(), reading->z(), 1.0);
  });

  emit rotationSensorChanged();
}

// signals

/*!
  \fn void OrientationDataSource::isStartedChanged();
  \brief Signal emitted when the \l isStarted property changes.
 */

/*!
  \fn void OrientationDataSource::orientationChanged(double quaternionX, double quaternionY, double quaternionZ, double quaternionW);
  \brief Signal emitted when the \l orientation property changes.
 */

/*!
  \fn void OrientationDataSource::rotationSensorChanged();
  \brief Signal emitted when the \l rotationSensor property changes.
 */

/*!
  \fn void OrientationDataSource::sensorStatusChanged();
  \brief Signal emitted when the \l sensorStatus property changes.
 */
