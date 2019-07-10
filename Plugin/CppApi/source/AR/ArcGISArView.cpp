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
  \brief Render and tracks the camera.
  \sa {AR}
 */

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISArView::ArcGISArView(QQuickItem* parent):
  ArcGISArViewInterface(parent)
{
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
  ArcGISArViewInterface(renderVideoFeed, tryUsingArKit, parent)
{
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
  emit originCameraChanged();
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
  m_sceneView->setParent(this);
  emit sceneViewChanged();

  startTracking();
}

/*!
  \brief Not implemented.
 */
Point ArcGISArView::arScreenToLocation(const Point& /*screenPoint*/) const
{
  return Point();
}

/*!
  \brief Not implemented.
 */
void ArcGISArView::updateCamera(double quaternionX, double quaternionY, double quaternionZ, double quaternionW,
                                double translationX, double translationY, double translationZ)
{
  if (!m_sceneView)
    return;

  TransformationMatrix newMatrix(quaternionX, quaternionY, quaternionZ, quaternionW,
                                 translationX, translationY, translationZ);
  TransformationMatrix matrix = m_originCamera.transformationMatrix().addTransformation(newMatrix);
  m_sceneView->setViewpointCamera(Camera(matrix));
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

