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

#include "ToolResourceProvider.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

ToolResourceProvider::ToolResourceProvider(QObject* parent /*= nullptr*/):
  QObject(parent)
{

}

ToolResourceProvider::~ToolResourceProvider()
{

}

Scene* ToolResourceProvider::map() const
{
  return m_map;
}

void ToolResourceProvider::registerMap(Scene* newMap)
{
  if (!newMap)
    return;

  m_map = newMap;

  emit mapChanged();
}

Scene* ToolResourceProvider::scene() const
{
  return m_scene;
}

void ToolResourceProvider::registerScene(Scene* newScene)
{
  if (!newScene)
    return;

  m_scene = newScene;

  emit sceneChanged();
}

GeoView* ToolResourceProvider::geoView() const
{
  return m_geoView;
}

void ToolResourceProvider::registerGeoView(GeoView* newGeoView)
{
  if (!newGeoView)
    return;

  m_geoView = newGeoView;
  emit geoViewChanged();
}

LayerListModel* ToolResourceProvider::operationalLayers() const
{
  if (m_map)
    return m_map->operationalLayers();
  else if (m_scene)
    return m_scene->operationalLayers();

  return nullptr;
}

void ToolResourceProvider::setBasemap(Basemap* newBasemap)
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

void ToolResourceProvider::clear()
{
  m_scene = nullptr;
  m_geoView = nullptr;

  emit sceneChanged();
  emit geoViewChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
