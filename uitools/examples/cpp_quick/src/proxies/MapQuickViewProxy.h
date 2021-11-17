#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_MAPQUCIKVIEWPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_MAPQUCIKVIEWPROXY_H

#include "GeoModelProxy.h"

#include <QObject>
#include <QPointer>

namespace Esri {
namespace ArcGISRuntime {
  class MapQuickView;
}
}

class MapQuickViewProxy : public QObject
{
  Q_OBJECT
  Q_PROPERTY(GeoModelProxy* map READ map NOTIFY mapChangedProxy)
public:
  Q_INVOKABLE explicit MapQuickViewProxy(QObject* parent = nullptr);
  ~MapQuickViewProxy() override;

private:
  GeoModelProxy* map();

signals:
  void mapChangedProxy();

private:
  Esri::ArcGISRuntime::MapQuickView* m_mapQuickView;
  mutable QPointer<GeoModelProxy> m_map;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_MAPQUCIKVIEWPROXY_H
