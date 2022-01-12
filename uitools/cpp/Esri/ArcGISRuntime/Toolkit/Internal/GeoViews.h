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

#include <MapGraphicsView.h>
#include <SceneGraphicsView.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  using SceneViewToolkit = SceneGraphicsView;
  using MapViewToolkit = MapGraphicsView;
} // Toolkit
} // ArcGISRuntime
} // Esri

#else

#include <MapQuickView.h>
#include <SceneQuickView.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  using SceneViewToolkit = SceneQuickView;
  using MapViewToolkit = MapQuickView;
} // Toolkit
} // ArcGISRuntime
} // Esri

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  inline Map* getGeoModel(MapViewToolkit* mapView)
  {
    return mapView ? mapView->map() : nullptr;
  }

  inline Scene* getGeoModel(SceneViewToolkit* sceneView)
  {
    return sceneView ? sceneView->arcGISScene() : nullptr;
  }

  inline auto getGeoModelChangedSignal(MapViewToolkit* /*mapView*/)
  {
    return &MapViewToolkit::mapChanged;
  }

  inline auto getGeoModelChangedSignal(SceneViewToolkit* /*sceneView*/)
  {
    return &SceneViewToolkit::sceneChanged;
  }
} // Toolkit
} // ArcGISRuntime
} // Esri

#endif

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GEOVIEWS_H
