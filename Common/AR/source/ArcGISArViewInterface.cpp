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

#include "ArcGISArViewInterface.h"
#include "LocationDataSource.h"
#include "ArcGISArViewRenderer.h"
#include <QGuiApplication>
#include <QQuickWindow>
#include <QScreen>
#include "ArWrapper.h"


using namespace Esri::ArcGISRuntime::Toolkit;
using namespace Esri::ArcGISRuntime::Toolkit::Internal;

/*!
  \class ArcGISArViewInterface
  \ingroup ArcGISQtToolkit
  \ingroup ArcGISQtToolkitAR
  \ingroup ArcGISQtToolkitARCppApi
  \ingroup ArcGISQtToolkitCppApi
  \since Esri::ArcGISRuntime 100.6
  \brief Base class to impement AR scene view.

  This class provides the AR features without using the C++ or QML APIs of the ArcGIS Runtime SDK for Qt.
  It is used as a base class to create two API-dependent classes: \l ArcGISArSceneView which uses the C++ API and
  \l QmlArcGISArSceneView which uses the QML API.
 */

/*!
  \brief A constructor that accepts an optional \a parent.

  The default values for the \l renderVideoFeed and \l tryUsingArKit properties are \c true.
 */
ArcGISArViewInterface::ArcGISArViewInterface(QQuickItem* parent):
  ArcGISArViewInterface(true, true, parent)
{
}

/*!
  \brief A constructor that accepts an optional \a parent.

  \list
  \li \a renderVideoFeed - Sets to \c true to render the camera frames in the background.
  \li \a tryUsingArKit - Sets to \c true to use the AR framework, depending of the platform (ARKit
  in Android and ARKit in iOS).
  \li \a parent - optional.
  \endlist
 */
ArcGISArViewInterface::ArcGISArViewInterface(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent):
  QQuickFramebufferObject(parent),
  m_renderVideoFeed(renderVideoFeed),
  m_tryUsingArKit(tryUsingArKit)
{
  // stops tracking when the app is minimized and starts when the app is active.
  connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, [this](Qt::ApplicationState state)
  {
    switch (state)
    {
      case Qt::ApplicationSuspended:
        if (m_tracking)
          stopTracking();
        break;
      case Qt::ApplicationActive:
        if (m_tracking)
          startTracking();
        break;
      default:
        // do nothing
        break;
    }
  });

  setFlag(ItemHasContents, true);
  updateTrackingSources();
}

/*!
   \brief The destructor.
 */
ArcGISArViewInterface::~ArcGISArViewInterface()
{
}

/*!
  \brief Returns \c true when the AR scene view is tracking.
 */
bool ArcGISArViewInterface::tracking() const
{
  return m_tracking;
}

/*!
  \brief Starts or stops the AR scene view tracking.
 */
void ArcGISArViewInterface::setTracking(bool tracking)
{
  if (tracking)
    startTracking();
  else
    stopTracking();
}

/*!
  \brief Returns \c true when the scene view renders the camera frames in the background.
 */
bool ArcGISArViewInterface::renderVideoFeed() const
{
  return m_renderVideoFeed;
}

/*!
  \brief Sets to \c true when the scene view renders the camera frames in the background.
 */
void ArcGISArViewInterface::setRenderVideoFeed(bool renderVideoFeed)
{
  if (m_renderVideoFeed == renderVideoFeed)
    return;

  // enable the render video in the AR wrapper or using QCamera
  if (m_arWrapper)
  {
    m_arWrapper->setRenderVideoFeed(renderVideoFeed);
  }
  else
  {
    // not implemented.
  }

  m_renderVideoFeed = renderVideoFeed;
  emit renderVideoFeedChanged();
}

/*!
  \brief The translation factor used to support a table top AR experience.
 */
double ArcGISArViewInterface::translationFactor() const
{
  return m_translationFactor;
}

/*!
  \brief The translation factor used to support a table top AR experience.

  All the translation of the device are multiplied by this factor, to have translations in
  the scene view adapted to the scene zooming.
 */
void ArcGISArViewInterface::setTranslationFactor(double translationFactor)
{
  if (m_translationFactor == translationFactor)
    return;

  // set the translationFactor to the TMCC object
  setTranslationFactorInternal(translationFactor);

  m_translationFactor = translationFactor;
  emit translationFactorChanged();
}

/*!
  \brief Returns \c true to use the AR framework, depending of the platform (ARKit
  in Android and ARKit in iOS).
 */
bool ArcGISArViewInterface::tryUsingArKit() const
{
  return m_tryUsingArKit;
}

/*!
  \brief Sets to \c true to use the AR framework, depending of the platform (ARKit
  in Android and ARKit in iOS).
 */
void ArcGISArViewInterface::setTryUsingArKit(bool tryUsingArKit)
{
  if (m_tryUsingArKit == tryUsingArKit)
    return;

  m_tryUsingArKit = tryUsingArKit;
  updateTrackingSources();
  emit tryUsingArKitChanged();
}

// sensors
/*!
  \brief Returns the \l LocationDataSource if the AR scene view uses it to update the
  location or \c nullptr is the location is updated by the AR framework.
 */
LocationDataSource* ArcGISArViewInterface::locationDataSource() const
{
  return m_locationDataSource;
}

/*!
  \brief Sets the location data source to \a locationDataSource.

  If \a locationDataSource is \c nullptr, the tracking of the \l LocationDataSource is disabled.
  If \l tryUsingArKit is \c true, the AR framework is used for the tracking, not \l LocationDataSource.
 */
void ArcGISArViewInterface::setLocationDataSource(LocationDataSource* locationDataSource)
{
  if (m_locationDataSource == locationDataSource)
    return;

  m_locationDataSource = locationDataSource;
  connect(m_locationDataSource, &LocationDataSource::locationChanged, this, &ArcGISArViewInterface::setLocationInternal);
  connect(m_locationDataSource, &LocationDataSource::headingChanged, this, &ArcGISArViewInterface::setHeadingInternal);
  emit locationDataSourceChanged();
}

/*!
  \brief Gets the tracking mode controlling how the locations generated from the location data source
  are used during AR tracking.

  The default value is \c LocationTrackingMode::Ignore.
 */
ArEnums::LocationTrackingMode ArcGISArViewInterface::locationTrackingMode() const
{
  return m_locationTrackingMode;
}

/*!
  \brief Sets the location tracking mode to \a locationTrackingMode.
 */
void ArcGISArViewInterface::setLocationTrackingMode(ArEnums::LocationTrackingMode locationTrackingMode)
{
  if (m_locationTrackingMode == locationTrackingMode)
    return;

  if (m_locationDataSource)
    m_locationDataSource->setLocationTrackingMode(locationTrackingMode);

  m_locationTrackingMode = locationTrackingMode;
  emit locationTrackingModeChanged();
}

/*!
  \brief Resets the device tracking and related properties.
 */
void ArcGISArViewInterface::resetTracking()
{
  resetTrackingInternal();
  m_arWrapper->resetTracking();
}

/*!
  \brief Starts AR tracking.
 */
void ArcGISArViewInterface::startTracking()
{
  Q_CHECK_PTR(m_arWrapper);

  // Create LocationDataSource if necessary
  if (!m_locationDataSource)
    setLocationDataSource(new LocationDataSource(this));

  // Start AR wrapper
  if (m_tryUsingArKit)
    m_arWrapper->startTracking();

  // Start location data source.
  if (m_locationDataSource)
    m_locationDataSource->start();

  m_tracking = true;
  emit trackingChanged();
}

/*!
  \brief Starts AR tracking with location tracking mode.
 */
void ArcGISArViewInterface::startTracking(ArEnums::LocationTrackingMode locationTrackingMode)
{
  // Set locationTrackingMode
  setLocationTrackingMode(locationTrackingMode);

  // Start tracking.
  startTracking();
}

/*!
  \brief Stops AR tracking.
 */
void ArcGISArViewInterface::stopTracking()
{
  Q_CHECK_PTR(m_arWrapper);
  m_arWrapper->stopTracking();

  if (m_locationDataSource)
    m_locationDataSource->stop();

  m_tracking = false;
  emit trackingChanged();
}

/*!
  \brief Gets the color of the debug point cloud.
 */
QColor ArcGISArViewInterface::pointCloudColor() const
{
  return m_arWrapper->pointCloudColor();
}

/*!
  \brief Sets the color of the debug point cloud to \a pointCloudColor.
 */
void ArcGISArViewInterface::setPointCloudColor(const QColor& pointCloudColor)
{
  if (pointCloudColor == m_arWrapper->pointCloudColor())
    return;

  m_arWrapper->setPointCloudColor(pointCloudColor);
  emit pointCloudColorChanged();
}

/*!
  \brief Gets the size of the debug point cloud.
 */
int ArcGISArViewInterface::pointCloudSize() const
{
  return m_arWrapper->pointCloudSize();
}

/*!
  \brief Sets the size of the debug point cloud to \a pointCloudSize.
 */
void ArcGISArViewInterface::setPointCloudSize(int pointCloudSize)
{
  if (pointCloudSize == m_arWrapper->pointCloudSize())
    return;

  m_arWrapper->setPointCloudSize(pointCloudSize);
  emit pointCloudSizeChanged();
}

/*!
  \brief Gets the color of the debug planes.
 */
QColor ArcGISArViewInterface::planeColor() const
{
  return m_arWrapper->planeColor();
}

/*!
  \brief Sets the color of the debug planes to \a planeColor.
 */
void ArcGISArViewInterface::setPlaneColor(const QColor& planeColor)
{
  if (planeColor == m_arWrapper->planeColor())
    return;

  m_arWrapper->setPlaneColor(planeColor);
  emit planeColorChanged();
}

/*!
  \internal
 */
void ArcGISArViewInterface::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
  // enable detection of orientation changes.
  if (window() && window()->screen())
  {
    window()->screen()->setOrientationUpdateMask(Qt::LandscapeOrientation	| Qt::PortraitOrientation |
                                                 Qt::InvertedLandscapeOrientation | Qt::InvertedPortraitOrientation);
  }

  QQuickItem::geometryChanged(newGeometry, oldGeometry);

  if (newGeometry.size() == oldGeometry.size())
    return;

  // update the geometry of the AR view
  Q_CHECK_PTR(m_arWrapper);
  m_arWrapper->setSize(newGeometry.size().toSize());
}

/*!
  \internal
 */
QQuickFramebufferObject::Renderer* ArcGISArViewInterface::createRenderer() const
{
  Q_CHECK_PTR(m_arWrapper);
  if (m_arViewRenderer)
    return m_arViewRenderer;

  m_arViewRenderer = new ArcGISArViewRenderer(m_arWrapper.get());
  return m_arViewRenderer;
}

/*!
  \internal
 */
std::array<double, 7> ArcGISArViewInterface::hitTestInternal(int x, int y) const
{
  Q_CHECK_PTR(m_arWrapper);
  return m_arWrapper->hitTest(x, y);
}

/*!
  \internal
 */
void ArcGISArViewInterface::updateTrackingSources()
{
  if (m_tryUsingArKit)
  {
    // Stop locationDataSource if it exists.
    if (m_locationDataSource)
      m_locationDataSource->stop();

    // disable the video rendering using the QCamera
    if (m_renderVideoFeed)
    {
      // not implemented.
    }

    // create the AR wrapper object if necessary
    if (!m_arWrapper)
    {
      m_arWrapper.reset(new ArWrapper(this));
      m_arWrapper->setRenderVideoFeed(m_renderVideoFeed);
    }
  }
  else
  {
    // delete the AR wrapper if necessary
    if (m_arWrapper)
    {
      m_arWrapper->stopTracking();
      m_arWrapper.reset();
    }

    // create default LocationDataSource if necessary.
    if (!m_locationDataSource)
      setLocationDataSource(new LocationDataSource(this));

    // enable the video rendering using the QCamera
    if (m_renderVideoFeed)
    {
      // not implemented.
    }
  }
}

/*!
  \fn ArRawPtr* ArcGISArViewInterface::arRawPtr() const;
  \brief Returns the internal object used for AR tracking. The available objects depend on
  the platform. Using an invalid template argument will cause a link error. The pointer returned
  by this function can be \c nullptr.

  \section1 On iOS

  \list
  \li \a ARSession - The current AR session object. https://developer.apple.com/documentation/arkit/arsession?language=objc
  \li \a ARConfiguration - The AR configuration object used to run the AR session.
  https://developer.apple.com/documentation/arkit/arconfiguration?language=objc
  \li \a NSObject<ARSessionDelegate> - The session delegate which received the updates.
  https://developer.apple.com/documentation/arkit/arsessiondelegate?language=objc
  \endlist

  \section1 On Android

  \list
  \li \a ArSession - The current AR session object. https://developers.google.com/ar/reference/c/group/session
  \li \a ArFrame - The last received AR frame object. https://developers.google.com/ar/reference/c/group/frame
  \li \a ArCamera - The last received AR camera object. https://developers.google.com/ar/reference/c/group/camera
  \endlist

  \code
  ARSession* arSession = arcGISArView->getAR<ARSession>();
  if (arSession)
  {
    // use AR session
  }
  \endcode
 */
template<typename ArRawPtr>
ArRawPtr* ArcGISArViewInterface::arRawPtr() const
{
  return m_arWrapper->arRawPtr<ArRawPtr>();
}

// signals

/*!
  \fn void ArcGISArViewInterface::translationFactorChanged();
  \brief Signal emitted when the \l translationFactor property changes.
 */

/*!
  \fn void ArcGISArViewInterface::renderVideoFeedChanged();
  \brief Signal emitted when the \l renderVideoFeed property changes.
 */

/*!
  \fn void ArcGISArViewInterface::trackingChanged();
  \brief Signal emitted when the \l tracking property changes.
 */
