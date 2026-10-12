/*******************************************************************************
 *  Copyright 2012-2026 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_APPLYTOGEOVIEW_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_APPLYTOGEOVIEW_H

// Qt headers
#include <QObject>

// Other headers
#include "GeoViews.h"

namespace Esri::ArcGISRuntime::Toolkit
{
  template<typename MapFunc, typename SceneFunc>
  inline bool applyToGeoView(QObject* geoView, const MapFunc& mapFunc, const SceneFunc& sceneFunc)
  {
    if (auto* mapView = qobject_cast<MapViewToolkit*>(geoView))
    {
      mapFunc(mapView);
    }
    else if (auto* sceneView = qobject_cast<SceneViewToolkit*>(geoView))
    {
      sceneFunc(sceneView);
    }
    else if (auto* localSceneView = qobject_cast<LocalSceneViewToolkit*>(geoView))
    {
      sceneFunc(localSceneView);
    }
#ifdef WIDGETS_ARCGISRUNTIME_TOOLKIT
    else if (auto* mapWidget = qobject_cast<MapWidgetToolkit*>(geoView))
    {
      mapFunc(mapWidget);
    }
    else if (auto* sceneWidget = qobject_cast<SceneWidgetToolkit*>(geoView))
    {
      sceneFunc(sceneWidget);
    }
#endif
    else
    {
      return false;
    }
    return true;
  }
} // namespace Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_APPLYTOGEOVIEW_H
