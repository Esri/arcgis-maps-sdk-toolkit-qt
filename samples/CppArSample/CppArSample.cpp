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
#include "PointCloudLayer.h"
#include "PortalItem.h"
#include "IntegratedMeshLayer.h"

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
// from https://github.com/Esri/arcgis-runtime-toolkit-ios/blob/mhd/ARExampleUpdates/Examples/ArcGISToolkitExamples/ARExample.swift#L303

// Creates an empty scene with an elevation source.
// Mode: Full-Scale AR
void CppArSample::createEmptyScene()
{
  Q_CHECK_PTR(m_sceneView);

  if (m_scene)
    delete m_scene;

  m_scene = new Scene(this);
  createSurfaceWithElevation();
  m_sceneView->setArcGISScene(m_scene);
}

// Creates a scene based on the Streets base map.
// Mode: Full-Scale AR
void CppArSample::createStreetsScene()
{
  Q_CHECK_PTR(m_sceneView);

  if (m_scene)
    delete m_scene;

  m_scene = new Scene(BasemapType::Streets, this);
  createSurfaceWithElevation();
  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);
  m_sceneView->setArcGISScene(m_scene);
}

// Creates a scene based on a point cloud layer.
// Mode: Tabletop AR
void CppArSample::createPointCloudScene()
{
  if (m_scene)
    delete m_scene;

  m_scene = new Scene(BasemapType::ImageryWithLabels, this);
  createSurfaceWithElevation();

  PortalItem* item = new PortalItem("fc3f4a4919394808830cd11df4631a54", m_scene);
  PointCloudLayer* layer = new PointCloudLayer(item, m_scene);
  m_scene->operationalLayers()->append(layer);

  connect(layer, &PointCloudLayer::doneLoading, this, [this, layer](Error error)
  {
    if (!error.isEmpty())
    {
      qDebug() << "Fails to load the point cloud scene:" << error.message() << error.additionalMessage();
      return;
    }

    // Create the origin camera at the center point of the data. This will ensure the data is anchored to the table.
    const Point center = layer->fullExtent().center();
    m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 0, 0, 0, 0));
  });

  m_arcGISArView->setTranslationFactor(2000.0);
  m_sceneView->setArcGISScene(m_scene);
}

// Creates a scene centered on Yosemite National Park.
// Mode: Tabletop AR
void CppArSample::createYosemiteScene()
{
  if (m_scene)
    delete m_scene;

  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl yosemiteUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                         "VRICON_Yosemite_Sample_Integrated_Mesh_scene_layer/SceneServer");
  IntegratedMeshLayer* layer = new IntegratedMeshLayer(yosemiteUrl, m_scene);
  m_scene->operationalLayers()->append(layer);

  connect(layer, &PointCloudLayer::doneLoading, this, [this, layer](Error error)
  {
    if (!error.isEmpty())
    {
      qDebug() << "Fails to load the Yosemite scene:" << error.message() << error.additionalMessage();
      return;
    }

    // Get the center point of the layer's extent.
    const Point center = layer->fullExtent().center();

    // Find the elevation of the layer at the center point.
    Surface* surface = m_scene->baseSurface();
    connect(surface, &Surface::locationToElevationCompleted, this, [this, center](QUuid, double elevation)
    {
      // Create the origin camera at the center point and elevation of the data.
      // This will ensure the data is anchored to the table.
      m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), elevation, 0, 0, 0));

    });
    surface->locationToElevation(center);
  });

  m_arcGISArView->setTranslationFactor(1000.0);
  m_sceneView->setArcGISScene(m_scene);
}

// Creates a scene centered the US-Mexico border.
// Mode: Tabletop AR
void CppArSample::createBorderScene()
{
  if (m_scene)
    delete m_scene;

  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl borderUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                       "VRICON_SW_US_Sample_Integrated_Mesh_scene_layer/SceneServer");
  IntegratedMeshLayer* layer = new IntegratedMeshLayer(borderUrl, m_scene);
  m_scene->operationalLayers()->append(layer);

  connect(layer, &PointCloudLayer::doneLoading, this, [this, layer](Error error)
  {
    if (!error.isEmpty())
    {
      qDebug() << "Fails to load the border scene:" << error.message() << error.additionalMessage();
      return;
    }

    // Get the center point of the layer's extent.
    const Point center = layer->fullExtent().center();

    // Find the elevation of the layer at the center point.
    Surface* surface = m_scene->baseSurface();
    connect(surface, &Surface::locationToElevationCompleted, this, [this, center](QUuid, double elevation)
    {
      // Create the origin camera at the center point and elevation of the data.
      // This will ensure the data is anchored to the table.
      m_arcGISArView->setOriginCamera(Camera(center.x(), center.y(), elevation, 0, 0, 0));
    });
    surface->locationToElevation(center);
  });

  m_arcGISArView->setTranslationFactor(1000.0);
  m_sceneView->setArcGISScene(m_scene);
}

void CppArSample::createBrestScene()
{
  Q_CHECK_PTR(m_sceneView);

  if (m_scene)
    delete m_scene;

  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // create layer
  const QUrl brestFrance("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                         "Buildings_Brest/SceneServer/layers/0");
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(brestFrance, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  const Camera observerCamera(Point(-4.49492, 48.3808, 48.2511, SpatialReference::wgs84()), 344.488, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(observerCamera);
  m_arcGISArView->setTranslationFactor(250.0);

  m_sceneView->setArcGISScene(m_scene);
}

void CppArSample::createBerlinScene()
{
  if (m_scene)
    delete m_scene;

  // from https://github.com/Esri/arcgis-runtime-toolkit-ios/blob/mhd/ARExampleUpdates/Examples/ArcGISToolkitExamples/ARExample.swift#L303

  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // berlin scene
  const QUrl buildingsService("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                              "Buildings_Berlin/SceneServer");
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(buildingsService, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  connect(layer, &ArcGISSceneLayer::doneLoading, this, [this, layer](Error)
  {
    const Point center = geometry_cast<Point>(GeometryEngine::project(layer->fullExtent().center(), SpatialReference::wgs84()));
    const Camera camera(center.y(), center.x(), 600.0, 120.0, 60.0, 0.0);
    m_arcGISArView->setOriginCamera(camera);
  });

  // set scene
  m_sceneView->setArcGISScene(m_scene);
}

void CppArSample::createSurfaceWithElevation()
{
  Q_CHECK_PTR(m_scene);

  // add elevation source
  const QUrl elevationSourceUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
  const QList<ElevationSource*> sources = { new ArcGISTiledElevationSource(elevationSourceUrl, m_scene) };
  Surface* baseSurface = new Surface(sources, m_scene);

  BackgroundGrid grid;
  grid.setVisible(false);
  baseSurface->setBackgroundGrid(grid);
  // baseSurface->setNavigationConstraint(NavigationConstraint::None);
  m_scene->setBaseSurface(baseSurface);
}
