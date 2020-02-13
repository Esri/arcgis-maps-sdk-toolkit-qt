#include "NorthArrow.h"

#include "MapGraphicsView.h"

#include <QMouseEvent>

NorthArrow::NorthArrow(QWidget* parent) :
  QLabel(parent),
  m_mapView(nullptr)
{
  Q_INIT_RESOURCE(images);
  m_image = QPixmap(":/esri/arcgisruntime/tookit/images/compass.png");

  if (!m_image.isNull())
  {
    this->setPixmap(m_image);
  }
}

NorthArrow::~NorthArrow()
{
}

void NorthArrow::setMapView(Esri::ArcGISRuntime::MapGraphicsView* mapView)
{
  disconnect(this, nullptr, m_mapView, nullptr);

  m_mapView = mapView;

  connect(m_mapView, &Esri::ArcGISRuntime::MapGraphicsView::mapRotationChanged, this, [this]()
  {
    if (m_image.isNull())
      return;

    QMatrix rm;
    rm.rotate(-m_mapView->mapRotation());
    const int imageWidth = m_image.width();
    const int imageHeight = m_image.height();
    auto pix = m_image.transformed(rm, Qt::SmoothTransformation);
    pix = pix.copy((pix.width() - imageWidth)/2, (pix.height() - imageHeight)/2, imageWidth, imageHeight);
    this->setPixmap(pix);
  });
}

void NorthArrow::mouseDoubleClickEvent(QMouseEvent* event)
{
  if (m_mapView)
    m_mapView->rotate(0);
  event->accept();
}
