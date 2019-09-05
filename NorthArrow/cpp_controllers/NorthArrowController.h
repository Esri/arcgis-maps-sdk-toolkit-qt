#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

namespace Esri
  {
  namespace ArcGISRuntime
    {
#ifdef WIDGETS_TOOLKIT
  class MapGraphicsView;
  using MapViewType = MapGraphicsView;
#else
  class MapQuickView;
  using MapViewType = MapQuickView;
#endif
    }
  }

class NorthArrowController : public QObject
{
#ifndef WIDGETS_TOOLKIT
  Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
  Q_PROPERTY(int rotation READ rotation NOTIFY rotationChanged)
#endif

  Q_OBJECT
  Q_DISABLE_COPY(NorthArrowController)

public:
  NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();

  Q_INVOKABLE void resetRotation();

  Esri::ArcGISRuntime::MapViewType* mapView() const;
  void setMapView(Esri::ArcGISRuntime::MapViewType* mapView);

  int rotation() const;

signals:
  void mapViewChanged();
  void rotationChanged();

private:
  Esri::ArcGISRuntime::MapViewType* m_mapView = nullptr;
  int m_rotation = 0;
};

#endif // NORTHARROWCONTROLLER_H
