#include "MapQuickViewProxy.h"

#include <MapQuickView.h>
#include <Map.h>

MapQuickViewProxy::MapQuickViewProxy(QObject* parent) :
  QObject(parent),
  m_mapQuickView(qobject_cast<Esri::ArcGISRuntime::MapQuickView*>(parent)),
  m_map(nullptr)
{
  if (m_mapQuickView)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_mapQuickView, &MapQuickView::mapChanged, this, &MapQuickViewProxy::mapChanged);
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
