#ifndef TOOLKIT_GEOVIEWS_H
#define TOOLKIT_GEOVIEWS_H

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

#endif // TOOLKIT_GEOVIEWS_H
