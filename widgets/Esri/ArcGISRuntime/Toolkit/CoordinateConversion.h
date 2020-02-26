#ifndef TOOLKIT_COORDIANTECONVERSION_H
#define TOOLKIT_COORDIANTECONVERSION_H

#include <QFrame>

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

class CoordinateConversion : public QFrame 
{
  Q_OBJECT
public:
  explicit CoordinateConversion(QWidget* parent = nullptr);
  ~CoordinateConversion() override;

  void setMapView(MapGraphicsView* mapView);
  void setSceneView(SceneGraphicsView* sceneView);

  CoordinateConversionController* controller() const;

private:
  CoordinateConversionController* m_controller;
  Ui::CoordinateConversion* m_ui;
};

}
}
}

#endif // TOOLKIT_COORDIANTECONVERSION_H
