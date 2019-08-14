#include "NorthArrowController.h"

#include "MapQuickView.h"

NorthArrowController::NorthArrowController(QObject* parent):
  QObject(parent)
{
}

NorthArrowController::~NorthArrowController()
{
}

Esri::ArcGISRuntime::MapQuickView *NorthArrowController::mapView() const
{
  return m_mapView;
}

void NorthArrowController::setMapView(Esri::ArcGISRuntime::MapQuickView* mapView)
{
  if (mapView == m_mapView)
    return;

  m_mapView = mapView;
  emit mapViewChanged();

  if (!m_mapView)
    return;

  connect(m_mapView, &Esri::ArcGISRuntime::MapQuickView::mapRotationChanged, this, &NorthArrowController::rotationChanged);
}

int NorthArrowController::rotation() const
{
  return m_mapView ? m_mapView->mapRotation() : 0;
}
