// Copyright 2016 ESRI
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

namespace Toolkit
{

class TOOLKIT_EXPORT ToolResourceProvider : public QObject
{
  Q_OBJECT

public:

  static ToolResourceProvider* instance();

  ~ToolResourceProvider();

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
  void onIdentifyGraphicsOverlayCompleted(QUuid id, IdentifyGraphicsOverlayResult* identifyResult);
  void onScreenToLocationCompleted(QUuid taskId, const Point& location);

signals:
  void sceneChanged();
  void geoViewChanged();
  void mapChanged();
  void spatialReferenceChanged();
  void identifyGraphicsOverlayCompleted(QUuid id, IdentifyGraphicsOverlayResult* identifyResult);
  void setMouseCursorRequested(const QCursor& cursor);
  void screenToLocationCompleted(QUuid taskId, const Point& location);

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
