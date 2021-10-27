#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H

#include "GeoModelProxy.h"

#include <QObject>
#include <QPointer>

namespace Esri {
namespace ArcGISRuntime {
  class SceneQuickView;
}
}

class SceneQuickViewProxy : public QObject
{
  Q_OBJECT
  Q_PROPERTY(GeoModelProxy* scene READ scene NOTIFY sceneChanged)
public:
  Q_INVOKABLE explicit SceneQuickViewProxy(QObject* parent = nullptr);
  ~SceneQuickViewProxy() override;

private:
  GeoModelProxy* scene();

signals:
  void sceneChanged();

private:
  Esri::ArcGISRuntime::SceneQuickView* m_sceneQuickView;
  QPointer<GeoModelProxy> m_scene;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H
