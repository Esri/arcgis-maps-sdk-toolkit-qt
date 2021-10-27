#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_GEOMODELPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_GEOMODELPROXY_H

#include "EnumsProxy.h"

#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class GeoModel;
}
}

class GeoModelProxy : public QObject
{
  Q_OBJECT
  Q_PROPERTY(EnumsProxy::LoadStatus loadStatus READ loadStatus NOTIFY loadStatusChanged)
public:
  Q_INVOKABLE explicit GeoModelProxy(QObject* parent = nullptr);
  ~GeoModelProxy() override;

  EnumsProxy::LoadStatus loadStatus() const;

signals:
  void loadStatusChanged();

private:
  Esri::ArcGISRuntime::GeoModel* m_geoModel;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_GEOMODELPROXY_H
