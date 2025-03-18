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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_SCALEBARCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_SCALEBARCONTROLLER_H

// Qt headers
#include <QObject>

// STL headers
#include <CoreTypes.h>
#include <LinearUnit.h>

// Other headers
#include "GeoViews.h"

namespace Esri::ArcGISRuntime::Toolkit {

  class ScalebarController : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QObject* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
    Q_PROPERTY(Esri::ArcGISRuntime::UnitSystem unitSystem READ unitSystem WRITE setUnitSystem NOTIFY unitSystemChanged)
  public:
    Q_INVOKABLE explicit ScalebarController(QObject* parent = nullptr);

    ~ScalebarController();

    QObject* mapView() const;
    void setMapView(QObject* geoView);

    UnitSystem unitSystem();
    void setUnitSystem(UnitSystem unitSystem);

    double calculateBestScalebarLength(double maxLength, LinearUnit units);

    Q_INVOKABLE double calculateBestScalebarLength(double maxLength);

    Q_INVOKABLE double calculateDistance(double width);

    Q_INVOKABLE double calculateDisplayWidth(double displayDistance, double maximumDistance, double availableWidth);

    Q_INVOKABLE QString calculateDistanceInDisplayUnits(double distance, Esri::ArcGISRuntime::UnitSystem unitSystem);

  signals:
    void mapViewChanged();
    void unitSystemChanged();
    void viewpointChanged();

  private:
    MapViewToolkit* m_mapView{nullptr};
    UnitSystem m_unitSystem{UnitSystem::Metric};
    LinearUnit m_baseUnit{LinearUnitId::Meters};
  };

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_NORTHARROWCONTROLLER_H
