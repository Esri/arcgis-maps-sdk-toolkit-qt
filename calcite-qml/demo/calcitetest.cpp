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

#include "CalciteTest.h"

#include "Map.h"
#include "MapQuickView.h"
#include "MapTypes.h"
#include "Basemap.h"
#include "OpenStreetMapLayer.h"

using namespace Esri::ArcGISRuntime;

CalciteTest::CalciteTest(QObject* parent /* = nullptr */):
  QObject(parent),
  m_map(new Map(this))
{
  m_map->setBasemap(new Basemap(new OpenStreetMapLayer(this), this));
}

CalciteTest::~CalciteTest() = default;

void CalciteTest::init()
{
  // Register the map view for QML
  qmlRegisterType<MapQuickView>("Esri.Calcite", 1, 0, "MapView");
  qmlRegisterType<CalciteTest>("Esri.Calcite", 1, 0, "CalciteTest");
}

MapQuickView* CalciteTest::mapView() const
{
  return m_mapView;
}

// Set the view (created in QML)
void CalciteTest::setMapView(MapQuickView* mapView)
{
  if (!mapView || mapView == m_mapView)
    return;

  m_mapView = mapView;
  m_mapView->setMap(m_map);

  emit mapViewChanged();
}
