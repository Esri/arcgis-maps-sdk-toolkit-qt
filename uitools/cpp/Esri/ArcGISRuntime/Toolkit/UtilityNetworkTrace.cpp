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
#include <ArcGISFeature.h>
#include <FeatureLayer.h>
#include <FeatureTable.h>
#include <GeoView.h>
#include <GeometryEngine.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <Map.h>
#include <SimpleMarkerSymbol.h>
#include <Symbol.h>
#include <UtilityAssetType.h>
#include <UtilityElement.h>
#include <UtilityElementTraceResult.h>
#include <UtilityNamedTraceConfiguration.h>
#include <UtilityNetwork.h>
#include <UtilityNetworkListModel.h>
#include <UtilityNetworkSource.h>
#include <UtilityNetworkTraceOperationResult.h>
#include <UtilityTerminalConfiguration.h>
#include <UtilityTraceParameters.h>
#include <UtilityTraceResult.h>
#include <UtilityTraceResultListModel.h>

// Toolkit headers
#include "UtilityNetworkListItem.h"
#include "UtilityNetworkTraceStartingPoint.h"

// Qt headers
#include <QList>

// std headers
#include <cmath>

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
    m_startingPointSymbol(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor(Qt::green), 20.0f)),
    QObject(parent)
  {
    qDebug() << "UtilityNetworkTrace begins construction";
    m_mapView->graphicsOverlays()->append(m_startingPointsGraphicsOverlay);

    connect(this, &UtilityNetworkTrace::startingPointsChanged, this, []()
            {
              //
            });

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

    auto m = m_mapView;

    // identify symbols on mouse click
    connect(m_mapView, &MapQuickView::mouseClicked, this, [this](QMouseEvent& mouseEvent)
            {
              qDebug() << "### results searched";
              const double tolerance = 10.0;
              const bool returnPopups = false;
              Point mapPoint = this->m_mapView->screenToLocation(mouseEvent.x(), mouseEvent.y());

              // handle the identify results
              connect(m_mapView, &MapQuickView::identifyLayersCompleted, this, [this, mapPoint](QUuid, const QList<IdentifyLayerResult*>& results)
                      {
                        qDebug() << "### results found";
                        for (const auto& layer : results)
                        {
                          for (const auto& geoElement : layer->geoElements())
                          {
                            const auto ft = dynamic_cast<ArcGISFeature*>(geoElement);
                            addStartingPoint(ft, mapPoint);
                          }
                        }
                      });

              m_mapView->identifyLayers(mouseEvent.x(), mouseEvent.y(), tolerance, returnPopups);
            });

    setupUtilityNetworks();
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

  Symbol* UtilityNetworkTrace::startingPointSymbol() const
  {
    return m_startingPointSymbol;
  }

  void UtilityNetworkTrace::setStartingPointSymbol(Symbol* startingPointSymbol)
  {
    if (m_startingPointSymbol == startingPointSymbol)
      return;

    m_startingPointSymbol = startingPointSymbol;
    emit startingPointSymbolChanged();
  }

  void UtilityNetworkTrace::runTrace(const QString& name)
  {
    qDebug() << "Trace underway";
    setIsTraceInProgress(true);
    auto selectedTraceConfiguration = this->selectedTraceConfiguration();

    // create utility trace parameters for a connected trace
    QList<UtilityElement*> utilityElementsForStartingPoints;
    for (const auto& sp : startingPoints())
    {
      utilityElementsForStartingPoints.append(sp->utilityElement());
    }

    try
    {
      UtilityTraceParameters* utilityTraceParameters = new UtilityTraceParameters(selectedTraceConfiguration, utilityElementsForStartingPoints);

      UtilityNetworkTraceOperationResult* resultsInProgress = new UtilityNetworkTraceOperationResult(utilityTraceParameters, selectedTraceConfiguration);
      qDebug() << "result in progress: " << resultsInProgress->resultsGraphicsOverlay()->graphics()->size();
      qDebug() << "result in progress: " << resultsInProgress->name();

      // TODO: handle if selectedTraceConfig is null

      qDebug() << "trace params: " << utilityTraceParameters->traceConfiguration();

      // run the utility trace and get the results
      // async connection auto utilityTraceResultsFuture = m_selectedUtilityNetwork->trace(utilityTraceParameters);

      connect(m_selectedUtilityNetwork, &UtilityNetwork::traceCompleted, this, [this, resultsInProgress]()
              {
                /*try
                {
                  UtilityTraceResultListModel* utilityTraceResults = m_selectedUtilityNetwork->traceResult();
                  // set the raw results to the current result
                  resultsInProgress->setRawResults(utilityTraceResults);

                  // loop through the results
                  for (int i = 0; i < utilityTraceResults->size(); ++i)
                  {
                    auto utilityTraceResult = utilityTraceResults->at(i);
                    // if there are any warnings, add to the current result
                    if (!utilityTraceResult->warnings().isEmpty())
                    {
                      resultsInProgress->setWarnings(utilityTraceResult->warnings());
                    }

                    // TODO: handle errors / warnings
                    // TODO: zoom to extent
                    if (utilityTraceResult->traceResultObjectType() == UtilityTraceResultObjectType::UtilityElementTraceResult)
                    {
                      qDebug() << "Utility Element trace result";

                      auto utilityElementTraceResult = dynamic_cast<UtilityElementTraceResult*>(utilityTraceResult);

                      // add the element trace results to the current result
                      resultsInProgress->setElementResults(utilityElementTraceResult->elements());
                      QHash<UtilityAssetGroup*, QList<UtilityElement*>> elementResultsByAssetGroup{};
                      //resultsInProgress->setElementResultsByAssetGroup(resultsInProgress->elementResults()->stream().collect(groupingBy(UtilityElement::getAssetGroup)));

                      auto featuresFuture = m_selectedUtilityNetwork->featuresForElements(utilityElementTraceResult->elements());
                      /*this->setIsFetchingFeaturesInProgress(true);
                      featuresFuture.addDoneListener(()->
                                                     {
                                                       try
                                                       {
                                                         System.out.println("features future");
                                                         var features = featuresFuture.get();

                                                         // add the features to the current result
                                                         resultsInProgress.setFeatures(FXCollections.observableArrayList(features));

                                                         System.out.println("features: " + features.size());

                                                         selectResultFeatures(resultsInProgress);

                                                       } catch (Exception e)
                                                       {
                                                         e.printStackTrace();
                                                         isTraceInProgressProperty.set(false);
                                                       } finally
                                                       {
                                                         isFetchingFeaturesInProgressProperty.set(false);
                                                       }
                                                     });
                    }
                    else if (utilityTraceResult instanceof UtilityGeometryTraceResult)
                    {
                      System.out.println("Utility Geometry trace result");

                      var geometryTraceResult = (UtilityGeometryTraceResult)utilityTraceResult;

                      var multipoint = geometryTraceResult.getMultipoint();
                      if (multipoint != null)
                      {
                        var graphic = new Graphic(multipoint, new SimpleMarkerSymbol(defaultResultPointSymbol.getStyle(), defaultResultPointSymbol.getColor(), defaultResultPointSymbol.getSize()));
                        resultsInProgress.getGraphics().add(graphic);
                        resultsInProgress.getResultsGraphicsOverlay().getGraphics().add(graphic);
                      }

                      var polyline = geometryTraceResult.getPolyline();
                      if (polyline != null)
                      {
                        // TODO: linesymbol customisation
                        var graphic = new Graphic(polyline, new SimpleLineSymbol(defaultResultLineSymbol.getStyle(), defaultResultLineSymbol.getColor(), defaultResultLineSymbol.getWidth()));
                        resultsInProgress.getGraphics().add(graphic);
                        resultsInProgress.getResultsGraphicsOverlay().getGraphics().add(graphic);
                      }
                      System.out.println("result in progress graphics overlay: " + resultsInProgress.getResultsGraphicsOverlay());

                      var polygon = geometryTraceResult.getPolygon();
                      if (polygon != null)
                      {
                        // TODO: polygon customisation
                        var graphic = new Graphic(polygon, new SimpleFillSymbol(defaultResultFillSymbol.getStyle(), defaultResultFillSymbol.getColor(), defaultResultFillSymbol.getOutline()));
                        resultsInProgress.getGraphics().add(graphic);
                        resultsInProgress.getResultsGraphicsOverlay().getGraphics().add(graphic);
                      }
                      System.out.println("geometry results multipoint: " + geometryTraceResult.getMultipoint());
                      System.out.println("geometry results polyline: " + geometryTraceResult.getPolyline());
                      System.out.println("geometry results polygon: " + geometryTraceResult.getPolygon());

                      getMapView().getGraphicsOverlays().add(resultsInProgress.getResultsGraphicsOverlay());
                      System.out.println("number of gos: " + getMapView().getGraphicsOverlays().size());
                    }
                    else if (utilityTraceResult instanceof UtilityFunctionTraceResult)
                    {
                      System.out.println("Utility Function trace result");

                      var functionTraceResult = (UtilityFunctionTraceResult)utilityTraceResult;
                      functionTraceResult.getFunctionOutputs().forEach(functionOutput->resultsInProgress.getFunctionResults().add(functionOutput));
                      System.out.println("function results: " + functionTraceResult.getFunctionOutputs().size());
                    }
                    else
                    {
                      System.out.println("nothing found in results");
                    }
                  });
                  resultsInProgress.setName(name);
                  traceResults.addAll(resultsInProgress);
                  System.out.println("added results now");
                  isTraceInProgressProperty.set(false);
                } catch (Exception e)
                {
                  e.printStackTrace();
                  isTraceInProgressProperty.set(false);
                }*/
              });
      m_selectedUtilityNetwork->trace(utilityTraceParameters);

    } catch (Error e)
    {
      if (e.message().contains("namedTraceConfiguration must not be null"))
      {
        qDebug() << "Warning: namedtrace cannot be null";
      }
      setIsTraceInProgress(false);
    }
  }

  QList<UtilityNamedTraceConfiguration*> UtilityNetworkTrace::traceConfigurations() const
  {
    return m_traceConfigurations;
  }

  QList<UtilityNetworkTraceOperationResult*> UtilityNetworkTrace::traceResults()
  {
    return m_traceResults;
  }

  void UtilityNetworkTrace::refresh()
  {
    m_selectedUtilityNetwork = nullptr;
    m_selectedTraceConfiguration = nullptr;
    m_utilityNetworks = new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this);
    m_traceConfigurations.clear();
    m_startingPoints.clear();
    m_startingPointsGraphicsOverlay->graphics()->clear();
    m_traceResults.clear();
    setupUtilityNetworks();
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

  void UtilityNetworkTrace::addStartingPoint(ArcGISFeature* identifiedFeature, Point mapPoint)
  {
    auto geometry = identifiedFeature->geometry();
    auto utilityElement = m_selectedUtilityNetwork->createElementWithArcGISFeature(identifiedFeature);

    if (!utilityElement)
    {
      qDebug() << "Input feature does not belong to this utility network.... skipping.";
      return;
    }

    if (utilityElement)
    {
      auto startingPointAlreadyExists = false;
      auto utilityElementId = utilityElement->objectId();
      for (const auto& startingPoint : m_startingPoints)
      {
        if (startingPoint->utilityElement()->objectId() == utilityElementId)
        {
          startingPointAlreadyExists = true;
          qDebug() << "starting point for the selected utility element already exists.... skipping.";
        }
      }

      // skip if starting point already exists for the selected utility element
      if (!startingPointAlreadyExists)
      {
        if (utilityElement->networkSource()->sourceType() == UtilityNetworkSourceType::Edge &&
            geometry.geometryType() == GeometryType::Polyline)
        {
          auto polyline = geometry_cast<Polyline>(geometry);
          if (polyline.hasZ())
          {
            // get the geometry of the identified feature as a polyline, and remove the z component
            polyline = GeometryEngine::removeZ(polyline);
          }

          if (mapPoint.spatialReference() != polyline.spatialReference())
          {
            polyline = GeometryEngine::project(polyline, mapPoint.spatialReference());
          }

          geometry = polyline;
          // compute how far the clicked location is along the edge feature
          double fractionAlongEdge = GeometryEngine::fractionAlong(polyline, mapPoint, -1);
          if (!std::isnan(fractionAlongEdge))
          {
            // set the fraction along edge
            utilityElement->setFractionAlongEdge(fractionAlongEdge);
            qDebug() << "### EDGE";
          }
        }
        else if (utilityElement->networkSource()->sourceType() == UtilityNetworkSourceType::Junction &&
                 utilityElement->assetType()->terminalConfiguration() != nullptr)
        {
          auto utilityTerminalConfiguration = utilityElement->assetType()->terminalConfiguration();
          QList<UtilityTerminal*> terminals = utilityTerminalConfiguration->terminals();
          if (terminals.size() > 1)
          {
            utilityElement->setTerminal(utilityElement->assetType()->terminalConfiguration()->terminals().at(0));
            qDebug() << "### JUNCTION";
          }
        }

        auto graphic = new Graphic(geometry, this);
        //graphic->attributes().put("GlobalId", utilityElement.getGlobalId());
        graphic->setSymbol(m_startingPointSymbol);
        auto featureLayer = dynamic_cast<FeatureLayer*>(identifiedFeature->featureTable()->layer());
        auto symbol = featureLayer->renderer()->symbol(identifiedFeature);
        if (symbol == nullptr)
        {
          m_startingPoints.append(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, featureLayer->fullExtent(), this));
          qDebug() << "### Added with null";
        }
        else
        {
          m_startingPoints.append(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, graphic->geometry().extent(), this));
          qDebug() << "### Added with extent";
        }
      }
    }
  }

  void UtilityNetworkTrace::removeStartingPoint(UtilityNetworkTraceStartingPoint* startingPoint)
  {
    m_startingPoints.removeOne(startingPoint);
  }

  void UtilityNetworkTrace::setupUtilityNetworks()
  {
    // first check if there's a map
    if (m_mapView->map() != nullptr)
    {
      connect(m_mapView->map(), &Map::doneLoading, this, [this]()
              {
                const auto& map = m_mapView->map();

                if (!map->utilityNetworks()->isEmpty())
                {
                  if (map->utilityNetworks()->size() == 1)
                    setSelectedUtilityNetwork(map->utilityNetworks()->at(0));

                  for (int i = 0; i < map->utilityNetworks()->size(); ++i)
                  {
                    const auto& un = map->utilityNetworks()->at(i);

                    if (un->loadStatus() == LoadStatus::Loaded)
                      m_utilityNetworks->append(un);
                    else
                    {
                      connect(un, &UtilityNetwork::doneLoading, this, [this, un]()
                              {
                                this->m_utilityNetworks->append(un);
                              });
                      un->load();
                    }
                  }
                }
                else
                {
                  qDebug() << "There are no Utility Networks associated with the ArcGIS Map attached "
                           << "to the MapView. Utility Network Trace will not be displayed. Call "
                           << "UtilityNetworkTrace.refresh() after updating the ArcGIS Map to try again.";
                }
              });

      if (m_mapView->map()->loadStatus() != LoadStatus::Loaded)
      {
        // load the map if it is not already loaded
        m_mapView->map()->load();
      }
    }
    else
    {
      qDebug() << "There is no ArcGIS Map attached to the MapView. Utility Network Trace will not be"
               << " displayed. Call UtilityNetworkTrace.refresh() after updating the ArcGIS Map to try again.";
    }
  }

}
}
}
