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
  \brief ...
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArViewInterface::ArcGISArViewInterface(QQuickItem* parent):
  ArcGISArViewInterface(0, parent)
{
}

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArViewInterface::ArcGISArViewInterface(int /*renderVideoFeed*/, QQuickItem* parent):
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
  \brief ...
 */
double ArcGISArViewInterface::translationTransformationFactor() const
{
  return 0.0;
}

/*!
  \brief ...
 */
void ArcGISArViewInterface::setTranslationTransformationFactor(double /*translationTransformationFactor*/)
{
}

/*!
  \brief ...
 */
bool ArcGISArViewInterface::renderVideoFeed() const
{
  return false;
}

/*!
  \brief ...
 */
void ArcGISArViewInterface::setRenderVideoFeed(bool /*renderVideoFeed*/)
{
}

/*!
  \brief ...
 */
bool ArcGISArViewInterface::tracking() const
{
  return m_timerId != 0;
}

/*!
  \brief ...
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
  \brief ...
 */
void ArcGISArViewInterface::resetTracking()
{
}

/*!
  \brief ...
 */
bool ArcGISArViewInterface::resetUsingLocationService()
{
  return false;
}

/*!
  \brief ...
 */
void ArcGISArViewInterface::resetUsingSpacialAnchor()
{
}

/*!
  \brief ...
 */
void ArcGISArViewInterface::startTracking()
{
  m_arWrapper->startTracking();

  // TODO: hack to run a timer on Android. This timer will be removed in the future.
#ifdef Q_OS_ANDROID
  m_timerId = startTimer(50);
#endif
}

/*!
  \brief ...
 */
void ArcGISArViewInterface::stopTracking()
{
  m_arWrapper->stopTracking();
  killTimer(m_timerId);
  m_timerId = 0;
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
 */
void ArcGISArViewInterface::timerEvent(QTimerEvent* /*event*/)
{
  // render the frame buffer object
  update();

  // update the scene view camera
  updateCamera();
}

/*!
  \brief ...
 */
QQuickFramebufferObject::Renderer* ArcGISArViewInterface::createRenderer() const
{
  if (m_arViewRenderer)
    return m_arViewRenderer;

  m_arViewRenderer = new ArcGISArViewRenderer;
  m_arViewRenderer->setArWrapper(m_arWrapper.get());
  return m_arViewRenderer;
}

/*!
  \brief ...
 */
template<typename ArRawPtr>
ArRawPtr* ArcGISArViewInterface::getAR() const // crash. Use enable_if to improve the error message?
                         // "can't be casted to X type, only to X, Y, Z types on Android"
{}

#ifdef Q_OS_IOS

//template<>
//ARSession* ArcGISArViewInterface::getAR<ARSession>() const
//{
//  Q_CHECK_PTR(m_arWrapper);
//  return m_arWrapper->session();
//}

#elif defined Q_OS_ANDROID

template<>
ArSession* ArcGISArViewInterface::getAR<ArSession>() const
{
  Q_CHECK_PTR(m_arWrapper);
  // if (!m_arWrapper->initialized()) return nullptr; // useful?
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
