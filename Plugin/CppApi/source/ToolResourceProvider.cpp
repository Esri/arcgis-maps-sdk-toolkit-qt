/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#include "Basemap.h"
#include "Map.h"
#include "Scene.h"
#include "GeoView.h"
#include "MapView.h"
#include "SceneView.h"
#include "IdentifyGraphicsOverlayResult.h"

#include "ToolResourceProvider.h"

#include <QUuid>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

ToolResourceProvider::ToolResourceProvider(QObject* parent /*= nullptr*/):
  QObject(parent)
{

}

ToolResourceProvider* ToolResourceProvider::instance()
{
  static ToolResourceProvider s_instance;

  return &s_instance;
}

ToolResourceProvider::~ToolResourceProvider()
{

}

Map* ToolResourceProvider::map() const
{
  return m_map;
}

/*! \brief Sets the map to \a newMap.
 *
 * To clear the map pass \c nullptr.
 *
 * \sa mapChanged.
 */
void ToolResourceProvider::setMap(Map* newMap)
{
  if (newMap == m_map)
    return;

  m_map = newMap;

  emit mapChanged();

  if (m_map == nullptr)
    return;

  connect(m_map, &QObject::destroyed, this, [this]
  {
    m_map = nullptr;
    emit mapChanged();
  });
}

Scene* ToolResourceProvider::scene() const
{
  return m_scene;
}

/*! \brief Sets the scene to \a newScene.
 *
 * To clear the scene pass \c nullptr.
 *
 * \sa sceneChanged.
 */
void ToolResourceProvider::setScene(Scene* newScene)
{
  if (newScene == m_scene)
    return;

  m_scene = newScene;

  emit sceneChanged();

  if (m_scene == nullptr)
    return;

  connect(m_scene, &QObject::destroyed, this, [this]
  {
    m_scene = nullptr;
    emit sceneChanged();
  });
}

GeoView* ToolResourceProvider::geoView() const
{
  return m_geoView;
}

/*! \brief Sets the geoView to \a newGeoView.
 *
 * To clear the geoView pass \c nullptr.
 *
 * \sa geoViewChanged.
 * \sa spatialReferenceChanged.
 */
void ToolResourceProvider::setGeoView(GeoView* newGeoView)
{
  if (newGeoView == m_geoView)
    return;

  m_geoView = newGeoView;
  emit geoViewChanged();

  if (m_geoView == nullptr)
    return;

  if (!m_geoView->spatialReference().isEmpty())
    emit spatialReferenceChanged();

  QObject* geoViewObject = dynamic_cast<QObject*>(m_geoView);
  if (geoViewObject)
  {
    connect(geoViewObject, &QObject::destroyed, this, [this]
    {
      m_geoView = nullptr;
      emit geoViewChanged();
    });
  }
}

SpatialReference ToolResourceProvider::spatialReference() const
{
  return m_geoView ? m_geoView->spatialReference() : SpatialReference();
}

LayerListModel* ToolResourceProvider::operationalLayers() const
{
  if (m_map)
    return m_map->operationalLayers();
  else if (m_scene)
    return m_scene->operationalLayers();

  return nullptr;
}

void ToolResourceProvider::setBasemap(Basemap* newBasemap)
{
  if (!newBasemap)
    return;

  if (m_map)
  {
    newBasemap->setParent(m_map);
    m_map->setBasemap(newBasemap);
  }
  else if (m_scene)
  {
    newBasemap->setParent(m_scene);
    m_scene->setBasemap(newBasemap);
  }
}

void ToolResourceProvider::setMouseCursor(const QCursor& cursor)
{
  emit setMouseCursorRequested(cursor);
}

void ToolResourceProvider::onMouseClicked(QMouseEvent& mouseEvent)
{
  emit mouseClicked(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mouseClickedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mouseClickedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mouseClickedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mouseClickedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onMousePressed(QMouseEvent &mouseEvent)
{
  emit mousePressed(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mousePressedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mousePressedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mousePressedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mousePressedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onMouseMoved(QMouseEvent &mouseEvent)
{
  emit mouseMoved(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mouseMovedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mouseMovedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mouseMovedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mouseMovedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onMouseReleased(QMouseEvent &mouseEvent)
{
  emit mouseReleased(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mouseReleasedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mouseReleasedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mouseReleasedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mouseReleasedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onMousePressedAndHeld(QMouseEvent &mouseEvent)
{
  emit mousePressedAndHeld(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mousePressedAndHeldPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mousePressedAndHeldPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mousePressedAndHeldPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mousePressedAndHeldPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onMouseDoubleClicked(QMouseEvent &mouseEvent)
{
  emit mouseDoubleClicked(mouseEvent);

  if (!m_geoView)
    return;

  if (m_scene)
    emit mouseDoubleClickedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (m_map)
    emit mouseDoubleClickedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<SceneView*>(m_geoView))
    emit mouseDoubleClickedPoint(static_cast<SceneView*>(m_geoView)->screenToBaseSurface(mouseEvent.x(), mouseEvent.y()));
  else if (dynamic_cast<MapView*>(m_geoView))
    emit mouseDoubleClickedPoint(static_cast<MapView*>(m_geoView)->screenToLocation(mouseEvent.x(), mouseEvent.y()));
}

void ToolResourceProvider::onIdentifyGraphicsOverlayCompleted(QUuid id, IdentifyGraphicsOverlayResult* identifyResult)
{
  emit identifyGraphicsOverlayCompleted(id, identifyResult);
}

void ToolResourceProvider::onIdentifyGraphicsOverlaysCompleted(QUuid taskId, QList<IdentifyGraphicsOverlayResult *> identifyResults)
{
  emit identifyGraphicsOverlaysCompleted(taskId, identifyResults);
}

void ToolResourceProvider::onIdentifyLayerCompleted(QUuid taskId, IdentifyLayerResult* identifyResult)
{
  emit identifyLayerCompleted(taskId, identifyResult);
}

void ToolResourceProvider::onIdentifyLayersCompleted(QUuid taskId, QList<IdentifyLayerResult*> identifyResults)
{
  emit identifyLayersCompleted(taskId, identifyResults);
}

void ToolResourceProvider::onScreenToLocationCompleted(QUuid taskId, const Point& location)
{
  emit screenToLocationCompleted(taskId, location);
}

void ToolResourceProvider::onLocationChanged(const Point& location)
{
  emit locationChanged(location);
}

void ToolResourceProvider::clear()
{
  m_map = nullptr;
  m_scene = nullptr;
  m_geoView = nullptr;

  emit mapChanged();
  emit sceneChanged();
  emit geoViewChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
