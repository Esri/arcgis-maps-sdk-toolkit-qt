// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
#include "TimeSliderDemo.h"

#include "ArcGISMapImageLayer.h"
#include "Map.h"
#include "Scene.h"

using namespace Esri::ArcGISRuntime;

namespace {
  const char* dataUrl = "https://sampleserver6.arcgisonline.com/arcgis/rest/services/Hurricanes/MapServer";
}

TimeSliderDemo::TimeSliderDemo(QObject* parent) :
  BaseDemo(parent)
{
}

TimeSliderDemo::~TimeSliderDemo()
{
}

Map* TimeSliderDemo::initMap_(QObject* parent) const
{
  auto map = new Map(BasemapStyle::ArcGISDarkGray, parent);
  map->operationalLayers()->append(new ArcGISMapImageLayer(QUrl{dataUrl}, map));
  return map;
}

Scene* TimeSliderDemo::initScene_(QObject* parent) const
{
  auto scene = new Scene(BasemapStyle::ArcGISDarkGray, parent);
  scene->operationalLayers()->append(new ArcGISMapImageLayer(QUrl{dataUrl}, scene));
  return scene;
}
