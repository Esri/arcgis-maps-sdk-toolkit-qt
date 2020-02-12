#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class MapQuickView;
}
}

class NorthArrowController : public QObject
{
  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(int rotation READ rotation NOTIFY rotationChanged)
  Q_OBJECT

public:
  NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();

  Q_INVOKABLE void resetRotation();

  Esri::ArcGISRuntime::MapQuickView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);

  int rotation() const;

signals:
  void mapViewChanged();
  void rotationChanged();

private:
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  int m_rotation = 0;
};

#endif // NORTHARROWCONTROLLER_H
