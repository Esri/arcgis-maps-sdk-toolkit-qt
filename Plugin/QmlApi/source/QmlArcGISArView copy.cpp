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
#include <QQuickWindow>
#include <QScreen>
#include <QtQml>

/*!
  \class Esri::ArcGISRuntime::Toolkit::QmlArcGISArView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief Render and tracks the camera.
  \sa {AR}
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
  qDebug() << "***** set origin camera" << originCamera << m_tmcc;
  if (m_originCamera == originCamera || assertClassName(originCamera, "QmlCamera"))
    return;

  m_originCamera = originCamera;

  // set origin camera to tmcc if available
  if (tmcc())
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
  qDebug() << "&&&&&& " << m_sceneView;
  if (sceneView == m_sceneView || assertClassName(sceneView, "QmlSceneView"))
    return;

  m_sceneView = sceneView;

  // set TMCC to scene view
  if (m_tmcc)
    m_sceneView->setProperty("cameraController", QVariant::fromValue(m_tmcc));

  emit sceneViewChanged();

  startTracking();
}

/*!
  \qmlmethod Point ArcGISArView::screenToLocation(Point screenPoint)

  \brief Gets the location in the real world space corresponding to the screen point.
*/
QObject* QmlArcGISArView::screenToLocation(QObject* screenPoint) const
{
  if (!qmlWrapper() || !m_sceneView || assertClassName(screenPoint, "QmlPoint"))
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
  const std::array<double, 7> hitResult = ArcGISArViewInterface::screenToLocationInternal(x, y);

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
void QmlArcGISArView::updateTransformationMatrix(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                                 double translationX, double translationY, double translationZ)
{
  if (qmlWrapper())
  {
    QMetaObject::invokeMethod(m_qmlWrapper, "createAndSetTransformationMatrix", Qt::DirectConnection,
                              Q_ARG(QVariant, quaternionX),
                              Q_ARG(QVariant, quaternionY),
                              Q_ARG(QVariant, quaternionZ),
                              Q_ARG(QVariant, quaternionW),
                              Q_ARG(QVariant, translationX),
                              Q_ARG(QVariant, translationY),
                              Q_ARG(QVariant, translationZ));
  }
}

/*!
  \internal
 */
void QmlArcGISArView::updateFieldOfView(double xFocalLength, double yFocalLength,
                                        double xPrincipal, double yPrincipal,
                                        double xImageSize, double yImageSize)
{
  if (!m_sceneView)
    return;

  // get the screen orientation
  const Qt::ScreenOrientations orientation = window()->screen()->orientation();
  int deviceOrientation = 0;

  switch (orientation) {
    case Qt::PortraitOrientation:
      deviceOrientation = 0;
      break;
    case Qt::LandscapeOrientation:
      deviceOrientation = 1;
      break;
    case Qt::InvertedPortraitOrientation:
      deviceOrientation = 2;
      break;
    case Qt::InvertedLandscapeOrientation:
      deviceOrientation = 3;
      break;
    default:
      deviceOrientation = 0;
      break;
  }

  // set the field of view
  if (qmlWrapper())
  {
    QMetaObject::invokeMethod(m_qmlWrapper, "setFieldOfViewFromLensIntrinsics", Qt::DirectConnection,
                              Q_ARG(QVariant, QVariant::fromValue(m_sceneView)),
                              Q_ARG(QVariant, xFocalLength),
                              Q_ARG(QVariant, yFocalLength),
                              Q_ARG(QVariant, xPrincipal),
                              Q_ARG(QVariant, yPrincipal),
                              Q_ARG(QVariant, xImageSize),
                              Q_ARG(QVariant, yImageSize),
                              Q_ARG(QVariant, deviceOrientation));
  }
}

/*!
  \internal
 */
void QmlArcGISArView::renderFrame()
{
  if (!m_sceneView)
    return;

  QMetaObject::invokeMethod(m_sceneView, "renderFrame", Qt::DirectConnection);
}

/*!
  \internal
 */
void QmlArcGISArView::setTranslationFactorInternal(double translationFactor)
{
  if (tmcc())
    m_tmcc->setProperty("translationFactor", translationFactor);
}

/*!
  \internal
 */
void QmlArcGISArView::setLocationInternal(double latitude, double longitude, double altitude)
{
  qDebug() << "++++ set location internal" << latitude << longitude << altitude << m_sceneView << m_tmcc;
  if (m_sceneView)
    qDebug() << "+++ TMCC:" << m_sceneView->property("cameraController").value<QObject*>();

  if (tmcc())
  {
    QMetaObject::invokeMethod(m_qmlWrapper, "setLocation", Qt::DirectConnection,
                              Q_ARG(QVariant, latitude),
                              Q_ARG(QVariant, longitude),
                              Q_ARG(QVariant, altitude));
  }
}

/*!
  \internal

  Create the qmlWrapper object using the qmlWrapper.qml component if necessary and returns it.
 */
QObject* QmlArcGISArView::qmlWrapper() const
{
  if (!m_qmlWrapper)
  {
    // create TMCC object
    QQmlComponent qmlWrapperComponent(qmlEngine(this), QUrl("qrc:/qml/qmlWrapper.qml"));
    m_qmlWrapper = qmlWrapperComponent.create();
    Q_CHECK_PTR(m_qmlWrapper);
  }

  return m_qmlWrapper;
}

/*!
  \internal

  Create the TMCC object using the qmlWrapper if necessary and returns it.
 */
QObject* QmlArcGISArView::tmcc() const
{
  qDebug() << "***** >>>> m_tmcc" << m_tmcc;
  if (!m_tmcc)
  {
    // get the TMCC object from the QML object
    m_tmcc = qmlWrapper()->property("tmcc").value<QObject*>();
    Q_CHECK_PTR(m_tmcc);
    qDebug() << "*****>>>> m_tmcc" << m_tmcc << m_sceneView;

    connect(m_tmcc, SIGNAL(originCameraChanged()), this, SIGNAL(originCameraChanged()));

    // set TMCC to scene view
    if (m_sceneView)
      m_sceneView->setProperty("cameraController", QVariant::fromValue(m_tmcc));

    // set origin camera to tmcc if available
    if (m_originCamera)
      m_tmcc->setProperty("originCamera", QVariant::fromValue(m_originCamera));

    // set translation factor to tmcc
    m_tmcc->setProperty("translationFactor", translationFactor());
  }

  return m_tmcc;
}

/*!
  \internal

  As the QML API headers is not available, it's not possible to use the concret types in the QmlArcGISArView
  API. Then, this API uses QObject* and this function is used to verify than the type of the objects passed
  in the parameters are correct (using the Qt's meta-object).
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
