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
  \qmltype ArcGISArView
  \instantiates QmlArcGISArView
  \inherits ArcGISArViewInterface
  \ingroup ArcGISQtToolkitAR
  \ingroup ArcGISQtToolkitARQmlApi
  \ingroup ArcGISQtToolkit
  \ingroup ArcGISQtToolkitQmlApi
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief A scene view for displaying ARKit/ARCore features on mobile devices
   using the QML API.

  The Augmented Reality toolkit provides support for ARKit for iOS and Android.
  The Augmented Reality (AR) toolkit component allows quick and easy integration
  of AR into your application for a variety of scenarios.

  See AR.md for details.
 */

using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \internal
*/
QmlArcGISArView::QmlArcGISArView(QQuickItem* parent):
  ArcGISArViewInterface(parent)
{
}

/*!
  \internal
*/
QmlArcGISArView::~QmlArcGISArView()
{
}

/*!
  \qmlproperty Camera ArcGISArView::originCamera
  \brief The origin camera of the scene.
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
  emit originCameraChanged();
}

/*!
  \qmlproperty SceneView ArcGISArView::sceneView
  \brief The ArcGIS scene view used to display the scene.

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

  emit sceneViewChanged();
}

/*!
  \internal
 */
QObject* QmlArcGISArView::transformationMatrixCameraController() const
{
  return m_tmcc;
}

/*!
  \internal
 */
void QmlArcGISArView::setTransformationMatrixCameraController(QObject* tmcc)
{
  if (!tmcc || m_tmcc || !assertClassName(tmcc, "QmlTransformationMatrixCameraController"))
    return;

  m_tmcc = tmcc;
  emit transformationMatrixCameraControllerChanged();
}

/*!
  \internal
  Sets the initial transformation used to offset the originCamera.

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

  // QuaternionW can never be 0, this indicates an error occurred
  if (hitResult[3] == 0)
    return;

  emit initialTransformationChanged(0.0, 0.0, 0.0, 1.0, hitResult[4], hitResult[5], hitResult[6]);
}

/*!
  \internal
  \brief Gets the location in the real world space corresponding to the screen point.
*/
std::vector<qreal> QmlArcGISArView::screenToLocation(float x, float y) const
{
  // Converts the point from the screen space to the 3D space
  const std::array<double, 7> hitResult = ArcGISArViewInterface::hitTestInternal(x, y);

  // Copy the data to std::vector to use it in QML.
  return std::vector<qreal>(hitResult.begin(), hitResult.end());
}

/*!
  \brief Register the QML creatable types provide by QR toolkit.

  The static function register the QML types \l ArcGISArView and \l LocationDataSource in the QML engine.
  This function must becalled before using the QML types.
 */
void QmlArcGISArView::qmlRegisterTypes()
{
  qmlRegisterType<QmlArcGISArView>("Esri.ArcGISArToolkit", 1, 0, "ArcGISArViewInternal");
  qmlRegisterType<LocationDataSource>("Esri.ArcGISArToolkit", 1, 0, "LocationDataSource");
  qmlRegisterUncreatableType<ArEnums>("Esri.ArcGISArToolkit", 1, 0, "ArEnums", "ArEnums is not creatable.");

  // Register enum types.
  qRegisterMetaType<ArEnums::LocationTrackingMode>("ArEnums::LocationTrackingMode");
  qRegisterMetaType<ArEnums::SensorStatus>("ArEnums::SensorStatus");
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
  // Do nothing, translationFactorChanged signal is emitted in ArcGISArViewInterface::setTranslationFactor
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
  \fn void QmlArcGISArView::originCameraChanged();
  \brief Signal emitted when the \l originCamera property changes.
 */

/*!
  \fn void QmlArcGISArView::sceneViewChanged();
  \brief Signal emitted when the \l sceneView property changes.
 */
