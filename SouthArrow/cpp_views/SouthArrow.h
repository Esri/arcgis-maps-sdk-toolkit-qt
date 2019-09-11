#ifndef SouthArrow_H
#define SouthArrow_H

#include <QLabel>
#include <QPixmap>

namespace Ui {
class SouthArrow;
}

namespace Esri
  {
  namespace ArcGISRuntime
    {
    class MapGraphicsView;
    }
  }

class SouthArrowController;

class SouthArrow : public QLabel
{
  Q_OBJECT

public:
  explicit SouthArrow(QWidget* parent = nullptr);
  ~SouthArrow();

  void setMapView(Esri::ArcGISRuntime::MapGraphicsView* mapView);

  void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
  QPixmap m_image;
  SouthArrowController* m_controller = nullptr;
};

#endif // SouthArrow_H
