#include "MapQuickViewProxy.h"

#include <MapQuickView.h>
#include <Map.h>


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