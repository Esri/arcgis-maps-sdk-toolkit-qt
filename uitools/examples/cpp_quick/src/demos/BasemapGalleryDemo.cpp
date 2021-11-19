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
