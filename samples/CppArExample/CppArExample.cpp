/*******************************************************************************
 *  Copyright 2012-2019 Esri
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

// Update the origin camera using the values send by the calibration view.
// The m_originCamera is set when the scene is created. The origin camera sets to ArcGISArView is
// this m_originCamera updated with the offset values returned by the calibration view.
void CppArExample::updateOriginCamera(double latitude, double longitude, double altitude, double heading)
{
  const Camera newCamera(m_originCamera.location().y() + latitude,
                         m_originCamera.location().x() + longitude,
                         m_originCamera.location().z() + altitude,
                         m_originCamera.heading() + heading,
                         m_originCamera.pitch(),
                         m_originCamera.roll());
  m_arcGISArView->setOriginCamera(newCamera);
}

// Set whether point clouds should be visible.
void CppArExample::showPointCloud(bool visible)
{
  if (visible)
    m_arcGISArView->setPointCloudColor(QColor(50, 50, 255));
  else
    m_arcGISArView->setPointCloudColor(QColor());
}

// Set whether planes should be visible.
void CppArExample::showPlanes(bool visible)
{
  if (visible)
    m_arcGISArView->setPlaneColor(QColor(255, 0, 0, 10 ));
  else
    m_arcGISArView->setPlaneColor(QColor());
}

// Create an empty scene with an elevation source.
// Mode: Full-Scale AR
void CppArExample::createEmptyScene()
{
  // Create an empty scene with elevation
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_originCamera = Camera();
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}

// Create a scene based on the Streets base map.
// Mode: Full-Scale AR
void CppArExample::createStreetsScene()
{
  // Create a scene with streets
  m_scene = new Scene(BasemapType::Streets, this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_originCamera = Camera();
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}

// Create a scene based on the ImageryWithLabels base map.
// Mode: Full-Scale AR
void CppArExample::createImageryScene()
{
  // Create a scene with imagery
  m_scene = new Scene(BasemapType::ImageryWithLabels, this);
  createSurfaceWithElevation();

  // Set the location data source so we use our GPS location as the originCamera.
  m_originCamera = Camera();
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(true);
}

// Create a test scene
// Mode: Full-Scale AR
void CppArExample::createFullScaleTestScene()
{
  // Create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Create graphic overlay
  auto* graphicsOverlay = new GraphicsOverlay(this);
  Q_CHECK_PTR(m_sceneView);
  m_sceneView->graphicsOverlays()->append(graphicsOverlay);

  // Create graphics
  auto createSymbol = [graphicsOverlay](double x, double y, const QColor& color)
  {
    SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(
          SimpleMarkerSceneSymbolStyle::Sphere, color, 0.1, 0.1, 0.1, SceneSymbolAnchorPosition::Center, graphicsOverlay);
    graphicsOverlay->graphics()->append(new Graphic(Point(x, y, 0.00001), symbol, graphicsOverlay));
  };

  for (int i = -30; i <= 30; ++i)
  {
    for (int j = -30; j <= 30; ++j)
    {
      createSymbol(0.00001 * i, 0.00001 * j, QColor(Qt::blue));
    }
  }
  createSymbol(0.0, 0.0, QColor(Qt::green));

  // Set the location data source so we use our GPS location as the originCamera.
  m_originCamera = Camera();
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(1.0);

  changeScene(false);
}

// Create a scene based on a point cloud layer.
// Mode: Tabletop AR
void CppArExample::createPointCloudScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  PortalItem* item = new PortalItem("fc3f4a4919394808830cd11df4631a54", m_scene);
  auto* layer = new PointCloudLayer(item, m_scene);
  m_scene->operationalLayers()->append(layer);

  m_originCamera = Camera(39.7712, -74.1197, 1.0, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(18000.0);

  changeScene();
}

// Create a scene centered on Yosemite National Park.
// Mode: Tabletop AR
void CppArExample::createYosemiteScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl yosemiteUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                         "VRICON_Yosemite_Sample_Integrated_Mesh_scene_layer/SceneServer");
  auto* layer = new IntegratedMeshLayer(yosemiteUrl, m_scene);
  m_scene->operationalLayers()->append(layer);

  m_originCamera = Camera(37.7308, -119.612, 1212.0, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(18000.0);

  changeScene();
}

// Create a scene centered on the US-Mexico border.
// Mode: Tabletop AR
void CppArExample::createBorderScene()
{
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  const QUrl borderUrl("https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/"
                       "VRICON_SW_US_Sample_Integrated_Mesh_scene_layer/SceneServer");
  auto* layer = new IntegratedMeshLayer(borderUrl, m_scene);
  m_scene->operationalLayers()->append(layer);

  // Set origin camera
  m_originCamera = Camera(32.5337, -116.925, 126.0, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(10000.0);

  changeScene();
}

// Create a scene centered on Brest (France)
// Mode: Tabletop AR
void CppArExample::createBrestScene()
{
  // Create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Create layer
  const QUrl brestFrance("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                         "Buildings_Brest/SceneServer/layers/0");
  auto* layer = new ArcGISSceneLayer(brestFrance, this);
  m_scene->operationalLayers()->append(layer);

  // Set origin camera
  m_originCamera = Camera(48.3808, -4.49492, 48.2511, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(500.0);

  changeScene();
}

// Create a scene centered on Berlin (Germany)
// Mode: Tabletop AR
void CppArExample::createBerlinScene()
{
  // Create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Create berlin layer
  const QUrl buildingsService("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/"
                              "Buildings_Berlin/SceneServer");
  auto* layer = new ArcGISSceneLayer(buildingsService, this);
  m_scene->operationalLayers()->append(layer);

  // set origin camera
  m_originCamera = Camera(52.4993, 13.4215, 38.0, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(1000.0);

  changeScene();
}

// Create a test scene
// Mode: Tabletop AR
void CppArExample::createTabletopTestScene()
{
  // Create scene
  m_scene = new Scene(this);
  createSurfaceWithElevation();

  // Create graphic overlay
  auto* graphicsOverlay = new GraphicsOverlay(this);
  Q_CHECK_PTR(m_sceneView);
  m_sceneView->graphicsOverlays()->append(graphicsOverlay);

  // Create graphics
  auto createSymbol = [graphicsOverlay](double x, double y, double size, const QColor& color)
  {
    SimpleMarkerSceneSymbol* symbol = new SimpleMarkerSceneSymbol(
          SimpleMarkerSceneSymbolStyle::Sphere, color, size, size, size, SceneSymbolAnchorPosition::Center, graphicsOverlay);
    graphicsOverlay->graphics()->append(new Graphic(Point(x, y, 0.0), symbol, graphicsOverlay));
  };

  for (int i = -5; i <= 10; ++i)
  {
    createSymbol(0.0, 0.000001 * i, 0.1, QColor(Qt::blue));
    createSymbol(0.000001 * i, 0.0, 0.1, QColor(Qt::red));
  }
  createSymbol(0.0, 0.0, 0.11, QColor(Qt::green));

  // set origin camera
  m_originCamera = Camera(0.0, 0.0, 0.0, 0.0, 90.0, 0.0);
  m_arcGISArView->setOriginCamera(m_originCamera);
  m_arcGISArView->setTranslationFactor(10.0);

  changeScene();
}

// Create and add a surface with elevation to the scene.
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

// Change the current scene and delete the old one.
void CppArExample::changeScene(bool withLocationDataSource)
{
  Q_CHECK_PTR(m_sceneView);

  // Update the location data source
  LocationDataSource* oldLocationDataSource = m_arcGISArView->locationDataSource();
  if (withLocationDataSource)
  {
    if (!oldLocationDataSource)
      m_arcGISArView->setLocationDataSource(new LocationDataSource(this));
    // Else do nothing
  }
  else
  {
    m_arcGISArView->setLocationDataSource(nullptr);
    delete oldLocationDataSource;
  }

  // Set the new scene
  Scene* oldScene = m_sceneView->arcGISScene();
  m_sceneView->setArcGISScene(m_scene);
  delete oldScene;

  // Reset tracking
  if (m_arcGISArView->tracking())
    m_arcGISArView->resetTracking();
  else
    m_arcGISArView->startTracking();
}
