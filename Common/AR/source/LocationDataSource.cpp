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
#include <QCompass>

/*!
  \class Esri::ArcGISRuntime::Toolkit::LocationDataSource
  \ingroup ArcGISQtAr
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Updates the location using the positioning device.

  LocationDataSource uses the Qt class QGeoPositionInfoSource to get the location and the
  heading of the device, using the GPS sensor.

  LocationDataSource can be started using the function \l start and stopped using the function
  \l stop. When LocationDataSource is running, the signals \l headingChanged and \l locationChanged
  are emitted every time the values change.

  Most of the time, it's not necessary to create an object of this class. A default LocationDataSource
  is created automatically by \l ArcGISArSceneView when this is necessary. This class is public to
  give the possibility to override the class QGeoPositionInfoSource to support a custom GPS device.

  \sa {http://doc.qt.io/qt-5/qtpositioning-plugins.html}{Qt Positioning service plugins}.

  \section1 Latitude and longitude

  The latitude and longitude parameters hold the latitude and longitude in decimal degrees
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

  \section1 Heading

  The azimuth of the device as degrees from magnetic north in a clockwise direction based on the
  top of the device, as defined by QScreen::nativeOrientation.

  \sa {http://doc.qt.io/qt-5/qcompass.html}{QCompass}
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
  // create a default geo position source and compass if necessary
  if (!m_geoPositionSource)
    setGeoPositionSource(QGeoPositionInfoSource::createDefaultSource(this));

  if (!m_compass)
    setCompass(new QCompass(this));

  // start
  m_geoPositionSource->startUpdates();
  m_compass->start();

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
  if (m_geoPositionSource)
    m_geoPositionSource->stopUpdates();

  if (m_compass)
    m_compass->stop();

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
  \brief Gets the \l QGeoPositionInfoSource object.
 */
QGeoPositionInfoSource* LocationDataSource::geoPositionSource() const
{
  return m_geoPositionSource;
}

/*!
  \brief Sets the position source to \a geoPositionSource.
 */
void LocationDataSource::setGeoPositionSource(QGeoPositionInfoSource* geoPositionSource)
{
  if (m_geoPositionSource == geoPositionSource || !geoPositionSource)
    return;

  m_geoPositionSource = geoPositionSource;

  disconnect(m_geoPositionSourceConnection);
  m_geoPositionSourceConnection = connect(m_geoPositionSource, &QGeoPositionInfoSource::positionUpdated,
                                          this, [this](const QGeoPositionInfo &positionInfo)
  {
    // emit the new position if available
    const QGeoCoordinate& coordinate = positionInfo.coordinate();
    if (coordinate.isValid())
      emit locationChanged(coordinate.latitude(), coordinate.longitude(), coordinate.altitude());
  });

  emit geoPositionSourceChanged();
}

/*!
  \brief Gets the \l QCompass object.
 */
QCompass* LocationDataSource::compass() const
{
  return m_compass;
}

/*!
  \brief Sets the compass to \a compass.
 */
void LocationDataSource::setCompass(QCompass* compass)
{
  if (m_compass == compass || !compass)
    return;

  m_compass = compass;

  disconnect(m_compassConnection);
  m_compassConnection = connect(m_compass, &QCompass::readingChanged, this, [this]()
  {
    // emit the new heading if available
    QCompassReading* reading = m_compass->reading();
    Q_CHECK_PTR(reading);
    emit headingChanged(reading->azimuth());
  });

  emit compassChanged();
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
  \fn void LocationDataSource::geoPositionSourceChangedChanged();
  \brief Signal emitted when the \l geoPositionSourceChanged property changes.
 */

/*!
  \fn void LocationDataSource::compassChanged();
  \brief Signal emitted when the \l compass property changes.
 */

/*!
  \fn void LocationDataSource::sensorStatusChanged();
  \brief Signal emitted when the \l sensorStatus property changes.
 */
