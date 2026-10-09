/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GEOVIEWS_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GEOVIEWS_H

#ifdef WIDGETS_ARCGISRUNTIME_TOOLKIT

#include <LocalSceneWidget.h>
#include <MapGraphicsView.h>
#include <MapWidget.h>
#include <SceneGraphicsView.h>
#include <SceneWidget.h>

namespace Esri::ArcGISRuntime::Toolkit
{
  using SceneViewToolkit = SceneGraphicsView;
  using LocalSceneViewToolkit = LocalSceneWidget;
  using MapViewToolkit = MapGraphicsView;
  using SceneWidgetToolkit = SceneWidget;
  using MapWidgetToolkit = MapWidget;
} // namespace Esri::ArcGISRuntime::Toolkit

#else

#include <LocalSceneQuickView.h>
#include <MapQuickView.h>
#include <SceneQuickView.h>

namespace Esri::ArcGISRuntime::Toolkit
{
  using SceneViewToolkit = SceneQuickView;
  using LocalSceneViewToolkit = LocalSceneQuickView;
  using MapViewToolkit = MapQuickView;
} // namespace Esri::ArcGISRuntime::Toolkit

#endif

namespace Esri::ArcGISRuntime::Toolkit
{
  // Share model access across native widgets, graphics views, and Quick views without extra build guards.
  // The decltype return types select the overload supported by the concrete view.
  template<typename GeoViewType>
  inline auto getGeoModel(GeoViewType* geoView) -> decltype(geoView->map())
  {
    return geoView ? geoView->map() : nullptr;
  }

  template<typename GeoViewType>
  inline auto getGeoModel(GeoViewType* geoView) -> decltype(geoView->arcGISScene())
  {
    return geoView ? geoView->arcGISScene() : nullptr;
  }

  // Select the available signal while preserving its concrete member-pointer type for typed QObject::connect calls.
  template<typename GeoViewType>
  inline auto getGeoModelChangedSignal(GeoViewType*) -> decltype(&GeoViewType::mapChanged)
  {
    return &GeoViewType::mapChanged;
  }

  template<typename GeoViewType>
  inline auto getGeoModelChangedSignal(GeoViewType*) -> decltype(&GeoViewType::sceneChanged)
  {
    return &GeoViewType::sceneChanged;
  }
} // namespace Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GEOVIEWS_H
