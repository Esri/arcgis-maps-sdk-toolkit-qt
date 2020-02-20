#include "NorthArrow.h"

// Qt headers
#include <QMouseEvent>

// ArcGIS Headers
#include <MapGraphicsView.h>

#include "Esri/ArcGISRuntime/Toolkit/NorthArrowController.h"

NorthArrow::NorthArrow(QWidget* parent) :
  QLabel(parent),
  m_controller(new NorthArrowController(this))
{
  m_image = QPixmap(":/esri/arcgisruntime/tookit/images/compass.png");

  if (!m_image.isNull())
  {
    this->setPixmap(m_image);
  }

  connect(m_controller, &NorthArrowController::headingChanged, this, [this]()
  {
    if (m_image.isNull())
      return;

    QMatrix rm;
    rm.rotate(-m_controller->heading());
    const int imageWidth = m_image.width();
    const int imageHeight = m_image.height();
    auto pix = m_image.transformed(rm, Qt::SmoothTransformation);
    pix = pix.copy((pix.width() - imageWidth)/2,
                   (pix.height() - imageHeight)/2,
                   imageWidth,
                   imageHeight);
    this->setPixmap(pix);
  });
}

NorthArrow::~NorthArrow()
{
}

void NorthArrow::setMapView(MapGraphicsView* mapView)
{
  m_controller->setGeoView(mapView);

}

void NorthArrow::mouseDoubleClickEvent(QMouseEvent* event)
{
  m_controller->setHeading(0);
  event->accept();
}

void NorthArrowController* NorthArrow::controller() const
{
  return m_controller;
}
