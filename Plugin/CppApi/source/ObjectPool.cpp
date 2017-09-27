// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "Basemap.h"
#include "Map.h"
#include "Scene.h"

#include "ObjectPool.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

ObjectPool::ObjectPool(QObject* parent /*= nullptr*/):
  QObject(parent)
{

}

ObjectPool::~ObjectPool()
{

}

Scene* ObjectPool::map() const
{
  return m_map;
}

void ObjectPool::registerMap(Scene* newMap)
{
  if (!newMap)
    return;

  m_map = newMap;

  emit mapChanged();
}

Scene* ObjectPool::scene() const
{
  return m_scene;
}

void ObjectPool::registerScene(Scene* newScene)
{
  if (!newScene)
    return;

  m_scene = newScene;

  emit sceneChanged();
}

GeoView* ObjectPool::geoView() const
{
  return m_geoView;
}

void ObjectPool::registerGeoView(GeoView* newGeoView)
{
  if (!newGeoView)
    return;

  m_geoView = newGeoView;
  emit geoViewChanged();
}

LayerListModel *ObjectPool::operationalLayers() const
{
  if (m_map)
    return m_map->operationalLayers();
  else if (m_scene)
    return m_scene->operationalLayers();

  return nullptr;
}

void ObjectPool::setBasemap(Basemap* newBasemap)
{
  if (!newBasemap)
    return;

  if (m_map)
  {
    newBasemap->setParent(m_map);
    m_map->setBasemap(newBasemap);
  }
  else if (m_scene)
  {
    newBasemap->setParent(m_scene);
    m_scene->setBasemap(newBasemap);
  }
}

void ObjectPool::clear()
{
  m_scene = nullptr;
  m_geoView = nullptr;

  emit sceneChanged();
  emit geoViewChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
