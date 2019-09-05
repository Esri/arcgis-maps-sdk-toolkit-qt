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

#include "CppArExample.h"

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
#include "LocationDataSource.h"

#include <QUrl>
#include <QFileInfo>
#include <QStandardPaths>

#include <QGeoPositionInfoSource>

#include "SimpleMarkerSceneSymbol.h"
#include "SimpleLineSymbol.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

CppArExample::CppArExample(QObject* parent):
  QObject(parent)
{
}

CppArExample::~CppArExample()
{
}

ArcGISArView* CppArExample::arcGISArView() const
{
  return m_arcGISArView;
}

void CppArExample::setArcGISArView(ArcGISArView* arcGISArView)
{
  if (!arcGISArView || arcGISArView == m_arcGISArView)
    return;

  m_arcGISArView = arcGISArView;
  emit arcGISArViewChanged();
}

SceneQuickView* CppArExample::sceneView() const
{
  return m_sceneView;
}

// Set the view (created in QML)
void CppArExample::setSceneView(SceneQuickView* sceneView)
{
  if (!sceneView || sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  emit sceneViewChanged();
}

// Creates an empty scene with an elevation source.
// Mode: Full-Scale AR
void CppArExample::createEmptyScene()
{
  // create an empty scene with elevation
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}

// Creates a scene based on the Streets base map.
// Mode: Full-Scale AR
void CppArExample::createStreetsScene()
{
  // create a scene with streets
  m_scene = new Scene(BasemapType::Streets, this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}

// Creates a scene based on the ImageryWithLabels base map.
// Mode: Full-Scale AR
void CppArExample::createImageryScene()
{
  // create a scene with imagery
  m_scene = new Scene(BasemapType::ImageryWithLabels, this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}



// Creates a test scene
// Mode: Full-Scale AR
void CppArExample::createFullScaleTestScene()
{
  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // create graphic overlay
  auto* graphicsOverlay = new GraphicsOverlay(this);
  Q_CHECK_PTR(m_sceneView);
  m_sceneView->graphicsOverlays()->append(graphicsOverlay);

  // create graphics
  auto createSymbol = [graphicsOverlay](double x, double y, const QColor& color)
  {
    SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(
          SimpleMarkerSceneSymbolStyle::Sphere, color, 0.1, 0.1, 0.1, SceneSymbolAnchorPosition::Bottom, graphicsOverlay);
    graphicsOverlay->graphics()->append(new Graphic(Point(x, y, 0.00001), symbol, graphicsOverlay));
  };

  for (int i = -30; i <= 30; ++i)
  {
    for (int j = -30; j <= 30; ++j)
    {
      createSymbol(0.00001 * i, 0.00001 * j, Qt::blue);
    }
  }
  createSymbol(0.0, 0.0, Qt::green);

  m_arcGISArView->setOriginCamera(Camera(0.0, 0.0, 0.0, 0.0, 90.0, 0.0));






//  auto createSymbol2 = [graphicsOverlay](double x1, double y1, double x2, double y2, const QColor& color)
//  {
//    SimpleLineSymbol* symbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, color, 500.0f, graphicsOverlay);

//    graphicsOverlay->graphics()->append(new Graphic(Point(x1, y1, 0.00001), symbol, graphicsOverlay));
//    graphicsOverlay->graphics()->append(new Graphic(Point(x2, y2, 0.00001), symbol, graphicsOverlay));
//  };

//  for (int i = -10; i <= 10; ++i)
//  {
//    createSymbol2(0.00001 * i, -0.0001, 0.00001 * i, 0.0001, Qt::blue);
//    createSymbol2(-0.0001, 0.00001 * i, 0.0001, 0.00001 * i, Qt::red);
//  }



  // Set the location data source so we use our GPS location as the originCamera.
//  m_arcGISArView->setOriginCamera(Camera());
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(false);
}





// Creates a scene based on a point cloud layer.
// Mode: Tabletop AR
void CppArExample::createPointCloudScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  PortalItem* item = new PortalItem("fc3f4a4919394808830cd11df4631a54", m_scene);
  auto* layer = new PointCloudLayer(item, m_scene);
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
    m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 300.0, 0.0, 90.0, 0.0));
  });

  m_arcGISArView->setTranslationFactor(2000.0);

  changeScene();
}

// Creates a scene centered on Yosemite National Park.
// Mode: Tabletop AR
void CppArExample::createYosemiteScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl yosemiteUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                         "VRICON_Yosemite_Sample_Integrated_Mesh_scene_layer/SceneServer");
  auto* layer = new IntegratedMeshLayer(yosemiteUrl, m_scene);
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
      qDebug() << "======>>> " << elevation;
      m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 200.0 /*elevation*/, 0.0, 90.0, 0.0));
    });
    surface->locationToElevation(center);
  });

  m_arcGISArView->setTranslationFactor(1000.0);

  changeScene();
}

// Creates a scene centered on the US-Mexico border.
// Mode: Tabletop AR
void CppArExample::createBorderScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl borderUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                       "VRICON_SW_US_Sample_Integrated_Mesh_scene_layer/SceneServer");
  auto* layer = new IntegratedMeshLayer(borderUrl, m_scene);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  m_arcGISArView->setOriginCamera(Camera(32.5337, -116.925, 126.0, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(1000.0);

  changeScene();
}

// Creates a scene centered on Brest (France)
// Mode: Tabletop AR
void CppArExample::createBrestScene()
{
  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // create layer
  const QUrl brestFrance("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                         "Buildings_Brest/SceneServer/layers/0");
  auto* layer = new ArcGISSceneLayer(brestFrance, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  m_arcGISArView->setOriginCamera(Camera(48.3808, -4.49492, 48.2511, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(250.0);

  changeScene();
}

// Creates a scene centered on Berlin (Germany)
// Mode: Tabletop AR
void CppArExample::createBerlinScene()
{
  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // berlin scene
  const QUrl buildingsService("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                              "Buildings_Berlin/SceneServer");
  auto* layer = new ArcGISSceneLayer(buildingsService, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  m_arcGISArView->setOriginCamera(Camera(52.4993, 13.4215, 38.0, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(1000.0);

  changeScene();
}

// Creates a test scene
// Mode: Tabletop AR
void CppArExample::createTabletopTestScene()
{
  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // create graphic overlay
  auto* graphicsOverlay = new GraphicsOverlay(this);
  Q_CHECK_PTR(m_sceneView);
  m_sceneView->graphicsOverlays()->append(graphicsOverlay);

  // create graphics
  auto createSymbol = [graphicsOverlay](double x, double y, const QColor& color)
  {
    SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(
          SimpleMarkerSceneSymbolStyle::Sphere, color, 0.1, 0.1, 0.1, SceneSymbolAnchorPosition::Bottom, graphicsOverlay);
    graphicsOverlay->graphics()->append(new Graphic(Point(x, y, 0.0), symbol, graphicsOverlay));
  };

  for (int i = -5; i <= 10; ++i)
  {
    createSymbol(0.0, 0.000001 * i, Qt::blue);
    createSymbol(0.000001 * i, 0.0, Qt::red);
  }
  createSymbol(0.0, 0.0, Qt::green);

  m_arcGISArView->setOriginCamera(Camera(0.0, 0.0, 0.0, 0.0, 90.0, 0.0));
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene();
}

// create and add a surface with elevation to the scene.
void CppArExample::createSurfaceWithElevation()
{
  Q_CHECK_PTR(m_scene);

  // add elevation source
  const QUrl elevationSourceUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
  const QList<ElevationSource*> sources = { new ArcGISTiledElevationSource(elevationSourceUrl, m_scene) };
  auto* baseSurface = new Surface(sources, m_scene);

  BackgroundGrid grid;
  grid.setVisible(false);
  baseSurface->setBackgroundGrid(grid);
  baseSurface->setNavigationConstraint(NavigationConstraint::None);
  m_scene->setBaseSurface(baseSurface);
}

// change the current scene and delete the old one.
void CppArExample::changeScene(bool withLocationDataSource)
{
  Q_CHECK_PTR(m_sceneView);

  // stop tracking
  m_arcGISArView->stopTracking();

  // update the location data source
  LocationDataSource* oldLocationDataSource = m_arcGISArView->locationDataSource();
  if (withLocationDataSource)
  {
    if (!oldLocationDataSource)
      m_arcGISArView->setLocationDataSource(new LocationDataSource(this));
    // else do nothing
  }
  else
  {
    m_arcGISArView->setLocationDataSource(nullptr);
    delete oldLocationDataSource;
  }

  // set the new scene
  Scene* oldScene = m_sceneView->arcGISScene();
  m_sceneView->setArcGISScene(m_scene);
  delete oldScene;

  // reset and start tracking
  m_arcGISArView->resetTracking();
  m_arcGISArView->startTracking();
}
