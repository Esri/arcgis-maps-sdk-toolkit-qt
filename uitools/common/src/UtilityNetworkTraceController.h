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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H

// Qt headers
#include <QObject>

// ArcGIS Maps SDK headers
#include <Point.h>

// Other headers
#include "GenericListModel.h"
#include "GeoViews.h"

Q_MOC_INCLUDE("UtilityNetwork.h")
Q_MOC_INCLUDE("UtilityNetworkTraceStartingPoint.h")
Q_MOC_INCLUDE("UtilityNamedTraceConfiguration.h")
Q_MOC_INCLUDE("Symbol.h")

namespace Esri::ArcGISRuntime {

class ArcGISFeature;
class ErrorException;
class GraphicsOverlay;
class SimpleFillSymbol;
class SimpleLineSymbol;
class SimpleMarkerSymbol;
class Symbol;
class UtilityElementTraceResult;
class UtilityNamedTraceConfiguration;
class UtilityNetwork;
class UtilityNetworkListModel;
class UtilityTraceParameters;
class UtilityTraceResultListModel;

namespace Toolkit {

class UtilityNetworkFunctionTraceResultsModel;
class UtilityNetworkTraceStartingPoint;
class UtilityNetworkTraceStartingPointsModel;

class UtilityNetworkTraceController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(UtilityNetwork* selectedUtilityNetwork READ selectedUtilityNetwork WRITE setSelectedUtilityNetwork NOTIFY selectedUtilityNetworkChanged)
  Q_PROPERTY(UtilityNamedTraceConfiguration* selectedTraceConfiguration READ selectedTraceConfiguration WRITE setSelectedTraceConfiguration NOTIFY selectedTraceConfigurationChanged)
  Q_PROPERTY(bool isTraceInProgress READ isTraceInProgress WRITE setIsTraceInProgress NOTIFY isTraceInProgressChanged)
  Q_PROPERTY(bool isAddingStartingPointEnabled READ isAddingStartingPointEnabled WRITE setIsAddingStartingPointEnabled NOTIFY isAddingStartingPointEnabledChanged)
  Q_PROPERTY(bool isAddingStartingPointInProgress READ isAddingStartingPointInProgress WRITE setIsAddingStartingPointInProgress NOTIFY isAddingStartingPointInProgressChanged)
  Q_PROPERTY(Symbol* startingPointSymbol READ startingPointSymbol WRITE setStartingPointSymbol NOTIFY startingPointSymbolChanged)
  Q_PROPERTY(QAbstractItemModel* startingPoints READ startingPoints CONSTANT)
  Q_PROPERTY(QAbstractItemModel* functionResults READ functionResults CONSTANT)
  Q_PROPERTY(QStringList traceConfigurationNames READ traceConfigurationNames NOTIFY traceConfigurationNamesChanged)
  Q_PROPERTY(bool isInsufficientStartingPoints READ isInsufficientStartingPoints WRITE setIsInsufficientStartingPoints NOTIFY isInsufficientStartingPointsChanged)
  Q_PROPERTY(bool isAboveMinimumStartingPoint READ isAboveMinimumStartingPoint WRITE setIsAboveMinimumStartingPoint NOTIFY isAboveMinimumStartingPointChanged)
  Q_PROPERTY(bool isResetResultsEnabled READ isResetResultsEnabled WRITE setIsResetResultsEnabled NOTIFY isResetResultsEnabledChanged)

public:
  Q_INVOKABLE explicit UtilityNetworkTraceController(QObject* parent = nullptr);

  ~UtilityNetworkTraceController() override;

  QObject* geoView() const;
  void setGeoView(QObject* mapView);

  UtilityNetwork* selectedUtilityNetwork() const;
  void setSelectedUtilityNetwork(UtilityNetwork* selectedUtilityNetwork);

  QAbstractItemModel* startingPoints() const;

  QAbstractItemModel* functionResults() const;

  QStringList traceConfigurationNames() const;
  void setTraceConfigurationNames(const QStringList& traceConfigurationNames);

  UtilityNamedTraceConfiguration* selectedTraceConfiguration() const;
  void setSelectedTraceConfiguration(UtilityNamedTraceConfiguration* selectedTraceConfiguration);

  bool isTraceInProgress() const;
  void setIsTraceInProgress(const bool isTraceInProgress);

  bool isAddingStartingPointEnabled() const;
  void setIsAddingStartingPointEnabled(const bool isAddingStartingPointEnabled);

  bool isAddingStartingPointInProgress() const;
  void setIsAddingStartingPointInProgress(const bool isAddingStartingPointInProgress);

  Symbol* startingPointSymbol() const;
  void setStartingPointSymbol(Symbol* startingPointSymbol);

  bool isInsufficientStartingPoints() const;
  void setIsInsufficientStartingPoints(bool isInsufficientStartingPoints);

  bool isAboveMinimumStartingPoint() const;
  void setIsAboveMinimumStartingPoint(bool isAboveMinimumStartingPoint);

  bool isResetResultsEnabled() const;
  void setIsResetResultsEnabled(bool isResetResultsEnabled);

  Q_INVOKABLE void runTrace(const QString& name);

  QList<Esri::ArcGISRuntime::UtilityNamedTraceConfiguration*> traceConfigurations() const;

  Q_INVOKABLE UtilityTraceResultListModel* traceResults();

  Q_INVOKABLE void refresh();

  Q_INVOKABLE void removeStartingPoint(int index);

  Q_INVOKABLE void zoomToStartingPoint(int index);

  Q_INVOKABLE void removeAllStartingPoints();

  Q_INVOKABLE void setSelectedTraceConfigurationNameByIndex(int index);

  Q_INVOKABLE void resetTraceResults();

signals:
  void geoViewChanged();
  void selectedUtilityNetworkChanged(Esri::ArcGISRuntime::UtilityNetwork* newValue);
  void startingPointsChanged();
  void traceConfigurationNamesChanged();
  void selectedTraceConfigurationChanged();
  void isTraceInProgressChanged();
  void isAddingStartingPointEnabledChanged();
  void isAddingStartingPointInProgressChanged();
  void startingPointSymbolChanged();
  void isInsufficientStartingPointsChanged();
  void isAboveMinimumStartingPointChanged();
  void isResetResultsEnabledChanged();

private slots:
  void onTraceCompleted();
  void onSelectedUtilityNetworkError(const Esri::ArcGISRuntime::ErrorException& e);
  void onFeaturesForElementsCompleted();

private:
  void addStartingPoint(ArcGISFeature* identifiedFeature, const Point& mapPoint);
  void setupUtilityNetworks();
  void applyStartingPointWarnings();

  QObject* m_geoView = nullptr;
  QObject* m_startingPointParent = nullptr;
  GraphicsOverlay* m_startingPointsGraphicsOverlay = nullptr;
  UtilityNetwork* m_selectedUtilityNetwork = nullptr;
  QStringList m_traceConfigurationNames;
  QList<UtilityNamedTraceConfiguration*> m_traceConfigurations;
  UtilityNamedTraceConfiguration* m_selectedTraceConfiguration = nullptr;
  UtilityNetworkTraceStartingPointsModel* m_startingPoints;
  UtilityNetworkFunctionTraceResultsModel* m_functionResults;
  UtilityTraceResultListModel* m_traceResults = nullptr;
  bool m_isTraceInProgress = false;
  bool m_isAddingStartingPointEnabled = false; // if so, user can select points on the map to become starting points
  bool m_isAddingStartingPointInProgress = false; // if so, it's processing selected points on the map to be starting points
  Symbol* m_startingPointSymbol;
  Point m_mapPoint;
  bool m_isInsufficientStartingPoints = true; // during initialization, it cannot be sufficient
  bool m_isAboveMinimumStartingPoint = false; // during initialization, it cannot be more than minimum
  bool m_isResetResultsEnabled = false;
  UtilityTraceParameters* m_utilityTraceParameters = nullptr;
  GraphicsOverlay* m_resultsGraphicsOverlay = nullptr;

  // these won't get refreshed in refresh() because they shouldn't change
  SimpleMarkerSymbol* m_resultPointSymbol = nullptr;
  SimpleLineSymbol* m_resultLineSymbol = nullptr;
  SimpleFillSymbol* m_resultFillSymbol = nullptr;
};

} // Toolkit
} // // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H
