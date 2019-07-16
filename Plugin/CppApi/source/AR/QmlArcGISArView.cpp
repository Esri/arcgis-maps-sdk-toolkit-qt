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

using namespace Esri::ArcGISRuntime;
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
  if (m_originCamera == originCamera)
    return;

  m_originCamera = originCamera;
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
  if (sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  emit sceneViewChanged();

  startTracking();
}

/*!
  \brief ...
 */
QObject* QmlArcGISArView::arScreenToLocation(QObject* /*screenPoint*/) const
{
  return nullptr;
}

/*!
  \brief Not implemented.
 */
void QmlArcGISArView::updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                   double translationX, double translationY, double translationZ)
{
  if (!m_tmcc)
  {
    // create TMCC object
    QQmlComponent tmcc(qmlEngine(this), QUrl("qrc:/tmcc.qml"));
    m_tmcc = tmcc.create();
  }

  if (!m_tmcc)
    return;

  QMetaObject::invokeMethod(m_tmcc, "createAndSetTransformationMatrix", Qt::DirectConnection,
                            Q_ARG(double, quaternionX),
                            Q_ARG(double, quaternionY),
                            Q_ARG(double, quaternionZ),
                            Q_ARG(double, quaternionW),
                            Q_ARG(double, translationX),
                            Q_ARG(double, translationY),
                            Q_ARG(double, translationZ));
}

/*!
  \brief Not implemented.
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
  QMetaObject::invokeMethod(m_sceneView, "setFieldOfViewFromLensIntrinsics", Qt::DirectConnection,
                            Q_ARG(double, xFocalLength),
                            Q_ARG(double, yFocalLength),
                            Q_ARG(double, xPrincipal),
                            Q_ARG(double, yPrincipal),
                            Q_ARG(double, xImageSize),
                            Q_ARG(double, yImageSize),
                            Q_ARG(int, deviceOrientation));
}

void QmlArcGISArView::renderFrame()
{
  if (!m_sceneView)
    return;

  QMetaObject::invokeMethod(m_sceneView, "renderFrame", Qt::DirectConnection);
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
