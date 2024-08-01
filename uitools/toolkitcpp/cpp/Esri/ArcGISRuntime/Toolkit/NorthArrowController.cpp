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

#include "Internal/GeoViews.h"

#include "Camera.h"

#include <QFuture>

// std headers
#include <cmath>

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \class Esri::ArcGISRuntime::Toolkit::NorthArrowController
    \inmodule EsriArcGISRuntimeToolkit
    \ingroup ArcGISQtToolkitUiCppControllers
    \brief In MVC architecture, this is the controller for the corresponding
    \c NorthArrow view.

    This controller calculates the current heading from a GeoView, and allows
    the \c NorthArrow to apply a given heading to the GeoView.
   */

  /*!
    \brief Constructor
    \list
      \li \a parent Parent owning \c QObject.
    \endlist
   */
  NorthArrowController::NorthArrowController(QObject* parent) :
    QObject(parent)
  {
  }

  /*!
    \brief Destructor.
   */
  NorthArrowController::~NorthArrowController()
  {
  }

  /*!
    \brief Set the heading by rotating the \c MapView or \c SceneView camera
    to the given \a heading (in degrees).
   */
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
  /*!
    \brief Returns the calculated heading of this controller in degrees.
   */
  double NorthArrowController::heading() const
  {
    if (auto mapView = qobject_cast<MapView*>(m_geoView))
      return mapView->mapRotation();
    else if (auto sceneView = qobject_cast<SceneView*>(m_geoView))
      return sceneView->currentViewpointCamera().heading();

    return static_cast<double>(NAN);
  }

  /*!
    \brief Returns the \c GeoView as a \c QObject.
   */
  QObject* NorthArrowController::geoView() const
  {
    return m_geoView;
  }

  /*!
    \brief Set the \c GeoView object this Controller uses.

    Internally this is cast to a \c MapView or SceneView using \c qobject_cast,
    which is why the paremeter is of form \c QObject and not \c GeoView.

    \list
      \li \a geoView Object which must inherit from \c GeoView* and \c QObject*.
    \endlist
  */
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

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::NorthArrowController::geoViewChanged()
    \brief Emitted when the geoView changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::NorthArrowController::headingChanged()
    \brief Emitted when the heading changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::NorthArrowController::geoView
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::NorthArrowController::heading
   */

} // Esri::ArcGISRuntime::Toolkit
