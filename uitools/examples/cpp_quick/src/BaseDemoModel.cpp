#include "BaseDemoModel.h"

#include <Map.h>
#include <MapQuickView.h>
#include <MapViewTypes.h>
#include <Scene.h>
#include <SceneQuickView.h>
#include <stdexcept>

using namespace Esri::ArcGISRuntime;

BaseDemoModel::BaseDemoModel(QObject* parent) :
  QObject(parent),
  m_geoView(nullptr)
{
}

BaseDemoModel::~BaseDemoModel()
{
}

Esri::ArcGISRuntime::GeoView* BaseDemoModel::geoView() const
{
  if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
  {
    return mapView;
  }
  else if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
  {
    return sceneView;
  }
  return nullptr;
}

QObject* BaseDemoModel::geoView_() const
{
  return m_geoView;
}

void BaseDemoModel::setGeoView(Esri::ArcGISRuntime::GeoView* geoView)
{
  if (geoView == nullptr)
  {
    setGeoView_(nullptr);
  }
  else
  {
    if (geoView->geoViewType() == GeoViewType::MapView)
    {
      setGeoView_(static_cast<MapQuickView*>(geoView));
    }
    else if (geoView->geoViewType() == GeoViewType::SceneView)
    {
      setGeoView_(static_cast<SceneQuickView*>(geoView));
    }
    else
    {
      throw std::runtime_error("Unknown type of GeoView");
    }
  }
}

void BaseDemoModel::setGeoView_(QObject* geoView)
{
  if (m_geoView == geoView)
  {
    return;
  }
  m_geoView = geoView;

  // If the GeoView has no map or scene applied, we apply our ow using the
  // `initMap_`/`initScene_` overloads.
  if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
  {
    if (mapView->map() == nullptr)
    {
      mapView->setMap(initMap_(mapView));
    }
  }
  else if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
  {
    if (sceneView->arcGISScene() == nullptr)
    {
      sceneView->setArcGISScene(initScene_(sceneView));
    }
  }

  emit geoViewChanged();
}

Esri::ArcGISRuntime::GeoModel* BaseDemoModel::geoModel() const
{
  if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
  {
    return mapView->map();
  }
  else if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
  {
    return sceneView->arcGISScene();
  }
  return nullptr;
}

bool BaseDemoModel::setGeoModel(Esri::ArcGISRuntime::Map* map)
{
  if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
  {
    mapView->setMap(map);
    return true;
  }
  return false;
}

bool BaseDemoModel::setGeoModel(Esri::ArcGISRuntime::Scene* scene)
{
  if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
  {
    sceneView->setArcGISScene(scene);
    return true;
  }
  return false;
}

Esri::ArcGISRuntime::Map* BaseDemoModel::initMap_(QObject* parent) const
{
  return new Map(BasemapStyle::ArcGISCommunity, parent);
}

Esri::ArcGISRuntime::Scene* BaseDemoModel::initScene_(QObject* parent) const
{
  return new Scene(BasemapStyle::ArcGISCommunity, parent);
}
