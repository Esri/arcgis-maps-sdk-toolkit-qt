#ifndef TOOLKIT_COORDIANTECONVERSION_H
#define TOOLKIT_COORDIANTECONVERSION_H

#include <QFrame>
#include <QPointer>

class QGraphicsEllipseItem;
class QMenu;

namespace Ui
{
class CoordinateConversion;
}

namespace Esri
{
namespace ArcGISRuntime
{
class MapGraphicsView;
class SceneGraphicsView;

namespace Toolkit
{
  
class CoordinateConversionController;
class Flash;

class CoordinateConversion : public QFrame 
{
  Q_OBJECT
public:
  explicit CoordinateConversion(QWidget* parent = nullptr);
  ~CoordinateConversion() override;

  void setMapView(MapGraphicsView* mapView);
  void setSceneView(SceneGraphicsView* sceneView);

  CoordinateConversionController* controller() const;

private slots:
  void addContextMenu(const QPoint& point);
  void flash();

private:
  CoordinateConversionController* m_controller;
  QMenu* m_resultsMenu;
  QPointer<Flash> m_flash;
  Ui::CoordinateConversion* m_ui;
};

}
}
}

#endif // TOOLKIT_COORDIANTECONVERSION_H
