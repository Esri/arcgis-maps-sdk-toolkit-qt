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
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

//Notes for iOS:
//  On iOS, starting from version 8, Core Location framework requires additional entries in the application's Info.plist with keys NSLocationAlwaysUsageDescription or NSLocationWhenInUseUsageDescription and a string to be displayed in the authorization prompt. The key NSLocationWhenInUseUsageDescription is used when requesting permission to use location services while the app is in the foreground. The key NSLocationAlwaysUsageDescription is used when requesting permission to use location services whenever the app is running (both the foreground and the background). If both entries are defined, NSLocationWhenInUseUsageDescription has a priority in the foreground mode.

// to do:
// - add heading
// - add sensor status changed (error from sensor?)
// - add QGeoPositionInfo::HorizontalAccuracy and GeoPositionInfo::VerticalAccuracy?

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

bool LocationDataSource::isStarted() const
{
  return m_isStarted;
}

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

void LocationDataSource::updateHeading(double heading)
{
  // not implemented
}

void LocationDataSource::updateLocation(/*Location*/ double location)
{
  // not implemented
}

SensorStatus LocationDataSource::sensorStatus() const
{
  return m_sensorStatus;
}

double LocationDataSource::heading() const
{
  return m_heading;
}

double LocationDataSource::latitude() const
{
  return m_latitude;
}

double LocationDataSource::longitude() const
{
  return m_longitude;
}

QGeoPositionInfoSource* LocationDataSource::positionSource() const
{
  return m_positionSource;
}

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
    m_latitude = coordinate.latitude();
    m_longitude = coordinate.longitude();
    emit locationChanged(coordinate.latitude(), coordinate.longitude());
  });

// connect to  void error(QGeoPositionInfoSource::Error positioningError)?

  emit positionSourceChanged();
}

// signals

/*!
  \fn void LocationDataSource::isStartedChanged();
  \brief Signal emitted when the \l isStarted property changes.
 */

/*!
  \fn void LocationDataSource::headingChanged();
  \brief Signal emitted when the \l heading property changes.
 */

/*!
  \fn void LocationDataSource::locationChanged();
  \brief Signal emitted when the \l location property changes.
 */

/*!
  \fn void LocationDataSource::sensorStatusChanged();
  \brief Signal emitted when the \l sensorStatus property changes.
 */
