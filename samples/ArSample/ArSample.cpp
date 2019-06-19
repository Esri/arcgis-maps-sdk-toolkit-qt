
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
//

#include "ArSample.h"

#include "ArcGISTiledElevationSource.h"
#include "Basemap.h"
#include "Scene.h"
#include "SceneQuickView.h"
#include "IntegratedMeshLayer.h"
#include "ArcGISSceneLayer.h"
#include "ArcGISArView.h"

#include <QUrl>
#include <QFileInfo>
#include <QStandardPaths>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

ArSample::ArSample(QObject* parent /* = nullptr */):
  QObject(parent)
{
  createScene();
}

ArSample::~ArSample()
{
}

ArcGISArView* ArSample::arcGISArView() const
{
  return m_arcGISArView;
}

void ArSample::setArcGISArView(ArcGISArView* arcGISArView)
{
  if (!arcGISArView || arcGISArView == m_arcGISArView)
    return;

  m_arcGISArView = arcGISArView;

  if (m_sceneView)
    m_arcGISArView->setSceneView(m_sceneView);

  emit arcGISArViewChanged();
}

SceneQuickView* ArSample::sceneView() const
{
  return m_sceneView;
}

// Set the view (created in QML)
void ArSample::setSceneView(SceneQuickView* sceneView)
{
  if (!sceneView || sceneView == m_sceneView)
    return;

  m_sceneView = sceneView;
  m_sceneView->setArcGISScene(m_scene);

  if (m_arcGISArView)
    m_arcGISArView->setSceneView(m_sceneView);

  emit sceneViewChanged();
}

void ArSample::createScene()
{
  // from https://devtopia.esri.com/mort5161/ARSamples/blob/b63c7b62c217d36fa44fb91fd271554061431af5/ArcGISAR.Droid/TestScenes.cs

  // Brest France
  Camera camera(Point(-4.49492, 48.3808, 48.2511, SpatialReference::wgs84()), 344.488, 74.1212, 0.0);
  const QUrl elevationUrl("https://scene.arcgis.com/arcgis/rest/services/BREST_DTM_1M/ImageServer");
  const QUrl layerUrl("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Brest/SceneServer/layers/0");
  auto layer = new ArcGISSceneLayer(layerUrl, this);

  // Berlin
  //  const Point targetExtent(-4.49492, 48.3808, 48.2511, SpatialReference::wgs84());
  //  const Camera camera(targetExtent, 344.488, 74.1212, 0.0);
  //  const QUrl elevationUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
  //  const QUrl layerUrl("https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_Berlin/SceneServer");
  //  auto layer = new ArcGISSceneLayer(layerUrl, this);

  // Barton School House
  //  const Camera camera(7.5965494109812166, 45.978823845553237, 4944.7202749764547, 92.526187357522176, 69.000299172744661, 0.0);
  //  const QUrl elevationUrl("http://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
  //  const QUrl layerUrl = QUrl("/sdcard/ArcGIS/Runtime/UnitTests/slpks/matterhorn_dxt.slpk");
  //  auto layer = new IntegratedMeshLayer(layerUrl, this);

  connect(layer, &Object::errorOccurred, [](Error e)
  {
    qDebug() << "==== error: " << e.message() << e.additionalMessage();
  });

  // create scene
  m_scene = new Scene(this);

  // set initial viewpoint
  const Viewpoint viewpoint(camera.location(), camera);
  m_scene->setInitialViewpoint(viewpoint);

  // set surface
  const QList<ElevationSource*> sources = { new ArcGISTiledElevationSource(elevationUrl, this) };
  Surface* baseSurface = new Surface(sources, this);

  BackgroundGrid grid;
  grid.setVisible(false);
  baseSurface->setBackgroundGrid(grid);

  baseSurface->setNavigationConstraint(NavigationConstraint::None);

  m_scene->setBaseSurface(baseSurface);

  // set layer
  layer->setOpacity(1.0f);

  m_scene->operationalLayers()->append(layer);
}
