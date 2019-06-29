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

#include "ArcGISArView.h"
#include "TransformationMatrix.h"

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISArView
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
ArcGISArView::ArcGISArView(QQuickItem* parent):
  ArcGISArView(0, parent)
{
}

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArView::ArcGISArView(int /*renderVideoFeed*/, QQuickItem* parent):
  QQuickFramebufferObject(parent),
  m_arWrapper(new ArWrapper(this))
{
  setFlag(ItemHasContents, true);
}

/*!
   \brief The destructor.
 */
ArcGISArView::~ArcGISArView()
{
}

/*!
  \brief ...
 */
Camera ArcGISArView::originCamera() const
{
  return Camera(m_originCamera);
}

/*!
  \brief ...
 */
void ArcGISArView::setOriginCamera(const Camera& originCamera)
{
  if (m_originCamera == originCamera)
    return;

  m_originCamera = originCamera;
  emit originCameraChanged();
}

/*!
  \brief ...
 */
SceneQuickView* ArcGISArView::sceneView() const
{
  return m_sceneView;
}

/*!
  \brief ...
 */
void ArcGISArView::setSceneView(SceneQuickView* sceneView)
{
  if (sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  // m_sceneView->setSpaceEffect(SpaceEffect::Transparent);
  m_sceneView->setBackgroundTransparent(true);
  m_sceneView->setAtmosphereEffect(AtmosphereEffect::None);
  m_sceneView->setParent(this);
  emit sceneViewChanged();

  startTracking();
}

/*!
  \brief ...
 */
double ArcGISArView::translationTransformationFactor() const
{
  return 0.0;
}

/*!
  \brief ...
 */
void ArcGISArView::setTranslationTransformationFactor(double /*translationTransformationFactor*/)
{
}

/*!
  \brief ...
 */
bool ArcGISArView::renderVideoFeed() const
{
  return false;
}

/*!
  \brief ...
 */
void ArcGISArView::setRenderVideoFeed(bool /*renderVideoFeed*/)
{
}

/*!
  \brief ...
 */
bool ArcGISArView::tracking() const
{
  return m_timerId != 0;
}

/*!
  \brief ...
 */
void ArcGISArView::setTracking(bool tracking)
{
  if (ArcGISArView::tracking() == tracking)
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
Point ArcGISArView::arScreenToLocation(const Point& /*screenPoint*/) const
{
  return Point();
}

/*!
  \brief ...
 */
void ArcGISArView::resetTracking()
{
}

/*!
  \brief ...
 */
bool ArcGISArView::resetUsingLocationService()
{
  return false;
}

/*!
  \brief ...
 */
void ArcGISArView::resetUsingSpacialAnchor()
{
}

/*!
  \brief ...
 */
void ArcGISArView::startTracking()
{
  qDebug() << "== start tracking";
  m_arWrapper->startTracking();
//  m_timerId = startTimer(50);
}

/*!
  \brief ...
 */
void ArcGISArView::stopTracking()
{
  m_arWrapper->stopTracking();
  killTimer(m_timerId);
  m_timerId = 0;
}

/*!
  \internal
 */
void ArcGISArView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
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
void ArcGISArView::timerEvent(QTimerEvent* /*event*/)
{
  // render the frame buffer object
  update();

  // update the scene view camera
  updateCamera();
}

/*!
  \brief ...
 */
QQuickFramebufferObject::Renderer* ArcGISArView::createRenderer() const
{
  if (m_arViewRenderer)
    return m_arViewRenderer;

  m_arViewRenderer = new ArcGISArViewRenderer;
  m_arViewRenderer->setArWrapper(m_arWrapper.get());
  return m_arViewRenderer;
}

void ArcGISArView::updateCamera()
{
  if (m_originCamera.isEmpty())
  {
    Camera camera = m_sceneView->currentViewpointCamera();
    m_originCamera = camera; // Camera(camera.location(), camera.heading(), 90, 0);
  }

  qDebug() << "---->>> update camera -----";
  TransformationMatrix matrix = m_originCamera.transformationMatrix().addTransformation(m_arWrapper->transformationMatrix());
  m_sceneView->setViewpointCamera(Camera(matrix));
}

/*!
  \brief ...
 */
template<typename ArRawPtr>
ArRawPtr* ArcGISArView::getAR() const // crash. Use enable_if to improve the error message?
                         // "can't be casted to X type, only to X, Y, Z types on Android"
{}

#ifdef Q_OS_IOS

//template<>
//ARSession* ArcGISArView::getAR<ARSession>() const
//{
//  Q_CHECK_PTR(m_arWrapper);
//  return m_arWrapper->session();
//}

#elif defined Q_OS_ANDROID

template<>
ArSession* ArcGISArView::getAR<ArSession>() const
{
  Q_CHECK_PTR(m_arWrapper);
  // if (!m_arWrapper->initialized()) return nullptr; // useful?
  return m_arWrapper->session();
}

#endif

// signals

/*!
  \fn void ArcGISArView::originCameraChanged();
  \brief Signal emitted when the \l originCamera property changes.
 */

/*!
  \fn void ArcGISArView::translationTransformationFactorChanged();
  \brief Signal emitted when the \l translationTransformationFactor property changes.
 */

/*!
  \fn void ArcGISArView::renderVideoFeedChanged();
  \brief Signal emitted when the \l renderVideoFeed property changes.
 */
