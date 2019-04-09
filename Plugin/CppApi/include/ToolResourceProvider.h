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

#ifndef TOOL_RESOURCE_PROVIDER_H
#define TOOL_RESOURCE_PROVIDER_H

#include <QObject>

#include "ToolkitCommon.h"
#include "Point.h"
#include <QMouseEvent>
#include <QUuid>
#include <QCursor>

namespace Esri
{
namespace ArcGISRuntime
{

class Basemap;
class GeoView;
class Map;
class LayerListModel;
class Scene;
class SpatialReference;
class IdentifyGraphicsOverlayResult;
class IdentifyLayerResult;

namespace Toolkit
{

class TOOLKIT_EXPORT ToolResourceProvider : public QObject
{
  Q_OBJECT

public:

  static ToolResourceProvider* instance();

  ~ToolResourceProvider() override;

  Map* map() const;
  void setMap(Map* newMap);

  Scene* scene() const;
  void setScene(Scene* newScene);

  GeoView* geoView() const;
  void setGeoView(GeoView* newGeoView);

  SpatialReference spatialReference() const;

  LayerListModel* operationalLayers() const;

  void setBasemap(Basemap* newBasemap);

  void setMouseCursor(const QCursor& cursor);

  void clear();

public slots:
  void onMouseClicked(QMouseEvent& mouseEvent);
  void onMousePressed(QMouseEvent& mouseEvent);
  void onMouseMoved(QMouseEvent& mouseEvent);
  void onMouseReleased(QMouseEvent& mouseEvent);
  void onMousePressedAndHeld(QMouseEvent& mouseEvent);
  void onMouseDoubleClicked(QMouseEvent& mouseEvent);
  void onIdentifyGraphicsOverlayCompleted(QUuid taskId, IdentifyGraphicsOverlayResult* identifyResult);
  void onIdentifyGraphicsOverlaysCompleted(QUuid taskId, QList<IdentifyGraphicsOverlayResult*> identifyResults);
  void onIdentifyLayerCompleted(QUuid taskId, IdentifyLayerResult* identifyResult);
  void onIdentifyLayersCompleted(QUuid taskId, QList<IdentifyLayerResult*> identifyResults);
  void onScreenToLocationCompleted(QUuid taskId, const Point& location);
  void onLocationChanged(const Point& location);

signals:
  void sceneChanged();
  void geoViewChanged();
  void mapChanged();
  void spatialReferenceChanged();
  void identifyGraphicsOverlayCompleted(QUuid taskId, IdentifyGraphicsOverlayResult* identifyResult);
  void identifyGraphicsOverlaysCompleted(QUuid taskId, QList<IdentifyGraphicsOverlayResult*> identifyResults);
  void identifyLayerCompleted(QUuid taskId, IdentifyLayerResult* identifyResult);
  void identifyLayersCompleted(QUuid taskId, QList<IdentifyLayerResult*> identifyResults);
  void setMouseCursorRequested(const QCursor& cursor);
  void screenToLocationCompleted(QUuid taskId, const Point& location);
  void locationChanged(const Point& location);

  // mouse signals
  void mouseClicked(QMouseEvent& mouseEvent);
  void mouseClickedPoint(const Point& point);
  void mousePressed(QMouseEvent& mouseEvent);
  void mousePressedPoint(const Point& point);
  void mouseMoved(QMouseEvent& mouseEvent);
  void mouseMovedPoint(const Point& point);
  void mouseReleased(QMouseEvent& mouseEvent);
  void mouseReleasedPoint(const Point& point);
  void mousePressedAndHeld(QMouseEvent& mouseEvent);
  void mousePressedAndHeldPoint(const Point& point);
  void mouseDoubleClicked(QMouseEvent& mouseEvent);
  void mouseDoubleClickedPoint(const Point& point);

private:
  explicit ToolResourceProvider(QObject* parent = nullptr);

  GeoView* m_geoView = nullptr;
  Map* m_map = nullptr;
  Scene* m_scene = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TOOL_RESOURCE_PROVIDER_H
