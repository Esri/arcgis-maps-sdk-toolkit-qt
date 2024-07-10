/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

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
