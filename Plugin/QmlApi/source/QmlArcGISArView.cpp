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

#include "QmlArcGISArView.h"

/*!
  \class Esri::ArcGISRuntime::Toolkit::QmlArcGISArView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Render and tracks the camera.
 */

using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
QmlArcGISArView::QmlArcGISArView(QQuickItem* parent):
  ArcGISArViewInterface(parent)
{
}

/*!
   \brief The destructor.
 */
QmlArcGISArView::~QmlArcGISArView()
{
}

/*!
  \qmlproperty Camera ArcGISArView::originCamera
  \brief Returns the origin camera.
 */
QObject* QmlArcGISArView::originCamera() const
{
  return m_originCamera;
}

void QmlArcGISArView::setOriginCamera(QObject* originCamera)
{
  if (m_originCamera == originCamera || !assertClassName(originCamera, "QmlCamera"))
    return;

  m_originCamera = originCamera;

  // set origin camera to tmcc if available
  if (m_tmcc)
    m_tmcc->setProperty("originCamera", QVariant::fromValue(m_originCamera));

  emit originCameraChanged();
}

/*!
  \qmlproperty SceneView ArcGISArView::sceneView
  \brief Returns the scene view.

  The space effect of the scene view is set to \c SpaceEffect::Transparent
  and the atmosphere effect is set to \c AtmosphereEffect::None.
 */
QObject* QmlArcGISArView::sceneView() const
{
  return m_sceneView;
}

void QmlArcGISArView::setSceneView(QObject* sceneView)
{
  if (!sceneView || m_sceneView || !assertClassName(sceneView, "QmlSceneView"))
    return;

  m_sceneView = sceneView;

  // set properties
  m_sceneView->setProperty("spaceEffect", 1); // SpaceEffect::Transparent
  m_sceneView->setProperty("atmosphereEffect", 0); // AtmosphereEffect::None
  m_sceneView->setProperty("manualRendering", true);

  if (m_tmcc)
    m_sceneView->setProperty("cameraController", QVariant::fromValue(m_tmcc));

  emit sceneViewChanged();
}

/*!
  \internal
 */
QObject* QmlArcGISArView::transformationMatrixCameraController() const
{
  return m_tmcc;
}

void QmlArcGISArView::setTransformationMatrixCameraController(QObject* tmcc)
{
  if (!tmcc || m_tmcc || !assertClassName(tmcc, "QmlTransformationMatrixCameraController"))
    return;

  m_tmcc = tmcc;

  // set TMCC to scene view
  if (m_sceneView)
    m_sceneView->setProperty("cameraController", QVariant::fromValue(m_tmcc));

  emit transformationMatrixCameraControllerChanged();
}

/*!
  \brief Sets the initial transformation used to offset the originCamera.

  The initial transformation is based on an AR point determined via existing plane hit detection
  from `screenPoint`. If an AR point cannot be determined, the initial transformation is the
  identity matrix.

  \list
    \li \a x - The x-coordinate of the screen point to determine the `initialTransformation` from.
    \li \a y - The y-coordinate of the screen point to determine the `initialTransformation` from.
  \endlist
 */
void QmlArcGISArView::setInitialTransformation(float x, float y)
{
  // Use the `hitTestInternal` method to get the matrix of `screenPoint`.
  const std::array<double, 7> hitResult = hitTestInternal(x, y);
  // quaternionW can never be 0, this indicates an error occurred
  if (hitResult[3] == 0)
    return;

  emit initialTransformationChanged(0.0, 0.0, 0.0, 1.0, hitResult[4], hitResult[5], hitResult[6]);
}

/*!
  \qmlmethod Point ArcGISArView::screenToLocation(Point screenPoint)
  \brief Gets the location in the real world space corresponding to the screen point.
*/
QObject* QmlArcGISArView::screenToLocation(QObject* screenPoint) const
{
  if (!m_sceneView || !assertClassName(screenPoint, "QmlPoint"))
    return nullptr;

  // gets the position of the screen point
  bool isOk = false;
  const double x = screenPoint->property("x").toDouble(&isOk);
  if (!isOk)
    return nullptr;

  const double y = screenPoint->property("y").toDouble(&isOk);
  if (!isOk)
    return nullptr;

  // gets the current viewpoint camera
  QObject* currentViewpointCamera = m_sceneView->property("currentViewpointCamera").value<QObject*>();
  if (!currentViewpointCamera)
    return nullptr;

  // converts the point from the screen space to the 3D space
  const std::array<double, 7> hitResult = ArcGISArViewInterface::hitTestInternal(x, y);

  // calculates the matrix transformations and creates the QmlPoint location.
  QVariant location;
  QMetaObject::invokeMethod(m_qmlWrapper, "screenToLocation", Qt::DirectConnection,
                            Q_RETURN_ARG(QVariant, location),
                            Q_ARG(QVariant, QVariant::fromValue(currentViewpointCamera)),
                            Q_ARG(QVariant, hitResult[0]),
                            Q_ARG(QVariant, hitResult[1]),
                            Q_ARG(QVariant, hitResult[2]),
                            Q_ARG(QVariant, hitResult[3]),
                            Q_ARG(QVariant, hitResult[4]),
                            Q_ARG(QVariant, hitResult[5]),
                            Q_ARG(QVariant, hitResult[6]));
  return location.value<QObject*>();
}

/*!
  \internal
 */
void QmlArcGISArView::setTransformationMatrixInternal(
    double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
    double translationX, double translationY, double translationZ)
{
  emit transformationMatrixChanged(quaternionX, quaternionY, quaternionZ, quaternionW,
                                   translationX, translationY, translationZ);
}

/*!
  \internal
 */
void QmlArcGISArView::setFieldOfViewInternal(double xFocalLength, double yFocalLength,
                                             double xPrincipal, double yPrincipal,
                                             double xImageSize, double yImageSize)
{
  emit fieldOfViewChanged(xFocalLength, yFocalLength,
                          xPrincipal, yPrincipal,
                          xImageSize, yImageSize);
}

/*!
  \internal
 */
void QmlArcGISArView::renderFrameInternal()
{
  emit renderFrame();
}

/*!
  \internal
 */
void QmlArcGISArView::setTranslationFactorInternal(double)
{
  // do nothing, translationFactorChanged signal is emitted in ArcGISArViewInterface::setTranslationFactor
}

/*!
  \internal
 */
void QmlArcGISArView::setLocationInternal(double latitude, double longitude, double altitude)
{
  emit locationChanged(latitude, longitude, altitude);
}

/*!
  \internal
 */
void QmlArcGISArView::setHeadingInternal(double heading)
{
  emit headingChanged(heading);
}

/*!
  \internal

  Resets the device tracking and related properties.
 */
void QmlArcGISArView::resetTrackingInternal()
{
  // to implmente
}

/*!
  \brief Verify than \a object is of the type expressed in \a className.
  \internal

  This method exists because the QML API headers are not available, so it's not possible to use the concret
  types in the QmlArcGISArView API. Then, this API uses QObject* and this function is used to verify than
  the type of the objects passed in the parameters are correct (using the Qt's meta-object).
 */
bool QmlArcGISArView::assertClassName(QObject* object, const QString& className) const
{
  return object && object->metaObject()->className() == className;
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
