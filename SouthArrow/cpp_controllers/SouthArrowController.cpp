#include "SouthArrowController.h"

#ifdef WIDGETS_TOOLKIT
#include "MapGraphicsView.h"
#else
#include "MapQuickView.h"
#endif

SouthArrowController::SouthArrowController(QObject* parent):
  QObject(parent)
{
}

SouthArrowController::~SouthArrowController()
{
}

void SouthArrowController::resetRotation()
{
  if (m_mapView)
  {
    m_mapView->setViewpointRotation(180);
  }
}

Esri::ArcGISRuntime::MapViewType* SouthArrowController::mapView() const
{
  return m_mapView;
}

void SouthArrowController::setMapView(Esri::ArcGISRuntime::MapViewType* mapView)
{
  if (mapView == m_mapView)
    return;

  m_mapView = mapView;
  emit mapViewChanged();

  if (!m_mapView)
    return;

  connect(m_mapView, &Esri::ArcGISRuntime::MapViewType::mapRotationChanged, this, &SouthArrowController::rotationChanged);
}

int SouthArrowController::rotation() const
{
  return m_mapView ? (static_cast<int>(m_mapView->mapRotation()) + 180) % 360 : 180;
}
