/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
