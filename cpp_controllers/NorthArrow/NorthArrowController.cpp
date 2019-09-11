#include "NorthArrowController.h"

#ifdef WIDGETS_TOOLKIT
#include "MapGraphicsView.h"
#else
#include "MapQuickView.h"
#endif

NorthArrowController::NorthArrowController(QObject* parent):
  QObject(parent)
{
}

NorthArrowController::~NorthArrowController()
{
}

void NorthArrowController::resetRotation()
{
  if (m_mapView)
  {
    m_mapView->setViewpointRotation(0);
  }
}

Esri::ArcGISRuntime::MapViewType* NorthArrowController::mapView() const
{
  return m_mapView;
}

void NorthArrowController::setMapView(Esri::ArcGISRuntime::MapViewType* mapView)
{
  if (mapView == m_mapView)
    return;

  m_mapView = mapView;
  emit mapViewChanged();

  if (!m_mapView)
    return;

  connect(m_mapView, &Esri::ArcGISRuntime::MapViewType::mapRotationChanged, this, &NorthArrowController::rotationChanged);
}

int NorthArrowController::rotation() const
{
  return m_mapView ? m_mapView->mapRotation() : 0;
}
