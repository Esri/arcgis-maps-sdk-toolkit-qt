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
