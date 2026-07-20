
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
#include "UtilityNetworkTraceDemo.h"

#include <ArcGISFeature.h>
#include <Envelope.h>
#include <Error.h>
#include <FeatureIterator.h>
#include <FeatureLayer.h>
#include <FeatureQueryResult.h>
#include <FeatureTable.h>
#include <Geometry.h>
#include <GeoView.h>
#include <GroupLayer.h>
#include <LayerListModel.h>
#include <Map.h>
#include <MapQuickView.h>
#include <MapTypes.h>
#include <Point.h>
#include <Portal.h>
#include <PortalItem.h>
#include <QueryParameters.h>

#include <QFuture>

using namespace Esri::ArcGISRuntime;

namespace
{
  struct TraceTestCase
  {
    QUrl webMapUrl;
    QString layerName;
    qint64 objectId;
  };

  const QList<TraceTestCase> testCases{
    {QUrl{"https://rt-server121.esri.com/portal/home/item.html?id=20676fa12a584aa3b11229bd4b429b2d"}, QStringLiteral("Electric Device"), 11778},
    {QUrl{"https://www.arcgis.com/home/item.html?id=471eb0bf37074b1fbb972b1da70fb310"}, QStringLiteral("Electric Distribution Device"), 3174}};

  FeatureLayer* findFeatureLayer(LayerListModel* layers, const QString& name)
  {
    for (auto* layer : *layers)
    {
      if (auto* featureLayer = qobject_cast<FeatureLayer*>(layer); featureLayer && featureLayer->name() == name)
      {
        return featureLayer;
      }

      if (auto* groupLayer = qobject_cast<GroupLayer*>(layer))
      {
        if (auto* featureLayer = findFeatureLayer(groupLayer->layers(), name))
        {
          return featureLayer;
        }
      }
    }

    return nullptr;
  }
} // namespace

UtilityNetworkTraceDemo::UtilityNetworkTraceDemo(QObject* parent) :
  BaseDemo(parent)
{
  connect(this, &BaseDemo::geoViewChanged, this, [this]()
  {
    if (auto* mapView = dynamic_cast<MapQuickView*>(geoView()))
    {
      auto* map = mapView->map();
      if (map->loadStatus() == LoadStatus::Loaded)
      {
        focusTestFeature(map);
      }
      else
      {
        connect(map, &Map::doneLoading, this, [this, map](const Error&)
        {
          focusTestFeature(map);
        });
      }
    }
  });
}

UtilityNetworkTraceDemo::~UtilityNetworkTraceDemo() = default;

void UtilityNetworkTraceDemo::loadTestCase(int index)
{
  if (index < 0 || index >= testCases.size())
  {
    return;
  }

  m_testCaseIndex = index;
  m_testFeature = nullptr;
  emit testFeatureChanged();
  if (auto* mapView = dynamic_cast<MapQuickView*>(geoView()))
  {
    auto* map = new Map(testCases.at(index).webMapUrl, mapView);
    connect(map, &Map::doneLoading, this, [this, map](const Error&)
    {
      focusTestFeature(map);
    });
    mapView->setMap(map);
  }
}

QObject* UtilityNetworkTraceDemo::testFeature() const
{
  return m_testFeature;
}

Map* UtilityNetworkTraceDemo::initMap_(QObject* parent) const
{
  return new Map(testCases.at(m_testCaseIndex).webMapUrl, parent);
}

Scene* UtilityNetworkTraceDemo::initGlobalScene_(QObject* /*parent*/) const
{
  return nullptr;
}

void UtilityNetworkTraceDemo::focusTestFeature(Map* map)
{
  auto* mapView = dynamic_cast<MapQuickView*>(geoView());
  if (!mapView || mapView->map() != map)
  {
    return;
  }

  const auto& testCase = testCases.at(m_testCaseIndex);
  auto* featureLayer = findFeatureLayer(map->operationalLayers(), testCase.layerName);
  if (!featureLayer)
  {
    qWarning() << "Could not find test layer" << testCase.layerName;
    return;
  }

  QueryParameters parameters;
  parameters.setObjectIds({testCase.objectId});
  featureLayer->featureTable()
    ->queryFeaturesAsync(parameters, this)
    .then(this, [this, map, mapView](FeatureQueryResult* result)
  {
    if (mapView->map() != map)
    {
      return;
    }

    auto iterator = result->iterator();
    if (!iterator.hasNext())
    {
      qWarning() << "Could not find the test feature";
      return;
    }

    auto* feature = static_cast<ArcGISFeature*>(iterator.next(this));
    m_testFeature = feature;
    emit testFeatureChanged();
    auto future = mapView->setViewpointCenterAsync(feature->geometry().extent().center(), 2500.0);
    Q_UNUSED(future)
  });
}
