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

#include "BasemapGalleryDemo.h"

#include "BasemapGalleryController.h"
#include "BasemapListModel.h"
#include "GeoView.h"
#include "Map.h"
#include "MapViewTypes.h"
#include "Portal.h"
#include "Scene.h"
#include "SceneViewTypes.h"

using namespace Esri::ArcGISRuntime;

BasemapGalleryDemo::BasemapGalleryDemo(QObject* parent) :
  BaseDemo(parent),
  m_controller(new Esri::ArcGISRuntime::Toolkit::BasemapGalleryController(this))
{
  auto portal = m_controller->portal();
  // Once the portal has loaded up basemaps. Choose the first available basemap from the model
  // and apply it to our active view.
  connect(m_controller, &Toolkit::BasemapGalleryController::basemapsChanged, this, [this, portal]
  {
    if (!portal->developerBasemaps() || portal->developerBasemaps()->isEmpty())
    {
      return;
    }

    // Ensure existing GeoModel is cleaned up after we set a new one
    if (m_controller->geoModel())
    {
      m_controller->geoModel()->deleteLater();
    }

    auto* basemap = portal->developerBasemaps()->at(0);

    if (geoView()->geoViewType() == GeoViewType::MapView)
    {
      setGeoModel(new Map(basemap, this));
    }
    else if (geoView()->geoViewType() == GeoViewType::SceneView)
    {
      setGeoModel(new Scene(SceneViewingMode::Global, basemap, this));
    }
    else if (geoView()->geoViewType() == GeoViewType::LocalSceneView)
    {
      setGeoModel(new Scene(SceneViewingMode::Local, basemap, this));
    }
  });
}

BasemapGalleryDemo::~BasemapGalleryDemo()
{
}

Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* BasemapGalleryDemo::controller() const
{
  return m_controller;
}

Esri::ArcGISRuntime::Map* BasemapGalleryDemo::initMap_(QObject*) const
{
  // 3D basemaps are not fetched by default, so we don't need to pre-instantiate the Map GeoModel
  return nullptr;
}

Esri::ArcGISRuntime::Scene* BasemapGalleryDemo::initGlobalScene_(QObject* parent) const
{
  // Create a temporary Scene to instantiate the Gallery with so it will fetch 3D basemaps
  return new Scene(SceneViewingMode::Global, parent);
}

Esri::ArcGISRuntime::Scene *BasemapGalleryDemo::initLocalScene_(QObject* parent) const
{
  // Create a temporary Scene to instantiate the Gallery with so it will fetch 3D basemaps
  return new Scene(SceneViewingMode::Local, parent);
}
