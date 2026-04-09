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

#include "NorthArrowDemo.h"

#include <MapQuickView.h>
#include <MapViewTypes.h>

#include <QFuture>

using namespace Esri::ArcGISRuntime;

NorthArrowDemo::NorthArrowDemo(QObject* parent) :
  BaseDemo(parent)
{
  connect(this, &BaseDemo::geoViewChanged, this, &NorthArrowDemo::setupMapView_);
}

NorthArrowDemo::~NorthArrowDemo()
{
}

double NorthArrowDemo::mapViewRotation(MapQuickView* mapView) const
{
  return mapView ? mapView->mapRotation() : 0.0;
}

void NorthArrowDemo::setMapViewRotation(MapQuickView* mapView, double degrees)
{
  if (!mapView)
  {
    return;
  }

  // indicate this sooner than necessary to prevent a feedback loop with the slider in QML
  setDrawingComplete_(false);

  auto future = mapView->setViewpointRotationAsync(degrees);
  Q_UNUSED(future);
}

void NorthArrowDemo::setupMapView_()
{
  auto* view = geoView();

  if (!view || view->geoViewType() != GeoViewType::MapView)
  {
    return;
  }

  if (m_drawStatusChangedConnection)
  {
    disconnect(m_drawStatusChangedConnection);
    m_drawStatusChangedConnection = {};
  }

  auto* mapView = static_cast<MapQuickView*>(view);
  mapView->setRotationByPinchingEnabled(true);
  setDrawingComplete_(mapView->drawStatus() == DrawStatus::Completed);
  m_drawStatusChangedConnection = connect(mapView, &MapQuickView::drawStatusChanged, this, [this](DrawStatus drawStatus)
  {
    setDrawingComplete_(drawStatus == DrawStatus::Completed);
  });
}

void NorthArrowDemo::setDrawingComplete_(bool completed)
{
  if (m_drawingComplete == completed)
  {
    return;
  }

  m_drawingComplete = completed;
  emit drawingCompleteChanged();
}
