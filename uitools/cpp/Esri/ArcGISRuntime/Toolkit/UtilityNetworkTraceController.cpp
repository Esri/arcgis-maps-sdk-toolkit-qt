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
#include <ArcGISFeatureListModel.h>
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
#include <Multipoint.h>
#include <Polygon.h>
#include <Polyline.h>
#include <Renderer.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SpatialReference.h>
#include <Symbol.h>
#include <SymbolTypes.h>
#include <TaskWatcher.h>
#include <UtilityAssetGroup.h>
#include <UtilityAssetType.h>
#include <UtilityElement.h>
#include <UtilityElementTraceResult.h>
#include <UtilityFunctionTraceResult.h>
#include <UtilityGeometryTraceResult.h>
#include <UtilityNamedTraceConfiguration.h>
#include <UtilityNetwork.h>
#include <UtilityNetworkAttribute.h>
#include <UtilityNetworkListModel.h>
#include <UtilityNetworkSource.h>
#include <UtilityNetworkTypes.h>
#include <UtilityTerminalConfiguration.h>
#include <UtilityTraceFunction.h>
#include <UtilityTraceFunctionOutput.h>
#include <UtilityTraceParameters.h>
#include <UtilityTraceResult.h>
#include <UtilityTraceResultListModel.h>
#include <Viewpoint.h>

// Toolkit headers
#include "Internal/DisconnectOnSignal.h"
#include "Internal/DoOnLoad.h"
#include "Internal/GeoViews.h"
#include "UtilityNetworkFunctionTraceResult.h"
#include "UtilityNetworkFunctionTraceResultsModel.h"
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
}
}

UtilityNetworkTraceController::UtilityNetworkTraceController(QObject* parent) :
  QObject(parent),
  m_startingPointParent(new QObject(this)),
  m_startingPointsGraphicsOverlay(new GraphicsOverlay(m_startingPointParent)),
  m_utilityNetworks(new GenericListModel(&UtilityNetworkListItem::staticMetaObject, this)),
  m_startingPoints(new UtilityNetworkTraceStartingPointsModel(this)),
  m_functionResults(new UtilityNetworkFunctionTraceResultsModel(this)),
  m_isAddingStartingPointEnabled(false),
  m_isAddingStartingPointInProgress(false),
  m_startingPointSymbol(new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Cross, QColor(Qt::green), 20.0f, this)),
  m_resultsGraphicsOverlay(new GraphicsOverlay(this))
{
  //
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
    // Maybe the UN is not loaded just yet.
    connectToGeoView(mapView, this, [this, mapView]
    {
      setupUtilityNetworksInt(mapView->map()->utilityNetworks(), m_utilityNetworks);
    });

    connect(this,
            &UtilityNetworkTraceController::selectedUtilityNetworkChanged, // this should've already been handled and newValue used
            this,
            [this](UtilityNetwork* newValue) // when it's used, we also need to use it
    {
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
          std::sort(std::begin(m_traceConfigurations),
                    std::end(m_traceConfigurations),
                    [](const auto& a, const auto& b)
          {
            return a->name() < b->name();
          });

          QStringList traceConfigNamesTemp{};

          for (const auto& traceConfig : m_traceConfigurations)
          {
            traceConfigNamesTemp.append(traceConfig->name());
          }

          setTraceConfigurationNames(traceConfigNamesTemp);

          if (!m_traceConfigurations.isEmpty())
          {
            // select the first trace configuration by default
            m_selectedTraceConfiguration = m_traceConfigurations.at(0);
          }
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
      for (const auto& layer : results)
      {
        for (const auto& geoElement : layer->geoElements())
        {
          const auto ft = dynamic_cast<ArcGISFeature*>(geoElement);
          addStartingPoint(ft, this->m_mapPoint);
        }
      }

      setIsAddingStartingPointInProgress(false);
    });

    // identify symbols on mouse click
    connect(mapView, &MapQuickView::mouseClicked, this, [this, mapView](QMouseEvent& mouseEvent)
    {
      if (isAddingStartingPointInProgress() || !isAddingStartingPointEnabled())
        return;

      setIsAddingStartingPointEnabled(false);
      setIsAddingStartingPointInProgress(true);

      const double tolerance = 10.0;
      const bool returnPopups = false;
      m_mapPoint = mapView->screenToLocation(mouseEvent.pos().x(), mouseEvent.pos().y());

      mapView->identifyLayers(mouseEvent.pos().x(), mouseEvent.pos().y(), tolerance, returnPopups);
    });

    connect(this, &UtilityNetworkTraceController::selectedTraceConfigurationChanged, this, [this]()
    {
      this->applyStartingPointWarnings();
    });

    connect(this, &UtilityNetworkTraceController::startingPointsChanged, this, [this]()
    {
      this->applyStartingPointWarnings();
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

  disconnect(m_selectedUtilityNetwork, nullptr, this, nullptr);

  m_selectedUtilityNetwork = selectedUtilityNetwork;

  connect(m_selectedUtilityNetwork, &UtilityNetwork::traceCompleted, this, &UtilityNetworkTraceController::onTraceCompleted);
  connect(m_selectedUtilityNetwork, &UtilityNetwork::errorOccurred, this, &UtilityNetworkTraceController::onSelectedUtilityNetworkError);
  connect(m_selectedUtilityNetwork, &UtilityNetwork::featuresForElementsCompleted, this, &UtilityNetworkTraceController::onFeaturesForElementsCompleted);

  emit selectedUtilityNetworkChanged(m_selectedUtilityNetwork);
}

QAbstractItemModel* UtilityNetworkTraceController::startingPoints() const
{
  return m_startingPoints;
}

QAbstractItemModel* UtilityNetworkTraceController::functionResults() const
{
  return m_functionResults;
}

QStringList UtilityNetworkTraceController::traceConfigurationNames() const
{
  return m_traceConfigurationNames;
}

void UtilityNetworkTraceController::setTraceConfigurationNames(const QStringList& traceConfigurationNames)
{
  if (m_traceConfigurationNames == traceConfigurationNames)
    return;

  m_traceConfigurationNames = traceConfigurationNames;
  emit traceConfigurationNamesChanged();
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

bool UtilityNetworkTraceController::isAddingStartingPointEnabled() const
{
  return m_isAddingStartingPointEnabled;
}

void UtilityNetworkTraceController::setIsAddingStartingPointEnabled(const bool isAddingStartingPointEnabled)
{
  if (m_isAddingStartingPointEnabled == isAddingStartingPointEnabled)
    return;

  m_isAddingStartingPointEnabled = isAddingStartingPointEnabled;
  emit isAddingStartingPointEnabledChanged();
}

bool UtilityNetworkTraceController::isAddingStartingPointInProgress() const
{
  return m_isAddingStartingPointInProgress;
}

void UtilityNetworkTraceController::setIsAddingStartingPointInProgress(const bool isAddingStartingPointInProgress)
{
  if (m_isAddingStartingPointInProgress == isAddingStartingPointInProgress)
    return;

  m_isAddingStartingPointInProgress = isAddingStartingPointInProgress;
  emit isAddingStartingPointInProgressChanged();
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

bool UtilityNetworkTraceController::isInsufficientStartingPoint() const
{
  return m_isInsufficientStartingPoint;
}

void UtilityNetworkTraceController::setIsInsufficientStartingPoint(bool isInsufficientStartingPoint)
{
  if (m_isInsufficientStartingPoint == isInsufficientStartingPoint)
    return;

  m_isInsufficientStartingPoint = isInsufficientStartingPoint;
  emit isInsufficientStartingPointChanged();
}

bool UtilityNetworkTraceController::isAboveMinimumStartingPoint() const
{
  return m_isAboveMinimumStartingPoint;
}

void UtilityNetworkTraceController::setIsAboveMinimumStartingPoint(bool isAboveMinimumStartingPoint)
{
  if (m_isAboveMinimumStartingPoint == isAboveMinimumStartingPoint)
    return;

  m_isAboveMinimumStartingPoint = isAboveMinimumStartingPoint;
  emit isAboveMinimumStartingPointChanged();
}

bool UtilityNetworkTraceController::isResetResultsEnabled() const
{
  return m_isResetResultsEnabled;
}

void UtilityNetworkTraceController::setIsResetResultsEnabled(bool isResetResultsEnabled)
{
  if (m_isResetResultsEnabled == isResetResultsEnabled)
    return;

  m_isResetResultsEnabled = isResetResultsEnabled;
  emit isResetResultsEnabledChanged();
}

void UtilityNetworkTraceController::runTrace(const QString& /*name*/)
{
  if (isTraceInProgress())
    return;

  resetTraceResults();

  setIsTraceInProgress(true);

  if (m_utilityTraceParameters)
  {
    delete m_utilityTraceParameters;
    m_utilityTraceParameters = nullptr;
  }

  m_utilityTraceParameters = new UtilityTraceParameters(m_selectedTraceConfiguration, m_startingPoints->utilityElements(), this);

  // Async UtilityNetwork::trace
  m_selectedUtilityNetwork->trace(m_utilityTraceParameters);
}

QList<UtilityNamedTraceConfiguration*> UtilityNetworkTraceController::traceConfigurations() const
{
  return m_traceConfigurations;
}

UtilityTraceResultListModel* UtilityNetworkTraceController::traceResults()
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
  m_functionResults->clear();
  m_startingPointsGraphicsOverlay->graphics()->clear();
  m_resultsGraphicsOverlay->graphics()->clear();
  m_isTraceInProgress = false;
  m_isAddingStartingPointEnabled = false;
  m_isAddingStartingPointInProgress = false;
  m_isInsufficientStartingPoint = true;
  m_isAboveMinimumStartingPoint = false;
  m_isResetResultsEnabled = false;
  emit startingPointsChanged();

  if (m_startingPointParent)
  {
    delete m_startingPointParent;
    m_startingPointParent = nullptr;
  }

  m_startingPointParent = new QObject(this);

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

      if (map->utilityNetworks()->size() > 0)
      {
        // TODO append all UNs into a list and allow the user to select which one to load
        // User can load any of them during the app's lifetime.
        // Load the first one by default.
        setSelectedUtilityNetwork(map->utilityNetworks()->at(0));
      }
      else
      {
        qDebug() << "There are no Utility Networks associated with the ArcGIS Map attached "
                 << "to the MapView. Utility Network Trace will not be displayed. Call "
                 << "UtilityNetworkTrace.refresh() after updating the ArcGIS Map to try again.";
        return;
      }
    }
    else
    {
      qDebug() << "Warning: Maps hasn't been loaded yet.";
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
    // This input feature does not belong to this utility network.
    return;
  }

  if (utilityElement)
  {
    const auto startingPointAlreadyExists = m_startingPoints->doesItemAlreadyExist(utilityElement);

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
        }
      }
      else if (utilityElement->networkSource()->sourceType() == UtilityNetworkSourceType::Junction &&
               utilityElement->assetType()->terminalConfiguration() != nullptr)
      {
        auto utilityTerminalConfiguration = utilityElement->assetType()->terminalConfiguration();
        QList<UtilityTerminal*> terminals = utilityTerminalConfiguration->terminals();
        if (terminals.size() > 1)
        {
          // The user can select between multiple terminals, but by default the first one is selected
          utilityElement->setTerminal(utilityElement->assetType()->terminalConfiguration()->terminals().at(0));
        }
      }

      auto graphic = new Graphic(geometry, m_startingPointParent);
      graphic->attributes()->insertAttribute("GlobalId", utilityElement->globalId());
      graphic->setSymbol(m_startingPointSymbol);
      auto featureLayer = dynamic_cast<FeatureLayer*>(identifiedFeature->featureTable()->layer());
      auto symbol = featureLayer->renderer()->symbol(identifiedFeature);

      m_startingPointsGraphicsOverlay->graphics()->append(graphic);
      if (symbol == nullptr)
      {
        // Adding with null symbol
        m_startingPoints->addStartingPoint(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, featureLayer->fullExtent(), m_startingPointParent));
        emit startingPointsChanged();
      }
      else
      {
        // Adding with extent
        m_startingPoints->addStartingPoint(new UtilityNetworkTraceStartingPoint(utilityElement, graphic, symbol, graphic->geometry().extent(), m_startingPointParent));
        emit startingPointsChanged();
      }
    }
  }
}

void UtilityNetworkTraceController::removeStartingPoint(int index)
{
  m_startingPoints->removeAt(index);
  m_startingPointsGraphicsOverlay->graphics()->removeAt(index);
  emit startingPointsChanged();
}

void UtilityNetworkTraceController::zoomToStartingPoint(int index)
{
  if (auto mapView = qobject_cast<MapView*>(m_geoView))
  {
    const Viewpoint currVP = mapView->currentViewpoint(ViewpointType::CenterAndScale);
    const Viewpoint newViewPoint(m_startingPoints->pointAt(index), currVP.targetScale());
    mapView->setViewpoint(newViewPoint, 1.0);
  }
}

void UtilityNetworkTraceController::removeAllStartingPoints()
{
  m_startingPointsGraphicsOverlay->graphics()->clear();
  m_startingPoints->clear();
  emit startingPointsChanged();
}

void UtilityNetworkTraceController::setSelectedTraceConfigurationNameByIndex(int index)
{
  if (m_traceConfigurations.size() > index)
    setSelectedTraceConfiguration(m_traceConfigurations.at(index));
}

void UtilityNetworkTraceController::resetTraceResults()
{
  setIsResetResultsEnabled(false);

  /*** CLEARING GEOMETRY TRACE RESULTS ***/
  m_resultsGraphicsOverlay->graphics()->clear();

  /*** CLEARING FUNCTION TRACE RESULTS ***/
  m_functionResults->clear();

  /*** CLEARING ELEMENT TRACE RESULTS ***/
  auto featuresList = m_selectedUtilityNetwork->featuresForElementsResult();
  QHash<FeatureLayer*, QList<Feature*>> layerToFeatures;
  for (const auto f : *featuresList)
  {
    auto featureLayer = static_cast<FeatureLayer*>(f->featureTable()->layer());

    auto findLayer = layerToFeatures.find(featureLayer);
    if (findLayer == std::end(layerToFeatures))
    {
      layerToFeatures[featureLayer] = {f};
    }
    else
    {
      findLayer.value().append(f);
    }
  }

  const auto layerKeys = layerToFeatures.keys();
  for (const auto lyr : layerKeys)
  {
    // this is where the functionality takes place
    lyr->unselectFeatures(layerToFeatures[lyr]);
  }
}

void UtilityNetworkTraceController::onTraceCompleted()
{
  m_traceResults = m_selectedUtilityNetwork->traceResult();

  QList<UtilityElement*> allElements;
  bool hasGeometryTraceResult{false};

  for (const auto result : *m_traceResults)
  {
    const auto type = result->traceResultObjectType();

    switch (type)
    {
      case UtilityTraceResultObjectType::UtilityElementTraceResult:
      {
        // Trace completed with Element Result
        setIsResetResultsEnabled(true);
        auto elementResult = static_cast<UtilityElementTraceResult*>(result);
        allElements.append(elementResult->elements());
        break;
      }
      case UtilityTraceResultObjectType::UtilityFunctionTraceResult:
      {
        // Trace completed with Function Result
        setIsResetResultsEnabled(true);
        const auto functionResult = static_cast<UtilityFunctionTraceResult*>(result);
        const auto outputList = functionResult->functionOutputs();

        for (const auto o : outputList)
        {
          m_functionResults->addFunctionResult(new UtilityNetworkFunctionTraceResult(o->function()->networkAttribute()->name(),
                                                                                     static_cast<int>(o->function()->functionType()),
                                                                                     o->result().toDouble(), this));
        }

        break;
      }
      case UtilityTraceResultObjectType::UtilityGeometryTraceResult:
      {
        // Trace completed with Geometry Result
        setIsResetResultsEnabled(true);
        auto geometryTraceResult = static_cast<UtilityGeometryTraceResult*>(result);

        auto multipoint = geometryTraceResult->multipoint();
        if (!multipoint.isEmpty())
        {
          SimpleMarkerSymbol* resultPointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle,
                                                                         QColor(0, 0, 255, 126),
                                                                         20,
                                                                         this);
          auto graphic = new Graphic(multipoint,
                                     resultPointSymbol,
                                     this);
          hasGeometryTraceResult = true;
          m_resultsGraphicsOverlay->graphics()->append(graphic);
        }

        auto polyline = geometryTraceResult->polyline();
        if (!polyline.isEmpty())
        {
          SimpleLineSymbol* resultPointSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Dot,
                                                                     QColor(0, 0, 255, 126),
                                                                     5,
                                                                     this);
          auto graphic = new Graphic(polyline,
                                     resultPointSymbol,
                                     this);
          hasGeometryTraceResult = true;
          m_resultsGraphicsOverlay->graphics()->append(graphic);
        }

        auto polygon = geometryTraceResult->polygon();
        if (!polygon.isEmpty())
        {
          SimpleFillSymbol* resultPointSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::ForwardDiagonal,
                                                                     QColor(0, 0, 255, 126),
                                                                     new SimpleLineSymbol(SimpleLineSymbolStyle::Solid,
                                                                                          QColor(0, 0, 255, 126),
                                                                                          2,
                                                                                          this),
                                                                     this);
          auto graphic = new Graphic(polygon,
                                     resultPointSymbol,
                                     this);
          hasGeometryTraceResult = true;
          m_resultsGraphicsOverlay->graphics()->append(graphic);
        }

        break;
      }
      default:
      {
        qDebug() << "Trace completed without usable object type";
        break;
      }
    }

  }

  if (!allElements.isEmpty())
  {
    // calling async featuresForElements with elements
    m_selectedUtilityNetwork->featuresForElements(allElements);
  }
  else
  {
    // when the above async completes, the run trace in-progress will be set to false
    setIsTraceInProgress(false);
  }

  if (hasGeometryTraceResult)
  {
    auto mapView = qobject_cast<MapViewToolkit*>(m_geoView);
    mapView->graphicsOverlays()->append(m_resultsGraphicsOverlay);
  }
}

void UtilityNetworkTraceController::onSelectedUtilityNetworkError(const Error& e)
{
  qDebug() << "selectedUtilityNetwork Error Occurred" << e.message() << "||" << e.additionalMessage();
  setIsTraceInProgress(false);
}

void UtilityNetworkTraceController::onFeaturesForElementsCompleted()
{
  auto featuresList = m_selectedUtilityNetwork->featuresForElementsResult();
  QHash<FeatureLayer*, QList<Feature*>> layerToFeatures;
  for (const auto f : *featuresList)
  {
    auto featureLayer = static_cast<FeatureLayer*>(f->featureTable()->layer());

    auto findLayer = layerToFeatures.find(featureLayer);
    if (findLayer == std::end(layerToFeatures))
    {
      layerToFeatures[featureLayer] = {f};
    }
    else
    {
      findLayer.value().append(f);
    }
  }

  const auto layerKeys = layerToFeatures.keys();
  for (const auto lyr : layerKeys)
  {
    // this is where the functionality takes place
    lyr->selectFeatures(layerToFeatures[lyr]);
  }

  setIsTraceInProgress(false);
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
        // TODO append all UNs into a list and allow the user to select which one to load
        // User can load any of them during the app's lifetime.
        // Load the first one by default.
        setSelectedUtilityNetwork(map->utilityNetworks()->at(0));

        for (const auto un : qAsConst(*map->utilityNetworks()))
        {
          if (un->loadStatus() == LoadStatus::Loaded)
          {
            // append UN without explicit loading
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
                qDebug() << "Utility Network done loading with error:" << e.message() << e.additionalMessage();
                return;
              }
              // append UN with explicit loading
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

void UtilityNetworkTraceController::applyStartingPointWarnings()
{
  if (selectedTraceConfiguration() != nullptr)
  {
    auto minimumStartingLocations = selectedTraceConfiguration()->minimumStartingLocations();
    auto minimum = 1;
    if (minimumStartingLocations == UtilityMinimumStartingLocations::Many)
    {
      minimum = 2;
    }

    setIsInsufficientStartingPoint(m_startingPoints->size() < minimum);
    setIsAboveMinimumStartingPoint(m_startingPoints->size() > minimum);
  }
}

} // Esri::ArcGISRuntime::Toolkit
