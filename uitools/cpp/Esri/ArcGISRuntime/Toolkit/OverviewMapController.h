/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_OVERVIEWMAPCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_OVERVIEWMAPCONTROLLER_H

// ArcGISRUntime Toolkit headers
#include "Internal/GeoViews.h"

// ArcGISRuntime headers
#include "TaskWatcher.h"

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
  class Graphic;
  class Symbol;
}
}

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class OverviewMapController : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
    Q_PROPERTY(QObject* insetView READ insetView CONSTANT)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
  public:
    Q_INVOKABLE OverviewMapController(QObject* parent = nullptr);

    ~OverviewMapController() override;

    MapViewToolkit* insetView() const;

    QObject* geoView() const;
    void setGeoView(QObject* mapView);

    Symbol* symbol() const;
    void setSymbol(Symbol* symbol);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

  signals:
    void geoViewChanged();
    void scaleFactorChanged();
    void symbolChanged();

  private:
    void applyInsetNavigationToMapView(MapViewToolkit* view);
    void applyInsetNavigationToSceneView(SceneViewToolkit* view);

    void applyMapNavigationToInset(MapViewToolkit* view);
    void applySceneNavigationToInset(SceneViewToolkit* viiew);

    void disableInteractions();

  private:
    MapViewToolkit* m_insetView = nullptr;
    QObject* m_geoView = nullptr;
    Graphic* m_reticle = nullptr;
    double m_scaleFactor = 25.0;
    TaskWatcher m_updateInsetViewpointTask;
    TaskWatcher m_updateGeoViewpointTask;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_OVERVIEWMAPCONTROLLER_H
