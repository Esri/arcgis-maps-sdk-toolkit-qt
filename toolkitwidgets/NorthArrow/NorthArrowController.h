#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

namespace Esri
  {
  namespace ArcGISRuntime
    {

  class MapGraphicsView;
    }
  }

class NorthArrowController : public QObject
{
  Q_PROPERTY(Esri::ArcGISRuntime::MapGraphicsView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(int rotation READ rotation NOTIFY rotationChanged)

  Q_OBJECT
  Q_DISABLE_COPY(NorthArrowController)

public:
  NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();

  Q_INVOKABLE void resetRotation();

  Esri::ArcGISRuntime::MapGraphicsView* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapGraphicsView* mapView);

  int rotation() const;

signals:
  void mapViewChanged();
  void rotationChanged();

private:
  Esri::ArcGISRuntime::MapGraphicsView* m_mapView = nullptr;
  int m_rotation = 0;
};

#endif // NORTHARROWCONTROLLER_H
