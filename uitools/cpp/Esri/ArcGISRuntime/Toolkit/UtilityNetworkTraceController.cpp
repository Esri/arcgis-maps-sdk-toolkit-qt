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
#include "UtilityNetworkTraceController.h"

// ArcGISRuntime headers
#include <ArcGISFeature.h>
#include <AttributeListModel.h>
#include <Error.h>
#include <FeatureLayer.h>
#include <FeatureTable.h>
#include <GeoView.h>
#include <GeometryEngine.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <IdentifyLayerResult.h>
#include <Map.h>
#include <Polyline.h>
#include <Renderer.h>
#include <SimpleMarkerSymbol.h>
#include <SpatialReference.h>
#include <Symbol.h>
#include <SymbolTypes.h>
#include <TaskWatcher.h>
#include <UtilityAssetGroup.h>
#include <UtilityAssetType.h>
#include <UtilityElement.h>
#include <UtilityElementTraceResult.h>
#include <UtilityNamedTraceConfiguration.h>
#include <UtilityNetwork.h>
#include <UtilityNetworkListModel.h>
#include <UtilityNetworkSource.h>
#include <UtilityNetworkTraceOperationResult.h>
#include <UtilityNetworkTypes.h>
#include <UtilityTerminalConfiguration.h>
#include <UtilityTraceParameters.h>
#include <UtilityTraceResult.h>
#include <UtilityTraceResultListModel.h>

// Toolkit headers
#include "Internal/DisconnectOnSignal.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"
#include "UtilityNetworkListItem.h"
#include "UtilityNetworkTraceStartingPoint.h"
#include "UtilityNetworkTraceStartingPointsModel.h"

// Qt headers
#include <QList>
#include <QUuid>

// std headers
#include <cmath>

namespace Esri::ArcGISRuntime::Toolkit {

namespace {
void setupUtilityNetworksInt(UtilityNetworkListModel* sourceModel, GenericListModel* targetModel)
{
  QObject::connect(sourceModel, &UtilityNetworkListModel::rowsInserted, targetModel,
                   [sourceModel, targetModel](const QModelIndex& parent, int first, int last)
  {
    if (parent.isValid())
      return;

    if (!targetModel->insertRows(first, last - first + 1))
      return;

    for (auto i = first; i <= last; ++i)
    {
      QModelIndex j = targetModel->index(i);
      auto targetItem = targetModel->element<UtilityNetworkListItem>(j);
      if (targetItem)
        targetItem->setUtilityNetwork(sourceModel->at(i));
    }
  });

  QObject::connect(sourceModel, &UtilityNetworkListModel::rowsRemoved, targetModel,
                   [targetModel](const QModelIndex& parent, int first, int last)
  {
    if (parent.isValid())
      return;

    targetModel->removeRows(first, last - first + 1);
  });

  QObject::connect(sourceModel, &UtilityNetworkListModel::rowsMoved, targetModel,
                   [targetModel](const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
  {
    if (parent.isValid() || destination.isValid())
      return;

    targetModel->moveRows(QModelIndex{}, start, end - start + 1, QModelIndex{}, row);
  });

  QList<QObject*> targetItems;
  for (int i = 0; i < sourceModel->size(); ++i)
  {
    targetItems << new UtilityNetworkListItem(sourceModel->at(i), targetModel);
  }
  targetModel->append(targetItems);
}

/*!
     \internal
     \brief Manages the connection between Controller \a self and GeoView \a geoView.
     Attempts to call functor `f` if/when the UtilityNetwork within the geoModel is loaded.
     This may also cause the geoModel itself to load.
     Will continue to call `f` every time a mapChanged signal is triggered on
     the GeoView.
     */
template <typename GeoViewToolkit, typename Func>
void connectToGeoView(GeoViewToolkit* geoView, UtilityNetworkTraceController* self, Func&& f)
{
  static_assert(
  std::is_same<GeoViewToolkit, MapViewToolkit>::value,
      "Must be connected to a MapView");

  auto connectToGeoModel = [self, geoView, f = std::forward<Func>(f)]
  {
    auto model = getGeoModel(geoView);
    if (!model)
    {
      return;
    }

    // Here we attempt to call `f` if/when both the GeoModel and UtilityNetwork are loaded.
    // This may happen immediately or asyncnronously. This can be interrupted if GeoView or
    // GeoModel changes in the interim.
    auto c = doOnLoaded(model, self, [self, model, geoView, f = std::move(f)]()
    {
      auto utilityNetworks = model->utilityNetworks();

      for (const auto utilityNetwork : *utilityNetworks)
      {
        if (!utilityNetwork)
          return;

        auto c2 = doOnLoaded(utilityNetwork, self, [f = std::move(f)]
        {
          f();
        });
        // Destroy the connection `c` if the map changes, or the geoView changes.
        // This means the connection is only relevant for as long as the model/view is relavant to
        // the UtilityNetworkTraceController.
        disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c2);
        disconnectOnSignal(self, &UtilityNetworkTraceController::geoViewChanged, self, c2);
      }
    });

    // Destroy the connection `c` if the map changes, or the geoView changes. This means
    // the connection is only relevant for as long as the model/view is relavant to the UtilityNetworkTraceController.
    disconnectOnSignal(geoView, getGeoModelChangedSignal(geoView), self, c);
    disconnectOnSignal(self, &UtilityNetworkTraceController::geoViewChanged, self, c);
  };

  // Hooks up to any geoModels that appear when the mapView changed signal is called.
  QObject::connect(geoView, getGeoModelChangedSignal(geoView), self, connectToGeoModel);
  connectToGeoModel();

  // Hook up to any viewpoint changes on the GeoView.
  /*auto c2 = QObject::connect(geoView, &std::remove_pointer<decltype(geoView)>::type::viewpointChanged,
                                 self, &UtilityNetworkTraceController::tryUpdateSelection);
      disconnectOnSignal(self, &UtilityNetworkTraceController::geoViewChanged, self, c2);*/
}
}

UtilityNetworkTraceController::UtilityNetworkTraceController(QObject* parent) :
  QObject(parent),
  m_startingPointParent(new QObject(this)),
  m_startingPointsGraphicsOverlay(new GraphicsOverlay(m_startingPointParent)),
  m_utilityNetworks(new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this)),
  m_startingPoints(new UtilityNetworkTraceStartingPointsModel(this)),
  m_startingPointSymbol(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor(Qt::green), 20.0f, this))
{
  qDebug() << "UtilityNetworkTrace begins construction";
}

UtilityNetworkTraceController::~UtilityNetworkTraceController() = default;

/*!
  \brief Returns the \c GeoView as a \c QObject.
 */
QObject* UtilityNetworkTraceController::geoView() const
{
  return m_geoView;
}

/*!
  \brief Set the GeoView object this Controller uses.

  Internally this is cast to a \c MapView using \c qobject_cast,
      which is why the paremeter is of form \c QObject and not \c GeoView.

  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and
        \c{QObject*}.
  \endlist
                                                               */
void UtilityNetworkTraceController::setGeoView(QObject* geoView)
{
  if (geoView == m_geoView)
    return;

  if (m_geoView)
  {
    disconnect(m_geoView, nullptr, this, nullptr);

    if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      auto map = mapView->map();

      if (map && map->utilityNetworks())
        disconnect(map->utilityNetworks(), nullptr, m_utilityNetworks, nullptr);
    }
    else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      // scene does not have utility networks
      return;
    }

    m_utilityNetworks->clear();
  }

  m_geoView = geoView;

  // Important that this emit happens before the below connections,
  // as this emit will destroy the connections set up below.
  emit geoViewChanged();

  if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
  {
    connect(mapView, &MapViewToolkit::mapChanged, this, [this]()
    {
      m_utilityNetworks->clear();
    });

    // `connectToGeoView` guarantees the map exists as it is only invoked once the geomodel is loaded.
    // maybe the UN is not loaded just yet.
    connectToGeoView(mapView, this, [this, mapView]
    {
      setupUtilityNetworksInt(mapView->map()->utilityNetworks(), m_utilityNetworks);
    });

    connect(this,
            &UtilityNetworkTraceController::selectedUtilityNetworkChanged, // this should already been handled and newValue used
            this,
            [this](UtilityNetwork* newValue) // when it's used, we also need to use it
    {
      qDebug() << "selectedUtilityNetworkChanged";
      if (newValue)
      {
        // have the connection, but the internal logic should be done when (or where) the newValue is handled.
        auto c = connect(m_selectedUtilityNetwork,
                         &UtilityNetwork::queryNamedTraceConfigurationsCompleted,
                         this,
                         [this](QUuid taskId, const QList<Esri::ArcGISRuntime::UtilityNamedTraceConfiguration*>& utilityNamedTraceConfigurationResults)
        {
            const auto findIter = m_traceConfigConnection.find(taskId);
            if (findIter != m_traceConfigConnection.end())
        {
            disconnect(*findIter);
            m_traceConfigConnection.remove(taskId);
      }
            m_traceConfigurations.clear();
            m_traceConfigurations = utilityNamedTraceConfigurationResults;
      });
        auto traceConfigs = m_selectedUtilityNetwork->queryNamedTraceConfigurations(nullptr);
        m_traceConfigConnection.insert(traceConfigs.taskId(), c);
      }
      else
      {
        refresh();
      }
    });

    mapView->graphicsOverlays()->append(m_startingPointsGraphicsOverlay);

    populateUtilityNetworksFromMap();

    // handle the identify results
    connect(mapView, &MapQuickView::identifyLayersCompleted, this, [this](QUuid, const QList<IdentifyLayerResult*>& results)
    {
      // set busy false
      if (!isAddingStartingPoint())
        return;

      qDebug() << "### identifyLayersCompleted results found";
      for (const auto& layer : results)
      {
        for (const auto& geoElement : layer->geoElements())
        {
          const auto ft = dynamic_cast<ArcGISFeature*>(geoElement);
          addStartingPoint(ft, this->m_mapPoint);
        }
      }

      setIsAddingStartingPoint(!isAddingStartingPoint());
    });

    // identify symbols on mouse click
    connect(mapView, &MapQuickView::mouseClicked, this, [this, mapView](QMouseEvent& mouseEvent)
    {
      // check if busy is running
      // if so -> set busy true; return;
      qDebug() << "### mouseClicked search begins";
      const double tolerance = 10.0;
      const bool returnPopups = false;
      m_mapPoint = mapView->screenToLocation(mouseEvent.pos().x(), mouseEvent.pos().y());

      /* qhash(taskid, mouseevent))))*/ mapView->identifyLayers(mouseEvent.pos().x(), mouseEvent.pos().y(), tolerance, returnPopups);
    });

    setupUtilityNetworks();
  }
}

/*!
  \brief Returns the known list of available utilityNetworks which
  can be textually displayed.

  Internally, this is a \c GenericListModel with an \c elementType of
  \c UtilityNetwork.
                      */
GenericListModel* UtilityNetworkTraceController::utilityNetworks() const
{
  qDebug() << "Returning m_utilityNetworks";
  return m_utilityNetworks;
}

UtilityNetwork* UtilityNetworkTraceController::selectedUtilityNetwork() const
{
  return m_selectedUtilityNetwork;
}

void UtilityNetworkTraceController::setSelectedUtilityNetwork(UtilityNetwork* selectedUtilityNetwork)
{
  if (m_selectedUtilityNetwork == selectedUtilityNetwork)
    return;

  m_selectedUtilityNetwork = selectedUtilityNetwork;
  emit selectedUtilityNetworkChanged(m_selectedUtilityNetwork);
}

QAbstractItemModel* UtilityNetworkTraceController::startingPoints() const
{
  return m_startingPoints;
}

UtilityNamedTraceConfiguration* UtilityNetworkTraceController::selectedTraceConfiguration() const
{
  return m_selectedTraceConfiguration;
}

void UtilityNetworkTraceController::setSelectedTraceConfiguration(UtilityNamedTraceConfiguration* selectedTraceConfiguration)
{
  if (m_selectedTraceConfiguration == selectedTraceConfiguration)
    return;

  m_selectedTraceConfiguration = selectedTraceConfiguration;
  emit selectedTraceConfigurationChanged();
}

bool UtilityNetworkTraceController::isTraceInProgress() const
{
  return m_isTraceInProgress;
}

void UtilityNetworkTraceController::setIsTraceInProgress(const bool isTraceInProgress)
{
  if (m_isTraceInProgress == isTraceInProgress)
    return;

  m_isTraceInProgress = isTraceInProgress;
  emit isTraceInProgressChanged();
}

bool UtilityNetworkTraceController::isAddingStartingPoint() const
{
  return m_isAddingStartingPoint;
}

void UtilityNetworkTraceController::setIsAddingStartingPoint(const bool isAddingStartingPoint)
{
  if (m_isAddingStartingPoint == isAddingStartingPoint)
    return;

  m_isAddingStartingPoint = isAddingStartingPoint;
  emit isAddingStartingPointChanged();
}

Symbol* UtilityNetworkTraceController::startingPointSymbol() const
{
  return m_startingPointSymbol;
}

void UtilityNetworkTraceController::setStartingPointSymbol(Symbol* startingPointSymbol)
{
  if (m_startingPointSymbol == startingPointSymbol)
    return;

  m_startingPointSymbol = startingPointSymbol;
  emit startingPointSymbolChanged();
}

void UtilityNetworkTraceController::runTrace(const QString& /*name*/)
{
  qDebug() << "Trace underway";
  setIsTraceInProgress(true);
  auto selectedTraceConfiguration = this->selectedTraceConfiguration();

  // create utility trace parameters for a connected trace
  QList<UtilityElement*> utilityElementsForStartingPoints = m_startingPoints->utilityElements();

  try
  {
    UtilityTraceParameters* utilityTraceParameters = new UtilityTraceParameters(selectedTraceConfiguration, utilityElementsForStartingPoints, this);

    UtilityNetworkTraceOperationResult* resultsInProgress = new UtilityNetworkTraceOperationResult(utilityTraceParameters, selectedTraceConfiguration, this);
    qDebug() << "result in progress: " << resultsInProgress->resultsGraphicsOverlay()->graphics()->size();
    qDebug() << "result in progress: " << resultsInProgress->name();

    // TODO: handle if selectedTraceConfig is null

    qDebug() << "trace params: " << utilityTraceParameters->traceConfiguration();

    // run the utility trace and get the results
    // async connection auto utilityTraceResultsFuture = m_selectedUtilityNetwork->trace(utilityTraceParameters);

    connect(m_selectedUtilityNetwork, &UtilityNetwork::traceCompleted, this, [/*this, resultsInProgress*/]()
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
                      this->setIsFetchingFeaturesInProgress(true);
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

QList<UtilityNamedTraceConfiguration*> UtilityNetworkTraceController::traceConfigurations() const
{
  return m_traceConfigurations;
}

QList<UtilityNetworkTraceOperationResult*> UtilityNetworkTraceController::traceResults()
{
  return m_traceResults;
}

void UtilityNetworkTraceController::refresh()
{
  m_selectedUtilityNetwork = nullptr;
  m_selectedTraceConfiguration = nullptr;
  m_utilityNetworks = new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this);
  m_traceConfigurations.clear();
  m_startingPoints->clear();
  m_startingPointsGraphicsOverlay->graphics()->clear();

  if (m_startingPointParent)
    delete m_startingPointParent;

  m_startingPointParent = new QObject(this);
  m_traceResults.clear();
  qDebug() << "Resetting";
  setupUtilityNetworks();
}

void UtilityNetworkTraceController::populateUtilityNetworksFromMap()
{
  const auto mapView = qobject_cast<MapViewToolkit*>(m_geoView);

  if (mapView)
  {
    auto map = mapView->map();
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

void UtilityNetworkTraceController::addStartingPoint(ArcGISFeature* identifiedFeature, Point mapPoint)
{
  auto geometry = identifiedFeature->geometry();

  if (!m_selectedUtilityNetwork)
    return;

  auto utilityElement = m_selectedUtilityNetwork->createElementWithArcGISFeature(identifiedFeature);

  if (!utilityElement)
  {
    qDebug() << "Input feature does not belong to this utility network.... skipping.";
    return;
  }

  if (utilityElement)
  {
    auto startingPointAlreadyExists = m_startingPoints->doesItemAlreadyExist(utilityElement);
    if (startingPointAlreadyExists)
      qDebug() << "starting point for the selected utility element already exists.... skipping.";

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
          polyline = geometry_cast<Polyline>(GeometryEngine::removeZ(polyline));
        }

        if (mapPoint.spatialReference() != polyline.spatialReference())
        {
          polyline = geometry_cast<Polyline>(GeometryEngine::project(polyline, mapPoint.spatialReference()));
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

      auto graphic = new Graphic(geometry, m_startingPointParent);
      graphic->attributes()->insertAttribute("GlobalId", utilityElement->globalId());
      graphic->setSymbol(m_startingPointSymbol);
      auto featureLayer = dynamic_cast<FeatureLayer*>(identifiedFeature->featureTable()->layer());
      auto symbol = featureLayer->renderer()->symbol(identifiedFeature);
      //
      m_startingPointsGraphicsOverlay->graphics()->append(graphic);
      if (symbol == nullptr)
      {
        m_startingPoints->addStartingPoint(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, featureLayer->fullExtent(), m_startingPointParent));
        qDebug() << "### Added with null";
        emit startingPointsChanged();
      }
      else
      {
        m_startingPoints->addStartingPoint(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, graphic->geometry().extent(), m_startingPointParent));
        qDebug() << "### Added with extent";
        emit startingPointsChanged();
      }
    }
  }
}

void UtilityNetworkTraceController::removeStartingPoint(int index)
{
  m_startingPoints->removeAt(index);
}

void UtilityNetworkTraceController::setupUtilityNetworks()
{
  const auto mapView = qobject_cast<MapViewToolkit*>(m_geoView);
  // first check if there's a map
  if (mapView->map() != nullptr)
  {
    connect(mapView->map(), &Map::doneLoading, this, [this, mapView]()
    {
      const auto& map = mapView->map();

      if (!map->utilityNetworks()->isEmpty())
      {
        if (map->utilityNetworks()->size() == 1)
        {
          qDebug() << "There is exactly one UN";
          setSelectedUtilityNetwork(map->utilityNetworks()->at(0));
        }
        else
        {
          qDebug() << "There are multiple UNs";
        }

        for (const auto un : qAsConst(*map->utilityNetworks()))
        {
          if (un->loadStatus() == LoadStatus::Loaded)
          {
            qDebug() << "@@@ m_utilityNetworks->append(un) without explicit loading";
            m_utilityNetworks->append(un);
          }
          else
          {
            //single shot connection
            QMetaObject::Connection* const connection = new QMetaObject::Connection;
            *connection = connect(un, &UtilityNetwork::doneLoading, this, [this, un, connection](const Error& e)
            {
              if (!e.isEmpty())
              {
                qDebug() << e.message() << e.additionalMessage();
                return;
              }
              qDebug() << "@@@ m_utilityNetworks->append(un) with explicit loading";
              this->m_utilityNetworks->append(un);
              QObject::disconnect(*connection);
              delete connection;
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

    if (mapView->map()->loadStatus() != LoadStatus::Loaded)
    {
      // load the map if it is not already loaded
      mapView->map()->load();
    }
  }
  else
  {
    qDebug() << "There is no ArcGIS Map attached to the MapView. Utility Network Trace will not be"
             << " displayed. Call UtilityNetworkTrace.refresh() after updating the ArcGIS Map to try again.";
  }
}

} // Esri::ArcGISRuntime::Toolkit
