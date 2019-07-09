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
  \brief A constructor that accepts an optional \a parent.
 */
QmlArcGISArView::QmlArcGISArView(int renderVideoFeed, QQuickItem* parent):
  ArcGISArViewInterface(renderVideoFeed, parent)
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

//  m_sceneView = sceneView;
//   m_sceneView->setSpaceEffect(SpaceEffect::Transparent); // TODO: update the SDK for this change.
////  m_sceneView->setBackgroundTransparent(true);
//  m_sceneView->setAtmosphereEffect(AtmosphereEffect::None);
//  m_sceneView->setParent(this);
//  emit sceneViewChanged();

  startTracking();
}

/*!
  \brief ...
 */
QObject* QmlArcGISArView::arScreenToLocation(QObject* /*screenPoint*/) const
{
  return nullptr;
}

void QmlArcGISArView::updateCamera()
{
  if (!m_originCamera)
  {
    // QObject* camera = m_sceneView->currentViewpointCamera(); // QmlSceneView, property  currentViewpointCamera
//    m_originCamera = camera; // Camera(camera.location(), camera.heading(), 90, 0);
  }

//  QObject* matrix = m_originCamera.transformationMatrix().addTransformation(m_arWrapper->transformationMatrix());
//  m_sceneView->setViewpointCamera(Camera(matrix));
}

// signals

/*!
  \fn void QmlArcGISArView::originCameraChanged();
  \brief Signal emitted when the \l originCamera property changes.
 */
