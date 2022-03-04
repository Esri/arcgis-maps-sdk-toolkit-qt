
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTER_H

#include <QFrame>

namespace Ui {
  class FloorFilter;
}

namespace Esri {
namespace ArcGISRuntime {

  class MapGraphicsView;
  class SceneGraphicsView;

  namespace Toolkit {
    class FloorFilterController;

    class FloorFilter : public QFrame
    {
      Q_OBJECT
    public:
      Q_INVOKABLE explicit FloorFilter(QWidget* parent = nullptr);

      ~FloorFilter() override;

      void setMapView(MapGraphicsView* mapView);

      void setSceneView(SceneGraphicsView* sceneView);

      FloorFilterController* controller() const;

    private:
      FloorFilterController* m_controller = nullptr;
      Ui::FloorFilter* m_ui;
      bool m_sitesUpdatedFromController{false};
      bool m_facilitiesUpdatedFromController{false};
    };
  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FLOORFILTER_H
