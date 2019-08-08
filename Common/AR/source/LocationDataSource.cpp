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

#include "LocationDataSource.h"
#include <QGeoPositionInfoSource>

/*!
  \class Esri::ArcGISRuntime::Toolkit::LocationDataSource
  \ingroup ArcGISQtAr
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Updates the location using the positionning device.
  \sa {AR}

  LocationDataSource uses the Qt's class QGeoPositionInfoSource to get the location and the
  heading of the device, using the GPS sensor.

  LocationDataSource can be started using the function \l start and stopped using the function
  \l stop. When LocationDataSource is running, the signals \l headingChanged and \l locationChanged
  are emmitted everytime the values change.

  Most of the time, it's not necessary to create an object of this class. A default LocationDataSource
  is created automatically by \l ArcGISArSceneView when this is necessary. This class is public to
  give the possibility to override the class QGeoPositionInfoSource to support a custom GPS device.
  See https://doc.qt.io/qt-5/qtpositioning-plugins.html for Qt Positioning service plugins.

  \section1 Latitude and longitude

  This latitude and longitude parameters hold the latitude and longitude in decimal degrees
  in the WGS84 datum format.
  The property is undefined (\c NaN) if the latitude or longitude has not been set.

  A positive latitude indicates the Northern Hemisphere, and a negative latitude indicates
  the Southern Hemisphere. A positive longitude indicates the Eastern Hemisphere, and a
  negative longitude indicates the Western Hemisphere.

  To be valid, the latitude must be between -90 to 90 inclusive and the longitude must
  be between -180 to 180 inclusive.

  \section1 Altitude

  The \l altitude parameter holds the altitude in meters above sea level.
  The property is undefined (\c NaN) if the altitude has not been set.
 */

using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
LocationDataSource::LocationDataSource(QObject* parent) :
  QObject(parent)
{
}

/*!
  \brief The destructor.
 */
LocationDataSource::~LocationDataSource()
{
}

/*!
  \brief Returns \c true if the location data source is started.
 */
bool LocationDataSource::isStarted() const
{
  return m_isStarted;
}

/*!
  \brief Starts the location data source.
 */
void LocationDataSource::start()
{
  // create a default source if necessary
  if (!m_positionSource)
    setPositionSource(QGeoPositionInfoSource::createDefaultSource(this));

  // start
  m_positionSource->startUpdates();

  // update isStarted and sensorStatus properties
  m_isStarted = true;
  m_sensorStatus = SensorStatus::Started;
  emit isStartedChanged();
  emit sensorStatusChanged();
}

/*!
  \brief Stops the location data source.
 */
void LocationDataSource::stop()
{
  if (m_positionSource)
    m_positionSource->stopUpdates();

  // update isStarted and sensorStatus properties
  m_isStarted = false;
  m_sensorStatus = SensorStatus::Stopped;
  emit isStartedChanged();
  emit sensorStatusChanged();
}

/*!
  \brief Gets the sensor status.
 */
SensorStatus LocationDataSource::sensorStatus() const
{
  return m_sensorStatus;
}

/*!
  \brief Gets the position source.
 */
QGeoPositionInfoSource* LocationDataSource::positionSource() const
{
  return m_positionSource;
}

/*!
  \brief Sets the position source to \a positionSource.
 */
void LocationDataSource::setPositionSource(QGeoPositionInfoSource* positionSource)
{
  if (m_positionSource == positionSource || !positionSource)
    return;

  m_positionSource = positionSource;

  disconnect(m_positionSourceConnection);
  m_positionSourceConnection = connect(positionSource, &QGeoPositionInfoSource::positionUpdated,
                                       this, [this](const QGeoPositionInfo &update)
  {
    const QGeoCoordinate& coordinate = update.coordinate();
    if (coordinate.isValid())
      emit locationChanged(coordinate.latitude(), coordinate.longitude(), coordinate.altitude());
  });

  emit positionSourceChanged();
}

// signals

/*!
  \fn void LocationDataSource::headingChanged(double heading);
  \brief Signal emitted when the heading value changes.
 */

/*!
  \fn void LocationDataSource::isStartedChanged();
  \brief Signal emitted when the \l isStarted property changes.
 */

/*!
  \fn void LocationDataSource::locationChanged(double latitude, double longitude, double altitude);
  \brief Signal emitted when the location values change.
 */

/*!
  \fn void LocationDataSource::positionSourceChangedChanged();
  \brief Signal emitted when the \l positionSourceChanged property changes.
 */

/*!
  \fn void LocationDataSource::sensorStatusChanged();
  \brief Signal emitted when the \l sensorStatus property changes.
 */
