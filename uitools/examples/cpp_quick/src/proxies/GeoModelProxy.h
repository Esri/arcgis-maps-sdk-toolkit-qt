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
