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
#include "BaseDemo.h"

#include <Map.h>
#include <MapQuickView.h>
#include <MapViewTypes.h>
#include <Scene.h>
#include <SceneQuickView.h>
#include <stdexcept>

using namespace Esri::ArcGISRuntime;

BaseDemo::BaseDemo(QObject* parent) :
  QObject(parent),
  m_geoView(nullptr)
{
}

BaseDemo::~BaseDemo()
{
}

Esri::ArcGISRuntime::GeoView* BaseDemo::geoView() const
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

QObject* BaseDemo::geoView_() const
{
  return m_geoView;
}

void BaseDemo::setGeoView(Esri::ArcGISRuntime::GeoView* geoView)
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

void BaseDemo::setGeoView_(QObject* geoView)
{
  if (m_geoView == geoView)
  {
    return;
  }
  m_geoView = geoView;

  // If the GeoView has no map or scene applied, we apply our own using the
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

Esri::ArcGISRuntime::GeoModel* BaseDemo::geoModel() const
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

bool BaseDemo::setGeoModel(Esri::ArcGISRuntime::Map* map)
{
  if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
  {
    mapView->setMap(map);
    return true;
  }
  return false;
}

bool BaseDemo::setGeoModel(Esri::ArcGISRuntime::Scene* scene)
{
  if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
  {
    sceneView->setArcGISScene(scene);
    return true;
  }
  return false;
}

Esri::ArcGISRuntime::Map* BaseDemo::initMap_(QObject* parent) const
{
  return new Map(BasemapStyle::ArcGISCommunity, parent);
}

Esri::ArcGISRuntime::Scene* BaseDemo::initScene_(QObject* parent) const
{
  return new Scene(BasemapStyle::ArcGISCommunity, parent);
}
