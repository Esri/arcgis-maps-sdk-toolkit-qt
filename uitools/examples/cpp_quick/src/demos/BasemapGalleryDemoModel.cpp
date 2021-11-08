#include "BasemapGalleryDemoModel.h"

#include "Esri/ArcGISRuntime/Toolkit/BasemapGalleryController.h"

#include <MapView.h>
#include <GeoView.h>
#include <Map.h>
#include <Portal.h>
#include <Scene.h>

BasemapGalleryDemoModel::BasemapGalleryDemoModel(QObject* parent) :
  BaseDemoModel(parent),
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

BasemapGalleryDemoModel::~BasemapGalleryDemoModel()
{
}

Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* BasemapGalleryDemoModel::controller() const
{
  return m_controller;
}

Esri::ArcGISRuntime::Map* BasemapGalleryDemoModel::initMap_(QObject*) const
{
  // Don't create a default map. We derive our map from the BasemapGallery.
  return nullptr;
}

Esri::ArcGISRuntime::Scene* BasemapGalleryDemoModel::initScene_(QObject*) const
{
  // Don't create a default scene. We derive our scene from the BasemapGallery.
  return nullptr;
}
