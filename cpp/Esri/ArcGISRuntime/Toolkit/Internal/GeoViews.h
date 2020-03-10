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
/*!
* \internal
* \file GeoViews.h
* This file is an implementation detail for some controllers in the Toolkit.
* A [Map|Scene]GraphicsView has the same interface as a [Map|Scene]QuickView,
* but they implement these interfaces independently due to Qt limitations with
* multiple inheritance.
*
* In addition, when the toolkit supports the Quick workflow we
* depend on the Qt QML module. When the toolkit supports the widgets workflow
* we depends on the Qt Widgets module. A user who writes a Widgets app should
* not be forced to include Quick as a dependency or vice versa.
*
* This include will include the correct set of Views depending on 
* whether flag \c WIDGETS_ARCGISRUNTIME_TOOLKIT is defined or not.
*
* Implicitly, this means we assume the user will not need a combination of
* Widget and Quick GeoViews in their app at the same time, but this is an
* unlikely scenario -- even if the user had both quick and widgets in
* their app, they would still likely default to one set of views, e.g.
* [Map|Scene]QuickView.
*/
#ifdef WIDGETS_ARCGISRUNTIME_TOOLKIT
#  include <MapGraphicsView.h>
#  include <SceneGraphicsView.h>

  namespace Esri { namespace ArcGISRuntime { namespace Toolkit {
    using SceneViewToolkit = SceneGraphicsView;
    using MapViewToolkit = MapGraphicsView;
  } } }
#else 
#  include <MapQuickView.h>
#  include <SceneQuickView.h>

  namespace Esri { namespace ArcGISRuntime { namespace Toolkit {
    using SceneViewToolkit = SceneQuickView;
    using MapViewToolkit = MapQuickView;
  } } }
#endif

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GEOVIEWS_H
