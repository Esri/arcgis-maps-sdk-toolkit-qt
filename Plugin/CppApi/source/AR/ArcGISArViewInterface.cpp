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

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArViewInterface::ArcGISArViewInterface(QQuickItem* parent):
  ArcGISArViewInterface(false, false, parent)
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
ArcGISArViewInterface::ArcGISArViewInterface(bool /*renderVideoFeed*/, bool /*tryUsingArKit*/, QQuickItem* parent):
  QQuickFramebufferObject(parent),
  m_arWrapper(new ArWrapper(this))
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
double ArcGISArViewInterface::translationTransformationFactor() const
{
  return 0.0;
}

/*!
  \brief Not implemented.
 */
void ArcGISArViewInterface::setTranslationTransformationFactor(double /*translationTransformationFactor*/)
{
}

/*!
  \brief Not implemented.
 */
bool ArcGISArViewInterface::renderVideoFeed() const
{
  return false;
}

/*!
  \brief Not implemented.
 */
void ArcGISArViewInterface::setRenderVideoFeed(bool /*renderVideoFeed*/)
{
}

/*!
  \brief Retuns true if the AR is tracking.
 */
bool ArcGISArViewInterface::tracking() const
{
  return m_timerId != 0;
}

/*!
  \brief Starts or stops AR tracking.
 */
void ArcGISArViewInterface::setTracking(bool tracking)
{
  if (ArcGISArViewInterface::tracking() == tracking)
    return;

  if (tracking)
    startTracking();
  else
    stopTracking();

  emit trackingChanged();
}

/*!
  \brief No implemented.
 */
void ArcGISArViewInterface::resetTracking()
{
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

  // TODO: hack to run a timer on Android. This timer will be removed in the future.
#ifdef Q_OS_ANDROID
  m_timerId = startTimer(50);
#endif
}

/*!
  \brief Stops AR tracking.
 */
void ArcGISArViewInterface::stopTracking()
{
  Q_CHECK_PTR(m_arWrapper);
  m_arWrapper->stopTracking();

  // TODO: hack to run a timer on Android. This timer will be removed in the future.
#ifdef Q_OS_ANDROID
  killTimer(m_timerId);
  m_timerId = 0;
#endif
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
  \internal

   Hack to run a timer on Android. This timer will be removed in the future.
 */
void ArcGISArViewInterface::timerEvent(QTimerEvent* /*event*/)
{
  // render the frame buffer object
  update();

  // update the scene view camera
  updateCamera(0, 0, 0, 0, 0, 0, 0);
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

#endif

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
