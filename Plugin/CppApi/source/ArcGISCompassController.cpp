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

#include "ArcGISCompassController.h"
#include "ToolResourceProvider.h"
#include "ToolManager.h"

using namespace Esri::ArcGISRuntime;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISCompassController
  \inmodule ArcGISQtToolkit
  \ingroup ToolArcGISCompass
  \brief The controller for the ArcGIS Compass tool.
  \since Esri::ArcGISRuntime 100.2

  \sa {ArcGIS Compass Tool}
 */

/*!
   \brief The constructor that accepts an optional \a parent object.
 */
ArcGISCompassController::ArcGISCompassController(QObject *parent):
  AbstractTool(parent)
{
  ToolManager::instance().addTool(this);

  connect(ToolResourceProvider::instance(), &ToolResourceProvider::geoViewChanged, this, [this]()
  {
    GeoView* geoView = ToolResourceProvider::instance()->geoView();
    if (geoView)
      setView(geoView);
  });
}

/*!
   \brief The destructor.
 */
ArcGISCompassController::~ArcGISCompassController()
{
}

/*!
   \property ArcGISCompassController::heading
   \brief The compass heading in degrees.
 */
void ArcGISCompassController::setHeading(double rotation)
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
    m_sceneView->setViewpointCamera(updatedCamera, 0.50);
  }
}

/*!
   \property ArcGISCompassController::autoHide
   \brief Whether the compass should be hidden when its heading is north (zero degrees).
 */
void ArcGISCompassController::setAutoHide(bool autoHide)
{
  if (m_autoHide == autoHide)
    return;

  m_autoHide = autoHide;
  emit autoHideChanged();
}

/*!
  \fn Esri::ArcGISRuntime::Toolkit::ArcGISCompassController::setView(Esri::ArcGISRuntime::GeoView* geoView)
  \brief Sets the \c Esri::ArcGISRuntime::GeoView associated with the compass to \a geoView.

  Returns \c true on success.
 */
bool ArcGISCompassController::setView(GeoView *geoView)
{
  if (!geoView)
    return false;

  // controller can only be set to one view
  if (m_sceneView != nullptr || m_mapView != nullptr)
    return false;

  if (geoView->geoViewType() == GeoViewType::SceneView)
    return setView(static_cast<SceneQuickView*>(geoView));
  else if(geoView->geoViewType() == GeoViewType::MapView)
    return setView(static_cast<MapQuickView*>(geoView));
  else
    return false;
}

bool ArcGISCompassController::setView(MapQuickView* mapView)
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

bool ArcGISCompassController::setView(SceneQuickView* sceneView)
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

double ArcGISCompassController::heading() const
{
  return m_heading;
}

bool ArcGISCompassController::autoHide() const
{
  return m_autoHide;
}

QString ArcGISCompassController::toolName() const
{
  return "ArcGISCompass";
}

} // Toolkit
} // ArcGISRuntime
} // Esri
