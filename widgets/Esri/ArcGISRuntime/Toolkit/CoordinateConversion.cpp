#include "CoordinateConversion.h"

#include "Esri/ArcGISRuntime/Toolkit/CoordinateConversionController.h"

#include "ui_CoordinateConversion.h"

// ArcGISRuntime headers
#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversion::CoordinateConversion(QWidget* parent) :
  QFrame(parent),
  m_controller(new CoordinateConversionController(this)),
  m_ui(new Ui::CoordinateConversion)
{
  m_ui->setupUi(this);
}

CoordinateConversion::~CoordinateConversion()
{
  delete m_ui;
}

void CoordinateConversion::setMapView(MapGraphicsView* mapView)
{
  m_controller->setGeoView(mapView);
}

void CoordinateConversion::setSceneView(SceneGraphicsView* sceneView)
{
  m_controller->setGeoView(sceneView);
}

CoordinateConversionController* CoordinateConversion::controller() const
{
  return m_controller;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
