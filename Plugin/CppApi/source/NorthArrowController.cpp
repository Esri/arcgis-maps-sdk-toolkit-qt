// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "Camera.h"
#include "MapQuickView.h"
#include "SceneQuickView.h"
#include "NorthArrowController.h"

using namespace Esri::ArcGISRuntime;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

NorthArrowController::NorthArrowController(QObject *parent):
  QObject(parent)
{
}

NorthArrowController::~NorthArrowController()
{
}

void NorthArrowController::setHeading(double rotation)
{
  if (m_heading == rotation)
    return;

  if (m_mapView)
    m_mapView->setViewpointRotation(rotation);
  else if (m_sceneView)
  {
    // create a new camera with same pitch and roll but different heading
    Camera currentCamera = m_sceneView->currentViewpointCamera();
    Camera updatedCamera = currentCamera.rotateTo(rotation, currentCamera.pitch(), currentCamera.roll());
    m_sceneView->setViewpointCamera(updatedCamera);
  }
}

void NorthArrowController::setAutoHide(bool autoHide)
{
  if (m_autoHide == autoHide)
    return;

  m_autoHide = autoHide;
  emit autoHideChanged();
}

bool NorthArrowController::setView(MapQuickView* mapView)
{
  if (!mapView)
    return false;

  // controller can only be set to one view
  if (m_sceneView != nullptr || m_mapView != nullptr)
    return false;

  // set mapView
  m_mapView = mapView;

  connect(m_mapView, &MapQuickView::mapRotationChanged, this,
  [this]()
  {
    m_heading = m_mapView->mapRotation();
    emit headingChanged();
  });

  m_heading = mapView->mapRotation();

  return true;
}

bool NorthArrowController::setView(SceneQuickView* sceneView)
{
  if (!sceneView)
    return false;

  // controller can only be set to one view
  if (m_sceneView != nullptr || m_mapView != nullptr)
    return false;

  // set SceneView
  m_sceneView = sceneView;

  connect(m_sceneView, &SceneQuickView::viewpointChanged, this,
  [this]()
  {
    m_heading = m_sceneView->currentViewpointCamera().heading();
    emit headingChanged();
  });

  m_heading = m_sceneView->currentViewpointCamera().heading();

  return true;
}

double NorthArrowController::heading() const
{
  return m_heading;
}

bool NorthArrowController::autoHide() const
{
  return m_autoHide;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
