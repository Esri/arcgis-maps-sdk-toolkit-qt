// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
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
  Q_PROPERTY(GeoModelProxy* scene READ scene NOTIFY sceneChangedProxy)
public:
  Q_INVOKABLE explicit SceneQuickViewProxy(QObject* parent = nullptr);
  ~SceneQuickViewProxy() override;

private:
  GeoModelProxy* scene();

signals:
  void sceneChangedProxy();

private:
  Esri::ArcGISRuntime::SceneQuickView* m_sceneQuickView;
  QPointer<GeoModelProxy> m_scene;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H
