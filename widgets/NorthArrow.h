#ifndef NORTHARROW_H
#define NORTHARROW_H

#include <QLabel>
#include <QPixmap>

namespace Esri { namespace ArcGISRuntime { class MapGraphicsView; } }

class NorthArrowController;

class NorthArrow : public QLabel
{
  Q_OBJECT

public:
  explicit NorthArrow(QWidget* parent = nullptr);
  ~NorthArrow();

  void setMapView(Esri::ArcGISRuntime::MapGraphicsView* mapView);

  void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
  QPixmap m_image;
  NorthArrowController* m_controller = nullptr;
};

#endif // NORTHARROW_H
