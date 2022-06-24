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

#include "Esri/ArcGISRuntime/Toolkit/BasemapGalleryController.h"

#include <GeoView.h>
#include <Map.h>
#include <MapView.h>
#include <Portal.h>
#include <Scene.h>

BasemapGalleryDemo::BasemapGalleryDemo(QObject* parent) :
  BaseDemo(parent),
  m_controller(new Esri::ArcGISRuntime::Toolkit::BasemapGalleryController(this))
{
  using namespace Esri::ArcGISRuntime;
  auto portal = m_controller->portal();
  // Once the portal has loaded up basemaps. Choose the first available basemap from the model
  // and apply it to our active view.
  connect(portal, &Portal::developerBasemapsChanged, this,
          [this, portal]
          {
            if (portal->developerBasemaps()->size() > 0)
            {
              auto basemap = portal->developerBasemaps()->at(0);
              if (geoView()->geoViewType() == GeoViewType::MapView)
              {
                setGeoModel(new Map(basemap, this));
              }
              else if (geoView()->geoViewType() == GeoViewType::SceneView)
              {
                setGeoModel(new Scene(basemap, this));
              }
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
  // Don't create a default map. We derive our map from the BasemapGallery.
  return nullptr;
}

Esri::ArcGISRuntime::Scene* BasemapGalleryDemo::initScene_(QObject*) const
{
  // Don't create a default scene. We derive our scene from the BasemapGallery.
  return nullptr;
}
