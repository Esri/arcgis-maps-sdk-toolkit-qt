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
#include "GeoModelProxy.h"

#include <GeoModel.h>

/*!
    \internal
    \class GeoModelProxy
    \brief This class exposes GeoModel such that GeoModel.loadStatus is accessible from QML.
    I.e. in the case of wanting to access: `MapView.map.loadStatus` or `SceneView.scene.loadStatus`
    from QML.
*/

GeoModelProxy::GeoModelProxy(QObject* parent) :
  QObject(parent),
  m_geoModel(qobject_cast<Esri::ArcGISRuntime::GeoModel*>(parent))
{
  if (m_geoModel)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_geoModel, &GeoModel::loadStatusChanged, this, &GeoModelProxy::loadStatusChanged);
  }
}

GeoModelProxy::~GeoModelProxy()
{
}

EnumsProxy::LoadStatus GeoModelProxy::loadStatus() const
{
  if (m_geoModel)
  {
    return static_cast<EnumsProxy::LoadStatus>(m_geoModel->loadStatus());
  }
  return EnumsProxy::LoadStatus::LoadStatusUnknown;
}
