/*******************************************************************************
 *  Copyright 2012-2018 Esri
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
#include "OrientationDataSource.h"
#include <QQuickWindow>
#include <QScreen>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISArViewInterface
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Base class to impemente AR view.
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

// notes:
// - if ArKit/ArCore are disable or if tryUsingArKit is false, try to use Location and orientation data sources
//   for positionning and device orientation
// todo:
// - if ArKit/ArCore are disable, there is no ARFrame to render. Need fallback to use QCamera.

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArViewInterface::ArcGISArViewInterface(QQuickItem* parent):
  ArcGISArViewInterface(true, true, parent)
{
}

/*!
  \brief A constructor that accepts an optional \a parent.

  \list
  \li \a renderVideoFeed - Not implemented.
  \li \a tryUsingArKit - Not implemented.
  \li \a parent - optional.
  \endlist
 */
ArcGISArViewInterface::ArcGISArViewInterface(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent):
  QQuickFramebufferObject(parent),
  m_arWrapper(new ArWrapper(this)),
  m_renderVideoFeed(renderVideoFeed),
  m_tryUsingArKit(tryUsingArKit)
{
  setFlag(ItemHasContents, true);
}

/*!
   \brief The destructor.
 */
ArcGISArViewInterface::~ArcGISArViewInterface()
{
}

/*!
  \brief Not implemented.
 */
double ArcGISArViewInterface::translationFactor() const
{
  return m_translationFactor;
}

/*!
  \brief Not implemented.
 */
void ArcGISArViewInterface::setTranslationFactor(double translationFactor)
{
  if (m_translationFactor == translationFactor)
    return;

  m_translationFactor = translationFactor;
  emit translationFactorChanged();
}

/*!
  \brief Not implemented.
 */
bool ArcGISArViewInterface::renderVideoFeed() const
{
  return m_renderVideoFeed;
}

/*!
  \brief Not implemented.
 */
void ArcGISArViewInterface::setRenderVideoFeed(bool renderVideoFeed)
{
  if (m_renderVideoFeed == renderVideoFeed)
    return;

  m_renderVideoFeed = renderVideoFeed;
  emit renderVideoFeedChanged();
}

/*!
  \brief Not implemented.
 */
bool ArcGISArViewInterface::tryUsingArKit() const
{
  return m_tryUsingArKit;
}

/*!
  \brief Not implemented.
 */
void ArcGISArViewInterface::setTryUsingArKit(bool tryUsingArKit)
{
  if (m_tryUsingArKit == tryUsingArKit)
    return;

  m_tryUsingArKit = tryUsingArKit;
  emit tryUsingArKitChanged();
}

/*!
  \brief Retuns true if the AR is tracking.
 */
bool ArcGISArViewInterface::tracking() const
{
  return m_tracking;
}

/*!
  \brief Starts or stops AR tracking.
 */
void ArcGISArViewInterface::setTracking(bool tracking)
{
  if (m_tracking == tracking)
    return;

  if (tracking)
    startTracking();
  else
    stopTracking();

  emit trackingChanged();
}

// sensors
LocationDataSource* ArcGISArViewInterface::locationDataSource() const
{
  return m_locationDataSource;
}

void ArcGISArViewInterface::setLocationDataSource(LocationDataSource* locationDataSource)
{
  if (m_locationDataSource == locationDataSource)
    return;

  m_locationDataSource = locationDataSource;
  // connections

  m_locationDataSourceConnection = connect(m_locationDataSource, &LocationDataSource::locationChanged,
                                           this, [this](double latitude, double longitude)
  {
    // APIs specific, uses updateCamera?
    updateCamera(0, 0, 0, 0, latitude, longitude, 0);
//    const Camera camera = m_sceneView->currentViewpointCamera().move(Point(latitude, longitude));
//    m_sceneView->setViewpointCamera(camera)
  });

  emit locationDataSourceChanged();
}

OrientationDataSource* ArcGISArViewInterface::orientationDataSource() const
{
  return m_orientationDataSource;
}

void ArcGISArViewInterface::setOrientationDataSource(OrientationDataSource* orientationDataSource)
{
  if (m_orientationDataSource == orientationDataSource)
    return;

  m_orientationDataSource = orientationDataSource;

  m_orientationDataSourceConnection =
      connect(m_orientationDataSource, &OrientationDataSource::orientationChanged,
              this, [this](double quaternionX, double quaternionY, double quaternionZ, double quaternionW)
  {
    // APIs specific, uses updateCamera?
    updateCamera(quaternionX, quaternionY, quaternionZ, quaternionW, 0, 0, 0);
//        let finalQuat:simd_quatd = simd_mul(simd_quatd(orientation: UIDevice.current.orientation), orientation)
//        let transformationMatrix = AGSTransformationMatrix(quaternionX: finalQuat.vector.x,
//                                                           quaternionY: finalQuat.vector.y,
//                                                           quaternionZ: finalQuat.vector.z,
//                                                           quaternionW: finalQuat.vector.w,
//                                                           translationX: 0.0,
//                                                           translationY: 0.0,
//                                                           translationZ: 0.0)
//        cameraController.transformationMatrix = transformationMatrix
  });

  emit orientationDataSourceChanged();
}

/*!
  \brief No implemented.
 */
void ArcGISArViewInterface::resetTracking()
{
  // m_initialLocation = nullptr;
  m_arWrapper->resetTracking();
}

/*!
  \brief No implemented.
 */
bool ArcGISArViewInterface::resetUsingLocationService()
{
  return false;
}

/*!
  \brief No implemented.
 */
void ArcGISArViewInterface::resetUsingSpacialAnchor()
{
}

/*!
  \brief Starts AR tracking.
 */
void ArcGISArViewInterface::startTracking()
{
  Q_CHECK_PTR(m_arWrapper);
  m_arWrapper->startTracking();
  m_tracking = true;

  // uses locationDataSource and orientation data sources
  if (!m_tryUsingArKit)
  {
    if (!m_locationDataSource)
      setLocationDataSource(new LocationDataSource(this));

    if (!m_orientationDataSource)
      setOrientationDataSource(new OrientationDataSource(this));

    m_locationDataSource->start();
    m_orientationDataSource->start();
  }

  // enable detection of orientation changes.
  window()->screen()->setOrientationUpdateMask(Qt::LandscapeOrientation	| Qt::PortraitOrientation |
                                               Qt::InvertedLandscapeOrientation | Qt::InvertedPortraitOrientation);
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

  if (m_orientationDataSource)
    m_orientationDataSource->stop();

  m_tracking = false;
}

/*!
  \internal
 */
void ArcGISArViewInterface::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
  QQuickItem::geometryChanged(newGeometry, oldGeometry);

  if (newGeometry.size() == oldGeometry.size())
    return;

  // update the geometry of the AR view
  Q_CHECK_PTR(m_arWrapper);
  m_arWrapper->setSize(newGeometry.size().toSize());
}

/*!
  \brief ...
 */
QQuickFramebufferObject::Renderer* ArcGISArViewInterface::createRenderer() const
{
  Q_CHECK_PTR(m_arWrapper);
  if (m_arViewRenderer)
    return m_arViewRenderer;

  m_arViewRenderer = new ArcGISArViewRenderer;
  m_arViewRenderer->setArWrapper(m_arWrapper.get());
  return m_arViewRenderer;
}

/*!
  \brief Returns the internal object used for AR tracking. The available objects depend on
  the plateform.

  For iOS:

  \list
  \li \a ARSession - AR session. https://developer.apple.com/documentation/arkit/arsession?language=objc
  \li \a ARConfiguration - AR configuration. https://developer.apple.com/documentation/arkit/arconfiguration?language=objc
  \li \a ArSessionDelegate - Derived class called when a new AR frame is available.
         https://developer.apple.com/documentation/arkit/arsessiondelegate?language=objc
  \endlist

  For Android:

  \list
  \li \a ArSession - AR session. https://developers.google.com/ar/reference/c/group/session
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
ArRawPtr* ArcGISArViewInterface::arRawPtr() const // crash. Use enable_if to improve the error message?
                         // "can't be casted to X type, only to X, Y, Z types on Android"
{}

#ifdef Q_OS_IOS

//template<>
//ARSession* ArcGISArViewInterface::arRawPtr<ARSession>() const
//{
//  Q_CHECK_PTR(m_arWrapper);
//  return m_arWrapper->session();
//}

//template<>
//ARConfiguration* ArcGISArViewInterface::arRawPtr<ARConfiguration>() const
//{
//  Q_CHECK_PTR(m_arWrapper);
//  return m_arWrapper->configuration();
//}

//template<>
//ArSessionDelegate* ArcGISArViewInterface::arRawPtr<ArSessionDelegate>() const
//{
//  Q_CHECK_PTR(m_arWrapper);
//  return m_arWrapper->delegate();
//}

#elif defined Q_OS_ANDROID

template<>
ArSession* ArcGISArViewInterface::arRawPtr<ArSession>() const
{
  Q_CHECK_PTR(m_arWrapper);
  return m_arWrapper->session();
}

template<>
ArFrame* ArcGISArViewInterface::arRawPtr<ArFrame>() const
{
  Q_CHECK_PTR(m_arWrapper);
  return m_arWrapper->frame();
}

#endif

std::array<double, 7> ArcGISArViewInterface::screenToLocation(int x, int y) const
{
  Q_CHECK_PTR(m_arWrapper);
  return m_arWrapper->hitTest(x, y);
}

// signals

/*!
  \fn void ArcGISArViewInterface::translationTransformationFactorChanged();
  \brief Signal emitted when the \l translationTransformationFactor property changes.
 */

/*!
  \fn void ArcGISArViewInterface::renderVideoFeedChanged();
  \brief Signal emitted when the \l renderVideoFeed property changes.
 */

/*!
  \fn void ArcGISArViewInterface::trackingChanged();
  \brief Signal emitted when the \l tracking property changes.
 */
