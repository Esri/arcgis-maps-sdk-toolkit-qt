
/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#include "NorthArrowController.h"

#include "GeoViews.h"

#include "Camera.h"

#include <QFuture>

// std headers
#include <cmath>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

  NorthArrowController::NorthArrowController(QObject* parent) :
    QObject(parent)
  {
  }

  NorthArrowController::~NorthArrowController()
  {
  }

  void NorthArrowController::setHeading(double heading)
  {
    if (auto mapView = qobject_cast<MapView*>(m_geoView))
    {
      auto future = mapView->setViewpointRotationAsync(heading);
      Q_UNUSED(future)
    }
    else if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
    {
      Camera currentCamera = sceneView->currentViewpointCamera();
      Camera updatedCamera = currentCamera.rotateTo(heading, currentCamera.pitch(), currentCamera.roll());
      auto future = sceneView->setViewpointCameraAsync(updatedCamera, 0.50);
      Q_UNUSED(future)
    }
  }

  double NorthArrowController::heading() const
  {
    if (auto mapView = qobject_cast<MapView*>(m_geoView))
      return mapView->mapRotation();
    else if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
      return sceneView->currentViewpointCamera().heading();

    return static_cast<double>(NAN);
  }

  QObject* NorthArrowController::geoView() const
  {
    return m_geoView;
  }

  void NorthArrowController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);
    }

    m_geoView = geoView;

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connect(mapView, &MapViewToolkit::mapRotationChanged,
              this, &NorthArrowController::headingChanged);
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      connect(sceneView, &SceneViewToolkit::viewpointChanged,
              this, &NorthArrowController::headingChanged);
    }

    emit geoViewChanged();
  }

} // Esri::ArcGISRuntime::Toolkit
