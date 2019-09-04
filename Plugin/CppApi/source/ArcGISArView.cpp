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

#include "ArcGISArView.h"
#include "TransformationMatrix.h"
#include "TransformationMatrixCameraController.h"
#include <QQuickWindow>
#include <QScreen>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISArView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Render and tracks the camera.
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;
using namespace Esri::ArcGISRuntime::Toolkit::Internal;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArView::ArcGISArView(QQuickItem* parent):
  ArcGISArViewInterface(parent),
  m_tmcc(new TransformationMatrixCameraController(this)),
  m_initialTransformation(TransformationMatrix::createIdentityMatrix(this))
{
  connect(m_tmcc, &TransformationMatrixCameraController::originCameraChanged, this, &ArcGISArView::originCameraChanged);
}

/*!
  \brief A constructor that accepts an optional \a parent.

  \list
  \li \a renderVideoFeed - Not implemented.
  \li \a tryUsingArKit - Not implemented.
  \li \a parent - optional.
  \endlist
 */
ArcGISArView::ArcGISArView(bool renderVideoFeed, bool tryUsingArKit, QQuickItem* parent):
  ArcGISArViewInterface(renderVideoFeed, tryUsingArKit, parent),
  m_tmcc(new TransformationMatrixCameraController(this))
{
  connect(m_tmcc, &TransformationMatrixCameraController::originCameraChanged, this, &ArcGISArView::originCameraChanged);
}

/*!
   \brief The destructor.
 */
ArcGISArView::~ArcGISArView()
{
}

/*!
  \brief Gets the origin camera.
 */
Camera ArcGISArView::originCamera() const
{
  return Camera(m_originCamera);
}

/*!
  \brief Sets the origin camera to \a originCamera.
 */
void ArcGISArView::setOriginCamera(const Camera& originCamera)
{
  if (m_originCamera == originCamera)
    return;

  m_originCamera = originCamera;
  m_tmcc->setOriginCamera(originCamera);
  // don't emit originCameraChanged, this signal is emited by the core runtime

  // If we're using ARKit, reset its tracking.
//  if isUsingARKit {
//      arSCNView.session.run(arConfiguration, options: .resetTracking)
//  }
//  resetTracking();
}

/*!
  \brief Gets the scene view.
 */
SceneQuickView* ArcGISArView::sceneView() const
{
  return m_sceneView;
}

/*!
  \brief Sets the scene view to \a sceneView.

  The space effect of the scene view is set to \c SpaceEffect::Transparent
  and the atmosphere effect is set to \c AtmosphereEffect::None.
 */
void ArcGISArView::setSceneView(SceneQuickView* sceneView)
{
  if (sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  m_sceneView->setSpaceEffect(SpaceEffect::Transparent);
  m_sceneView->setAtmosphereEffect(AtmosphereEffect::None);
  m_sceneView->setManualRendering(true);
  m_sceneView->setCameraController(m_tmcc);

  connect(m_sceneView, &SceneQuickView::touched, this, [this](QTouchEvent& event)
  {
    setInitialTransformation(event.touchPoints().first().lastScreenPos().toPoint());
  });

  emit sceneViewChanged();
}

/*!
  \brief Sets the initial transformation used to offset the originCamera.

  The initial transformation is based on an AR point determined via existing plane hit detection
  from `screenPoint`. If an AR point cannot be determined, this method will return `false`.

  \list
    \li \a screenPoint - The screen point to determine the `initialTransformation` from.
  \endlist
 */
void ArcGISArView::setInitialTransformation(const QPoint& screenPoint)
{
  // Use the `internalHitTest` method to get the matrix of `screenPoint`.
  const std::array<double, 7> hitResult = internalHitTest(screenPoint.x(), screenPoint.y());
  if (hitResult[0] == 0) // todo: improve the return value (bool?)
    return;

  // Set the `initialTransformation` as the AGSTransformationMatrix.identity - hit test matrix.
  const auto hitMatrix = std::unique_ptr<TransformationMatrix>(
        TransformationMatrix::createWithQuaternionAndTranslation(0, 0, 0, 1,hitResult[4], hitResult[5], hitResult[6]));
  Q_CHECK_PTR(hitMatrix.get());

  auto identity = std::unique_ptr<TransformationMatrix>(TransformationMatrix::createIdentityMatrix());
  Q_CHECK_PTR(identity);

  m_initialTransformation = identity->subtractTransformation(hitMatrix.get(), this);
  Q_CHECK_PTR(m_initialTransformation);
}

/*!
  \brief Gets the location in the real world space corresponding to the screen point.
 */
Point ArcGISArView::screenToLocation(const Point& screenPoint) const
{
  if (!m_sceneView)
    return Point();

  const std::array<double, 7> hitResult = internalHitTest(screenPoint.x(), screenPoint.y());
  if (hitResult[0] == 0)
    return Point();

  auto hitMatrix = std::unique_ptr<TransformationMatrix>(
        TransformationMatrix::createWithQuaternionAndTranslation(
          hitResult[0], hitResult[1], hitResult[2], hitResult[3], hitResult[4], hitResult[5], hitResult[6]));

  auto currentViewpointMatrix = std::unique_ptr<TransformationMatrix>(
        m_sceneView->currentViewpointCamera().transformationMatrix());

  auto finalMatrix = std::unique_ptr<TransformationMatrix>(currentViewpointMatrix->addTransformation(hitMatrix.get()));
  return Camera(finalMatrix.get()).location();
}

/*!
  \internal
 */
void ArcGISArView::setTransformationMatrixInternal(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                                   double translationX, double translationY, double translationZ)
{
  auto matrix = std::unique_ptr<TransformationMatrix>(TransformationMatrix::createWithQuaternionAndTranslation(
                                                        quaternionX, quaternionY, quaternionZ, quaternionW,
                                                        translationX, translationY, translationZ));
  Q_CHECK_PTR(matrix.get());

  Q_CHECK_PTR(m_initialTransformation);
  auto finalMatrix = std::unique_ptr<TransformationMatrix>(m_initialTransformation->addTransformation(matrix.get()));

  Q_CHECK_PTR(m_tmcc);
  m_tmcc->setTransformationMatrix(finalMatrix.get());
}

/*!
  \internal
 */
void ArcGISArView::setFieldOfViewInternal(double xFocalLength, double yFocalLength,
                                          double xPrincipal, double yPrincipal,
                                          double xImageSize, double yImageSize)
{
  if (!m_sceneView)
    return;

  // get the screen orientation
  const Qt::ScreenOrientations orientation = window()->screen()->orientation();
  DeviceOrientation deviceOrientation;

  switch (orientation) {
    case Qt::PortraitOrientation:
      deviceOrientation = DeviceOrientation::Portrait;
      break;
    case Qt::LandscapeOrientation:
      deviceOrientation = DeviceOrientation::LandscapeRight;
      break;
    case Qt::InvertedPortraitOrientation:
      deviceOrientation = DeviceOrientation::ReversePortrait;
      break;
    case Qt::InvertedLandscapeOrientation:
      deviceOrientation = DeviceOrientation::LandscapeLeft;
      break;
    default:
      deviceOrientation = DeviceOrientation::Portrait;
      break;
  }

  // set the field of view
  m_sceneView->setFieldOfViewFromLensIntrinsics(xFocalLength, yFocalLength, xPrincipal, yPrincipal,
                                                xImageSize, yImageSize, deviceOrientation);
}

/*!
  \internal
 */
void ArcGISArView::renderFrameInternal()
{
  if (!m_sceneView)
    return;

  m_sceneView->renderFrame();
}

/*!
  \internal
 */
void ArcGISArView::setTranslationFactorInternal(double translationFactor)
{
  m_tmcc->setTranslationFactor(translationFactor);
}

/*!
  \internal
 */
void ArcGISArView::setLocationInternal(double latitude, double longitude, double altitude)
{
  if (m_tmcc->originCamera().isEmpty())
  {
    // create a new origin camera
    m_tmcc->setOriginCamera(Camera(latitude, longitude, altitude+100, 0.0, 90.0, 0.0));
  }
  else
  {
    // update the origin camera
    const Camera oldCamera = m_tmcc->originCamera();
    m_tmcc->setOriginCamera(Camera(latitude, longitude, altitude+100, oldCamera.heading(), oldCamera.pitch(), oldCamera.roll()));
  }

  // todo: Reset the camera controller's transformationMatrix to its initial state, the Identity matrix.
//  cameraController.transformationMatrix = .identity
}

/*!
  \internal

  Resets the device tracking and related properties.
 */
void ArcGISArView::resetTrackingInternal()
{
  setOriginCamera(Camera());

  m_initialTransformation = TransformationMatrix::createIdentityMatrix(this);

  m_tmcc->setTransformationMatrix(m_initialTransformation);
}

// signals

/*!
  \fn void ArcGISArView::originCameraChanged();
  \brief Signal emitted when the \l originCamera property changes.
 */

/*!
  \fn void ArcGISArView::sceneViewChanged();
  \brief Signal emitted when the \l sceneView property changes.
 */

