// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.

#include "CppArSample.h"

#include "ArcGISTiledElevationSource.h"
#include "Basemap.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "IntegratedMeshLayer.h"
#include "ArcGISSceneLayer.h"
#include "ArcGISArView.h"
#include "GeometryEngine.h"

#include <QUrl>
#include <QFileInfo>
#include <QStandardPaths>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

CppArSample::CppArSample(QObject* parent):
  QObject(parent)
{
}

CppArSample::~CppArSample()
{
}

ArcGISArView* CppArSample::arcGISArView() const
{
  return m_arcGISArView;
}

void CppArSample::setArcGISArView(ArcGISArView* arcGISArView)
{
  if (!arcGISArView || arcGISArView == m_arcGISArView)
    return;

  m_arcGISArView = arcGISArView;
  emit arcGISArViewChanged();
}

SceneQuickView* CppArSample::sceneView() const
{
  return m_sceneView;
}

// Set the view (created in QML)
void CppArSample::setSceneView(SceneQuickView* sceneView)
{
  if (!sceneView || sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  emit sceneViewChanged();
}

// from https://devtopia.esri.com/mort5161/ARSamples/blob/master/ArcGISAR.Droid/TestScenes.cs

void CppArSample::createBrestScene()
{
  if (m_scene)
    delete m_scene;

  // create scene
  m_scene = new Scene(this);

  // create surface
  const QUrl elevationSourceUrl("https://scene.arcgis.com/arcgis/rest/services/BREST_DTM_1M/ImageServer");
  const QList<ElevationSource*> sources = { new ArcGISTiledElevationSource(elevationSourceUrl, this) };
  Surface* baseSurface = new Surface(sources, this);

  BackgroundGrid grid;
  grid.setVisible(false);
  baseSurface->setBackgroundGrid(grid);

  baseSurface->setNavigationConstraint(NavigationConstraint::None);

  m_scene->setBaseSurface(baseSurface);

  // create layer
  const QUrl brestFrance("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Brest/SceneServer/layers/0");
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(brestFrance, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  const Camera observerCamera(Point(-4.49492, 48.3808, 48.2511, SpatialReference::wgs84()), 344.488, 74.1212, 0.0);
  m_arcGISArView->setOriginCamera(observerCamera);
  m_arcGISArView->setTranslationFactor(250);

  m_sceneView->setArcGISScene(m_scene);
}

void CppArSample::createBerlinScene()
{
  if (m_scene)
    delete m_scene;

  // create scene
  m_scene = new Scene(Basemap::imagery(this), this);

  // create surface
  const QUrl elevationSourceUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
  const QList<ElevationSource*> sources = { new ArcGISTiledElevationSource(elevationSourceUrl, this) };
  Surface* baseSurface = new Surface(sources, this);

  BackgroundGrid grid;
  grid.setVisible(false);
  baseSurface->setBackgroundGrid(grid);
  baseSurface->setNavigationConstraint(NavigationConstraint::None);
  m_scene->setBaseSurface(baseSurface);

  // create layer
  const QUrl buildingsService("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Berlin/SceneServer");
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(buildingsService, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  connect(layer, &ArcGISSceneLayer::doneLoading, this, [this, layer](Error)
  {
    const Point center = geometry_cast<Point>(GeometryEngine::project(layer->fullExtent().center(), SpatialReference::wgs84()));
    const Camera camera(center.y(), center.x(), 600.0, 120.0, 60.0, 0.0);
    m_arcGISArView->setOriginCamera(camera);
  });
  m_arcGISArView->setTranslationFactor(1000);

  // set scene
  m_sceneView->setArcGISScene(m_scene);
}
