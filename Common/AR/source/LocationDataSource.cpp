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

using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \class Esri::ArcGISRuntime::Toolkit::LocationDataSource
  \ingroup ArcGISQtToolkitAR
  \ingroup ArcGISQtToolkitARCppApi
  \ingroup ArcGISQtToolkitARQmlApi
  \ingroup ArcGISQtToolkit
  \ingroup ArcGISQtToolkitCppApi
  \ingroup ArcGISQtToolkitQmlApi
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

  See also {http://doc.qt.io/qt-5/qtpositioning-plugins.html}{Qt Positioning service plugins}.

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

  \section1 Tracking Mode

  The tracking mode can be \c {LocationTrackingode::Ignore}{ignore} (the location data are ignored),
  \c {LocationTrackingode::Initial}{initial} (only the first location data are used) or
  \c {LocationTrackingode::Continuous}{continuous} (all the location data are used).

  If the tracking mode is not \c {LocationTrackingode::Ignore}{ignore} and the user didn't provide
  \l QGeoPositionInfoSource and \l QCompass objects, default ones are created with the
  \l LocationDataSource as parent. If one of these objects is provided, the \l LocationDataSource
  doesn't take ownership of the object.
 */


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
  // Do nothing if the tracking is already started.
  if (m_isStarted)
    return;

  // Update objects and connections.
  updateObjectsAndConnections();

  // Start sensors
  m_geoPositionSource->startUpdates();
  m_compass->start();

  // update isStarted and sensorStatus properties
  m_sensorStatus = ArEnums::SensorStatus::Starting;
  emit sensorStatusChanged();
}

/*!
  \brief Starts the location data source with a location tracking mode.
 */
void LocationDataSource::start(ArEnums::LocationTrackingMode locationTrackingMode)
{
  // Do nothing if the tracking is already started.
  if (m_isStarted)
    return;

  // Update the tracking mode.
  setLocationTrackingMode(locationTrackingMode);

  // Start the tracking
  start();
}

/*!
  \brief Stops the location data source.
 */
void LocationDataSource::stop()
{
  // Do nothing if the tracking is not started.
  if (!m_isStarted)
    return;

  // Stop sensors.
  if (m_geoPositionSource)
    m_geoPositionSource->stopUpdates();

  if (m_compass)
    m_compass->stop();

  // Disconnect signals.
  disconnect(m_geoPositionSourceConnection);
  disconnect(m_compassConnection);

  // Update isStarted and sensorStatus properties.
  m_isStarted = false;
  m_sensorStatus = ArEnums::SensorStatus::Stopped;
  emit isStartedChanged();
  emit sensorStatusChanged();
}

/*!
  \brief Gets the sensor status.
 */
ArEnums::SensorStatus LocationDataSource::sensorStatus() const
{
  return m_sensorStatus;
}

/*!
  \brief Gets the location tracking mode.
  \sa LocationTrackingMode
 */
ArEnums::LocationTrackingMode LocationDataSource::locationTrackingMode() const
{
  return m_locationTrackingMode;
}

/*!
  \brief Sets the location tracking mode to \a locationTrackingMode.

  The location tracking mode cannot be changed after tracking is started.

  \sa LocationTrackingMode
 */
void LocationDataSource::setLocationTrackingMode(ArEnums::LocationTrackingMode locationTrackingMode)
{
  if (m_locationTrackingMode == locationTrackingMode)
    return;

  m_locationTrackingMode = locationTrackingMode;

  if (m_isStarted)
    updateObjectsAndConnections();

  emit locationTrackingModeChanged();
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

  The geoposition source cannot be changed after tracking is started.
 */
void LocationDataSource::setGeoPositionSource(QGeoPositionInfoSource* geoPositionSource)
{
  if (m_isStarted || m_geoPositionSource == geoPositionSource)
    return;

  m_geoPositionSource = geoPositionSource;
  emit geoPositionSourceChanged();
}

/*!
  \brief Gets the compass object.
 */
QCompass* LocationDataSource::compass() const
{
  return m_compass;
}

/*!
  \brief Sets the compass to \a compass.

  The compass cannot be changed after tracking is started.
 */
void LocationDataSource::setCompass(QCompass* compass)
{
  if (m_isStarted || m_compass == compass)
    return;

  m_compass = compass;
  emit compassChanged();
}

/*!
  \internal
  Connect or disconnect the geoPositionSource and compass signals.
  If locationTrackingMode is "Continuous", the connection still alive until stopped.
  If locationTrackingMode is "Initial", the connection is disconnected after the first signal
  is emitted.
 */
void LocationDataSource::updateObjectsAndConnections()
{
  // The geoPositionSource and compass objects can be created by the user and
  // can be still active when the pointers are set to null. Therefore, the connections
  // must be disconnected manually.
  disconnect(m_geoPositionSourceConnection);
  disconnect(m_compassConnection);

  if (m_locationTrackingMode == ArEnums::LocationTrackingMode::Ignore)
    return;

  // If necessary, create a default geoposition source and compass.
  if (!m_geoPositionSource)
    setGeoPositionSource(QGeoPositionInfoSource::createDefaultSource(this));

  if (!m_compass)
    setCompass(new QCompass(this));

  // Connect the geoPositionSource.
  m_geoPositionSourceConnection = connect(m_geoPositionSource, &QGeoPositionInfoSource::positionUpdated,
                                          this, [this](const QGeoPositionInfo& positionInfo)
  {
    // Emit the new position if available.
    QGeoCoordinate coordinate = positionInfo.coordinate();
    if (coordinate.isValid())
      emit locationChanged(coordinate.latitude(), coordinate.longitude(), coordinate.altitude());

    // Update sensor status
    if (m_sensorStatus == ArEnums::SensorStatus::Starting)
    {
      m_isStarted = true;
      m_sensorStatus = ArEnums::SensorStatus::Started;
      emit isStartedChanged();
      emit sensorStatusChanged();
    }
    
    // Disconnect the signal if the location tracking mode is Initial.
    if (m_locationTrackingMode == ArEnums::LocationTrackingMode::Initial)
      disconnect(m_geoPositionSourceConnection);
  });

  // Connect the QCompass.
  m_compassConnection = connect(m_compass, &QCompass::readingChanged, this, [this]()
  {
    // emit the new heading if available
    QCompassReading* reading = m_compass->reading();
    Q_CHECK_PTR(reading);
    emit headingChanged(reading->azimuth());

    // Update sensor status
    if (m_sensorStatus == ArEnums::SensorStatus::Starting)
    {
      m_isStarted = true;
      m_sensorStatus = ArEnums::SensorStatus::Started;
      emit isStartedChanged();
      emit sensorStatusChanged();
    }
    
    // Disconnect the signal if the location tracking mode is Initial.
    if (m_locationTrackingMode == ArEnums::LocationTrackingMode::Initial)
      disconnect(m_compassConnection);
  });
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
  \fn void LocationDataSource::geoPositionSourceChanged();
  \brief Signal emitted when the \l geoPositionSource property changes.
 */

/*!
  \fn void LocationDataSource::compassChanged();
  \brief Signal emitted when the \l compass property changes.
 */

/*!
  \fn void LocationDataSource::sensorStatusChanged();
  \brief Signal emitted when the \l sensorStatus property changes.
 */

/*!
  \fn void LocationDataSource::locationTrackingModeChanged();
  \brief Signal emitted when the \l locationTrackingMode property changes.
 */
