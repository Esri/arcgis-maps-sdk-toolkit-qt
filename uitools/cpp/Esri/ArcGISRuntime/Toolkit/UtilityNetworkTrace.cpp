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
#include "UtilityNetworkTrace.h"

// ArcGISRuntime headers
#include <Feature.h>
#include <FeatureLayer.h>
#include <FeatureTable.h>
#include <GeoView.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <Map.h>
#include <Symbol.h>
#include <UtilityNamedTraceConfiguration.h>
#include <UtilityNetwork.h>
#include <UtilityNetworkListModel.h>

// Toolkit headers
#include "UtilityNetworkListItem.h"

// Qt headers
#include <QList>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  UtilityNetworkTrace::UtilityNetworkTrace(QObject* parent) :
    QObject(parent)
  {
    //
  }

  UtilityNetworkTrace::UtilityNetworkTrace(MapViewToolkit* mapView, QObject* parent) :
    m_mapView(mapView),
    m_utilityNetworks(new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this)),
    QObject(parent)
  {
    qDebug() << "UtilityNetworkTrace begins construction";
    m_mapView->graphicsOverlays()->append(m_graphicsOverlay);

    connect(this,
            &UtilityNetworkTrace::selectedUtilityNetworkChanged,
            this,
            [this](UtilityNetwork* newValue)
            {
              if (newValue)
              {
                connect(m_selectedUtilityNetwork,
                        &UtilityNetwork::queryNamedTraceConfigurationsCompleted,
                        this,
                        [this](QUuid /*taskId*/, const QList<Esri::ArcGISRuntime::UtilityNamedTraceConfiguration*>& utilityNamedTraceConfigurationResults)
                        {
                          m_traceConfigurations.clear();
                          m_traceConfigurations = utilityNamedTraceConfigurationResults;
                        });
                auto traceConfigs = m_selectedUtilityNetwork->queryNamedTraceConfigurations(nullptr);
              }
              else
              {
                m_traceConfigurations.clear();
                m_selectedTraceConfiguration = nullptr;
              }
            });
    populateUtilityNetworksFromMap();

    // identify symbols on mouse click
    connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
            {
              qDebug() << "### results searched";
              const double tolerance = 10.0;
              const bool returnPopups = false;
              m_mapView->identifyLayers(mouseEvent.x(), mouseEvent.y(), tolerance, returnPopups);
            });

    // handle the identify results
    connect(m_mapView, &MapQuickView::identifyLayersCompleted, this, [this](QUuid, const QList<IdentifyLayerResult*>& results)
            {
              qDebug() << "### results found";
              for (const auto& layer : results)
              {
                for (const auto& geoElement : layer->geoElements())
                {
                  const auto ft = dynamic_cast<Feature*>(geoElement);
                  if (ft)
                  {
                    qDebug() << "Yes, it's a feature";
                    const auto extent = ft->geometry().extent();
                    const auto layer = ft->featureTable()->layer();
                    const auto ftLayer = dynamic_cast<FeatureLayer*>(layer);

                    if (ftLayer)
                    {
                      const auto symbol = ftLayer->renderer()->symbol(ft);

                      // do I need this?
                      symbol->createSwatch(1.0);
                    }

                  }
                }
              }
            });
  }

  UtilityNetworkTrace::~UtilityNetworkTrace() = default;

  MapViewToolkit* UtilityNetworkTrace::mapView() const
  {
    return m_mapView;
  }

  void UtilityNetworkTrace::setMapView(MapViewToolkit* mapView)
  {
    if (!mapView || m_mapView == mapView)
      return;

    m_mapView = mapView;
    emit mapViewChanged();
  }

  UtilityNetwork* UtilityNetworkTrace::selectedUtilityNetwork() const
  {
    return m_selectedUtilityNetwork;
  }

  void UtilityNetworkTrace::setSelectedUtilityNetwork(UtilityNetwork* selectedUtilityNetwork)
  {
    if (m_selectedUtilityNetwork == selectedUtilityNetwork)
      return;

    m_selectedUtilityNetwork = selectedUtilityNetwork;
    emit selectedUtilityNetworkChanged(m_selectedUtilityNetwork);
  }

  QList<UtilityNetworkTraceStartingPoint*> UtilityNetworkTrace::startingPoints() const
  {
    return m_startingPoints;
  }

  void UtilityNetworkTrace::setStartingPoints(QList<UtilityNetworkTraceStartingPoint*> startingPoints)
  {
    if (m_startingPoints == startingPoints)
      return;

    m_startingPoints = startingPoints;
    emit startingPointsChanged();
  }

  UtilityNamedTraceConfiguration* UtilityNetworkTrace::selectedTraceConfiguration() const
  {
    return m_selectedTraceConfiguration;
  }

  void UtilityNetworkTrace::setSelectedTraceConfiguration(UtilityNamedTraceConfiguration* selectedTraceConfiguration)
  {
    if (m_selectedTraceConfiguration == selectedTraceConfiguration)
      return;

    m_selectedTraceConfiguration = selectedTraceConfiguration;
    emit selectedTraceConfigurationChanged();
  }

  bool UtilityNetworkTrace::isTraceInProgress() const
  {
    return m_isTraceInProgress;
  }

  void UtilityNetworkTrace::setIsTraceInProgress(bool isTraceInProgress)
  {
    if (m_isTraceInProgress == isTraceInProgress)
      return;

    m_isTraceInProgress = isTraceInProgress;
    emit isTraceInProgressChanged();
  }

  QList<UtilityNamedTraceConfiguration*> UtilityNetworkTrace::traceConfigurations() const
  {
    return m_traceConfigurations;
  }

  QList<UtilityNetworkTraceOperationResult*> UtilityNetworkTrace::traceResults() const
  {
    return m_traceResults;
  }

  void UtilityNetworkTrace::populateUtilityNetworksFromMap()
  {
    if (m_mapView)
    {
      auto map = m_mapView->map();
      if (map->loadStatus() == LoadStatus::Loaded)
      {
        const auto un = map->utilityNetworks();
        for (int i = 0; i < un->size(); ++i)
        {
          if (un->at(i)->loadStatus() == LoadStatus::Loaded)
          {
            m_utilityNetworks->append(un->at(i));
          }
          else
          {
            qDebug() << "Utility network" << i << "not loaded";
          }
        }

        if (map->utilityNetworks()->size() == 1)
        {
          setSelectedUtilityNetwork(map->utilityNetworks()->at(0));
        }
      }
      else
      {
        qDebug() << "map not loaded";
      }
    }
  }

}
}
}
