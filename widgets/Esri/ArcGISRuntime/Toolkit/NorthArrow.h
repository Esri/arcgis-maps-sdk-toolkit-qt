#ifndef NORTHARROW_H
#define NORTHARROW_H

#include <QLabel>
#include <QPixmap>

namespace Esri {
namespace ArcGISRuntime {

class MapGraphicsView;

namespace Toolkit {

class NorthArrowController;

class NorthArrow : public QLabel
{
  Q_OBJECT

public:
  explicit NorthArrow(QWidget* parent = nullptr);
  ~NorthArrow() override;

  void setMapView(MapGraphicsView* mapView);

  void mouseDoubleClickEvent(QMouseEvent* event) override;

  NorthArrowController* controller() const;

private:
  QPixmap m_image;
  NorthArrowController* m_controller;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NORTHARROW_H
