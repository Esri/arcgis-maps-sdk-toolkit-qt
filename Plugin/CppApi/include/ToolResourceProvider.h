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

namespace Esri
{
namespace ArcGISRuntime
{

class Basemap;
class GeoView;
class Map;
class LayerListModel;
class Scene;

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

  LayerListModel* operationalLayers() const;

  void setBasemap(Basemap* newBasemap);

  void clear();

signals:
  void sceneChanged();
  void geoViewChanged();
  void mapChanged();

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
