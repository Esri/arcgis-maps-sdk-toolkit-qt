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
  \brief ...
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime::Toolkit::Internal;

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
  \brief ...
 */
QObject* QmlArcGISArView::originCamera() const
{
  return m_originCamera;
}

/*!
  \brief ...
 */
void QmlArcGISArView::setOriginCamera(QObject* originCamera)
{
  if (m_originCamera == originCamera || assertClassName(originCamera, "QmlCamera"))
    return;

  m_originCamera = originCamera;

  // set origin camera to tmcc if available
  if (m_tmcc)
    m_tmcc->setProperty("originCamera", QVariant::fromValue(m_originCamera));

  emit originCameraChanged();
}

/*!
  \brief ...
 */
QObject* QmlArcGISArView::sceneView() const
{
  return m_sceneView;
}

/*!
  \brief ...
 */
void QmlArcGISArView::setSceneView(QObject* sceneView)
{
  if (sceneView == m_sceneView || assertClassName(sceneView, "QmlSceneView"))
    return;

  m_sceneView = sceneView;
  emit sceneViewChanged();

  startTracking();
}

/*!
  \brief ...
 */
QObject* QmlArcGISArView::screenToLocation(QObject* screenPoint) const
{
  if (!createQmlWrapper() || !m_sceneView || assertClassName(screenPoint, "QmlPoint"))
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
  QMetaObject::invokeMethod(m_tmcc, "screenToLocation", Qt::DirectConnection,
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
void QmlArcGISArView::updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                   double translationX, double translationY, double translationZ)
{
  if (!createQmlWrapper())
    return;

  QMetaObject::invokeMethod(m_tmcc, "createAndSetTransformationMatrix", Qt::DirectConnection,
                            Q_ARG(QVariant, quaternionX),
                            Q_ARG(QVariant, quaternionY),
                            Q_ARG(QVariant, quaternionZ),
                            Q_ARG(QVariant, quaternionW),
                            Q_ARG(QVariant, translationX),
                            Q_ARG(QVariant, translationY),
                            Q_ARG(QVariant, translationZ));
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
  QMetaObject::invokeMethod(m_tmcc, "setFieldOfViewFromLensIntrinsics", Qt::DirectConnection,
                            Q_ARG(QVariant, QVariant::fromValue(m_sceneView)),
                            Q_ARG(QVariant, xFocalLength),
                            Q_ARG(QVariant, yFocalLength),
                            Q_ARG(QVariant, xPrincipal),
                            Q_ARG(QVariant, yPrincipal),
                            Q_ARG(QVariant, xImageSize),
                            Q_ARG(QVariant, yImageSize),
                            Q_ARG(QVariant, deviceOrientation));
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
  if (m_tmcc)
    m_tmcc->setProperty("translationFactor", translationFactor);
}

/*!
  \internal
 */
QObject* QmlArcGISArView::createQmlWrapper() const
{
  if (!m_qmlWrapper)
  {
    // create TMCC object
    QQmlComponent qmlWrapperComponent(qmlEngine(this), QUrl("qrc:/qml/tmcc.qml"));
    m_qmlWrapper = qmlWrapperComponent.create();

    // get the TMCC object from the QML object
    m_tmcc = m_qmlWrapper->property("tmcc").value<QObject*>();

    // set TMCC to scene view
    if (m_sceneView)
      m_sceneView->setProperty("cameraController", QVariant::fromValue(m_tmcc));

    // set origin camera to tmcc if available
    if (m_originCamera)
      m_tmcc->setProperty("originCamera", QVariant::fromValue(m_originCamera));

    // set translation factor to tmcc
    m_tmcc->setProperty("translationFactor", translationFactor());
  }

  return m_qmlWrapper;
}

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
