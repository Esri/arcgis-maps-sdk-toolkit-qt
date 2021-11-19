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
#include "MapQuickViewProxy.h"

#include <Map.h>
#include <MapQuickView.h>

/*!
    \internal
    \class MapQuickViewProxy
    \brief This class exposes MapQuickViewProxy such that `MapView.map` is accessible from QML
    This is registered as an extension to MapQuickView in main.cpp.
*/

MapQuickViewProxy::MapQuickViewProxy(QObject* parent) :
  QObject(parent),
  m_mapQuickView(qobject_cast<Esri::ArcGISRuntime::MapQuickView*>(parent)),
  m_map(nullptr)
{
  if (m_mapQuickView)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_mapQuickView, &MapQuickView::mapChanged, this, &MapQuickViewProxy::mapChangedProxy);
  }
}

MapQuickViewProxy::~MapQuickViewProxy()
{
}

GeoModelProxy* MapQuickViewProxy::map()
{
  if (m_mapQuickView)
  {
    auto map = m_mapQuickView->map();
    if (!m_map || m_map->parent() != map)
    {
      m_map = new GeoModelProxy(map);
    }
    return m_map;
  }
  return nullptr;
}
