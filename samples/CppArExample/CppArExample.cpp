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
#include "TextSymbol.h"

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

  m_arcGISArView->startTracking();
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

// Creates a scene based on a point cloud layer.
// Mode: Tabletop AR
void CppArExample::createPointCloudScene()
{
  m_scene = new Scene(this);
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
    m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 300.0, 0.0, 90.0, 0.0));
  });

  m_arcGISArView->setTranslationFactor(2000.0);

  changeScene();
}

// Creates a scene centered on Yosemite National Park.
// Mode: Tabletop AR
void CppArExample::createYosemiteScene()
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
      m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 2000.0 /*elevation*/, 0.0, 90.0, 0.0));
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
      m_arcGISArView->setOriginCamera(Camera(center.y(), center.x(), 700.0, 0.0, 90.0, 0.0));
    });
    surface->locationToElevation(center);
  });

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
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(brestFrance, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  const Camera observerCamera(Point(-4.49492, 48.3808, 48.2511, SpatialReference::wgs84()), 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(observerCamera);
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
  ArcGISSceneLayer* layer = new ArcGISSceneLayer(buildingsService, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  connect(layer, &ArcGISSceneLayer::doneLoading, this, [this, layer](Error)
  {
    const Point center = layer->fullExtent().center();
    const Camera camera(center.y(), center.x(), 10000.0, 0.0, 90.0, 0.0);
    m_arcGISArView->setOriginCamera(camera);
  });

  m_arcGISArView->setTranslationFactor(1.0);

  changeScene();
}

// Creates a test scene
// Mode: Tabletop AR
void CppArExample::createTestScene()
{
  // create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // create symbols
  SimpleMarkerSceneSymbolStyle style = SimpleMarkerSceneSymbolStyle::Sphere;
  double symbolSize = 10.0;
  SceneSymbolAnchorPosition anchorPosition = SceneSymbolAnchorPosition::Bottom;

  // create graphic overlay
  GraphicsOverlay* graphicsOverlay = new GraphicsOverlay(this);
  Q_CHECK_PTR(m_sceneView);
  m_sceneView->graphicsOverlays()->append(graphicsOverlay);

  // create graphics
  double offset = 0.0001;
  for (int i = -5; i <= 10; ++i)
  {
    {
      SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(style, QColor(Qt::blue), symbolSize, symbolSize,
                                                                    symbolSize, anchorPosition, this);
      graphicsOverlay->graphics()->append(new Graphic(Point(0.0, offset * i, 0.0), symbol, this));
    }
    {
      SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(style, QColor(Qt::red), symbolSize, symbolSize,
                                                                    symbolSize, anchorPosition, this);
      graphicsOverlay->graphics()->append(new Graphic(Point(offset * i, 0.0, 0.0), symbol, this));
    }
    {
      SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(style, QColor(Qt::green), symbolSize, symbolSize,
                                                                    symbolSize, anchorPosition, this);
      graphicsOverlay->graphics()->append(new Graphic(Point(0.0, 0.0, offset * i), symbol, this));
    }
  }

  {
    TextSymbol* symbol = new TextSymbol("N", QColor(Qt::blue), symbolSize * 150, HorizontalAlignment::Center,
                                        VerticalAlignment::Middle, this);
    graphicsOverlay->graphics()->append(new Graphic(Point(0.0, offset * 12, 0.0), symbol, this));
  }
  {
    TextSymbol* symbol = new TextSymbol("S", QColor(Qt::blue), symbolSize * 150, HorizontalAlignment::Center,
                                        VerticalAlignment::Middle, this);
    graphicsOverlay->graphics()->append(new Graphic(Point(0.0, -offset * 7, 0.0), symbol, this));
  }
  {
    TextSymbol* symbol = new TextSymbol("E", QColor(Qt::red), symbolSize * 150, HorizontalAlignment::Center,
                                        VerticalAlignment::Middle, this);
    graphicsOverlay->graphics()->append(new Graphic(Point(offset * 12, 0.0, 0.0), symbol, this));
  }
  {
    TextSymbol* symbol = new TextSymbol("W", QColor(Qt::red), symbolSize * 150, HorizontalAlignment::Center,
                                        VerticalAlignment::Middle, this);
    graphicsOverlay->graphics()->append(new Graphic(Point(-offset * 7, 0.0, 0.0), symbol, this));
  }

  m_arcGISArView->setOriginCamera(Camera(0.0, 0.0, 200.0, 0.0, 0.0, 0.0));
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
  Surface* baseSurface = new Surface(sources, m_scene);

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
  LocationDataSource* oldLds = m_arcGISArView->locationDataSource();

  if (withLocationDataSource)
  {
    if (!oldLds)
      m_arcGISArView->setLocationDataSource(new LocationDataSource(this));
    // else do nothing
  }
  else
  {
    m_arcGISArView->setLocationDataSource(nullptr);
    delete oldLds;
  }

  // set the new scene
  Scene* oldScene = m_sceneView->arcGISScene();

  m_sceneView->setArcGISScene(m_scene);

  if (oldScene)
    delete oldScene;

  // reset and start tracking
  m_arcGISArView->resetTracking();
  m_arcGISArView->startTracking();
}
