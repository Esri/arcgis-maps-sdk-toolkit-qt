#ifndef NORTHARROW_H
#define NORTHARROW_H

#include <QLabel>
#include <QPixmap>

namespace Esri {
namespace ArcGISRuntime {

class MapGraphicsView;
class SceneGraphicsView;

namespace Toolkit {

class NorthArrowController;

class NorthArrow : public QLabel
{
  Q_OBJECT
public:
  explicit NorthArrow(QWidget* parent = nullptr);
  ~NorthArrow() override;

  void setMapView(MapGraphicsView* mapView);
  void setSceneView(SceneGraphicsView* mapView);

  NorthArrowController* controller() const;

protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
  QPixmap m_image;
  NorthArrowController* m_controller;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NORTHARROW_H
