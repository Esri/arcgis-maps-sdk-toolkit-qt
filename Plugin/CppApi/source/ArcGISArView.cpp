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

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;
using namespace Esri::ArcGISRuntime::Toolkit::Internal;

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISArView
  \ingroup ArcGISQtToolkitAR
  \ingroup ArcGISQtToolkitARCppApi
  \ingroup ArcGISQtToolkit
  \ingroup ArcGISQtToolkitCppApi
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief A scene view for displaying ARKit/ARCore features on mobile devices
  using the C++ API.

  The Augmented Reality (AR) toolkit provides support for ARKit for iOS and
  ArCore for Android.
  This toolkit component allows quick and easy integration
  of AR into your application for a variety of scenarios.

  See \l {https://github.com/Esri/arcgis-runtime-toolkit-qt/blob/master/Common/AR/README.md} {additional details about using the ArcGISArView toolkit component}.
 */

/*!
  \brief A constructor that accepts an optional \a parent object.
 */
ArcGISArView::ArcGISArView(QQuickItem* parent):
  ArcGISArViewInterface(parent),
  m_tmcc(new TransformationMatrixCameraController(this)),
  m_initialTransformation(TransformationMatrix::createIdentityMatrix(this))
{
  connect(m_tmcc, &TransformationMatrixCameraController::originCameraChanged, this, &ArcGISArView::originCameraChanged);
}

/*!
  \internal

  \list
  \li \a renderVideoFeed - Set to \c true to render the camera frames in the background.
  \li \a tryUsingArKit - Set to \c true to use the AR framework, depending of the platform (ARKit
  in Android and ARKit in iOS).
  \li \a parent - optional parent object.
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
  \property ArcGISArView::originCamera
  \brief Sets the origin camera of this ArcGISArView.
 */
void ArcGISArView::setOriginCamera(const Camera& originCamera)
{
  if (m_originCamera == originCamera)
    return;

  m_originCamera = originCamera;
  // Don't emit originCameraChanged, this signal is emited by the core runtime

  // Update TMCC origin camera.
  updateTmccOriginCamera();
}

/*!
  \property ArcGISArView::sceneView
  \brief Gets the SceneView associated with this ArcGISArView.
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
  \brief Sets the initial transformation used to offset the \l originCamera.

  The initial transformation is based on an AR point determined via existing plane hit detection
  from `screenPoint`. If an AR point cannot be determined, this method will return \c false.

  \list
    \li \a screenPoint - The screen point to determine the initialTransformation from.
  \endlist
 */
void ArcGISArView::setInitialTransformation(const QPoint& screenPoint)
{
  // Use the `hitTestInternal` method to get the matrix of `screenPoint`.
  const std::array<double, 7> hitResult = hitTestInternal(screenPoint.x(), screenPoint.y());
  // quaternionW can never be 0, this indicates an error occurred
  if (hitResult[3] == 0)
    return;

  // Set the `initialTransformation` as the AGSTransformationMatrix.identity - hit test matrix.
  const auto hitMatrix = std::unique_ptr<TransformationMatrix>(
        TransformationMatrix::createWithQuaternionAndTranslation(0.0, 0.0, 0.0, 1.0, hitResult[4], hitResult[5], hitResult[6]));
  Q_CHECK_PTR(hitMatrix.get());

  m_initialTransformation = TransformationMatrix::createIdentityMatrix()->subtractTransformation(hitMatrix.get(), this);
  Q_CHECK_PTR(m_initialTransformation);
}

/*!
  \brief Gets the location in the real world space corresponding to the screen
   point \a screenPoint.
 */
Point ArcGISArView::screenToLocation(const QPoint& screenPoint) const
{
  if (!m_sceneView)
    return Point();

  const std::array<double, 7> hitResult = hitTestInternal(screenPoint.x(), screenPoint.y());
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
  \brief Register the QML-creatable types provided by the AR toolkit.

  This static function registers the QML types \l ArcGISArView and
  \c LocationDataSource in the QML engine.
  This function must be called before using the QML types.
 */
void ArcGISArView::qmlRegisterTypes()
{
  qmlRegisterType<ArcGISArView>("Esri.ArcGISArToolkit", 1, 0, "ArcGISArView");
  qmlRegisterType<LocationDataSource>("Esri.ArcGISArToolkit", 1, 0, "LocationDataSource");
  qmlRegisterUncreatableType<ArEnums>("Esri.ArcGISArToolkit", 1, 0, "ArEnums", "ArEnums is not creatable.");

  // Register enum types.
  qRegisterMetaType<ArEnums::LocationTrackingMode>("ArEnums::LocationTrackingMode");
  qRegisterMetaType<ArEnums::SensorStatus>("ArEnums::SensorStatus");
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
  const DeviceOrientation deviceOrientation = toDeviceOrientation(orientation);

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
  // Save location camera parameters.
  if (m_locationCamera.isEmpty())
    m_locationCamera = Camera(latitude, longitude, altitude, 0.0, 90.0, 0.0);
  else
    m_locationCamera = Camera(latitude, longitude, altitude,
                              m_locationCamera.heading(), m_locationCamera.pitch(), m_locationCamera.roll());

  // Update TMCC origin camera.
  updateTmccOriginCamera();
}

/*!
  \internal
 */
void ArcGISArView::setHeadingInternal(double heading)
{
  const Point oldLocation = m_locationCamera.location();

  // Save location camera parameters.
  if (m_locationCamera.isEmpty())
    m_locationCamera = Camera(0.0, 0.0, 0.0, heading, 90.0, 0.0);
  else
    m_locationCamera = Camera(oldLocation.y(), oldLocation.x(), oldLocation.z(),
                              heading, m_locationCamera.pitch(), m_locationCamera.roll());

  // Update TMCC origin camera.
  updateTmccOriginCamera();
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

/*!
  \internal

  Cast from Qt's screen orientation to ArcGIS Runtime's screen orientation.
 */
DeviceOrientation ArcGISArView::toDeviceOrientation(Qt::ScreenOrientations orientation)
{
  switch (orientation)
  {
    case Qt::PortraitOrientation:
      return DeviceOrientation::Portrait;
    case Qt::LandscapeOrientation:
      return DeviceOrientation::LandscapeRight;
    case Qt::InvertedPortraitOrientation:
      return DeviceOrientation::ReversePortrait;
    case Qt::InvertedLandscapeOrientation:
      return DeviceOrientation::LandscapeLeft;
    default:
      return DeviceOrientation::Portrait;
  }
}

/*!
  \internal

  Calculate the origin camera to use in TMCC, by addition of the origin camera (wich contains
  the initial view of the scene view and calibration) and the location camera (which contains
  the GPS data).
  m_originCamera and m_locationCamera can be invalid if not set previously.
 */
void ArcGISArView::updateTmccOriginCamera() const
{
  if (m_originCamera.isEmpty())
  {
    if (!m_locationCamera.isEmpty())
      m_tmcc->setOriginCamera(m_locationCamera);
  }
  else
  {
    if (m_locationCamera.isEmpty())
    {
      m_tmcc->setOriginCamera(m_originCamera);
    }
    else
    {
      const Point oldLocation = m_locationCamera.location();
      const Point oldOriginLocation = m_originCamera.location();
      m_tmcc->setOriginCamera(Camera(oldOriginLocation.y() + oldLocation.y(),
                                     oldOriginLocation.x() + oldLocation.x(),
                                     oldOriginLocation.z() + oldLocation.z(),
                                     m_originCamera.heading() + m_locationCamera.heading(),
                                     m_originCamera.pitch() + m_locationCamera.pitch(),
                                     m_originCamera.roll() + m_locationCamera.roll()));
    }
  }
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

