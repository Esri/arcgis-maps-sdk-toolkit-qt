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

// ArcGISRuntime headers
#include <Point.h>

// Toolkit headers
#include "Internal/GenericListModel.h"
#include "Internal/GeoViews.h"

// Qt headers
#include <QObject>

Q_MOC_INCLUDE("UtilityNetwork.h")
Q_MOC_INCLUDE("UtilityNetworkTraceStartingPoint.h")
Q_MOC_INCLUDE("UtilityNamedTraceConfiguration.h")
Q_MOC_INCLUDE("UtilityNetworkTraceOperationResult.h")
Q_MOC_INCLUDE("Symbol.h")

namespace Esri::ArcGISRuntime {

class ArcGISFeature;
class GraphicsOverlay;
class Symbol;
class UtilityElementTraceResult;
class UtilityNamedTraceConfiguration;
class UtilityNetwork;
class UtilityNetworkListModel;
class UtilityTraceParameters;
class UtilityTraceResultListModel;

namespace Toolkit {

class UtilityNetworkTraceStartingPoint;
class UtilityNetworkTraceStartingPointsModel;
class UtilityNetworkTraceOperationResult;

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
  Q_PROPERTY(QStringList traceConfigurationNames READ traceConfigurationNames NOTIFY traceConfigurationNamesChanged)
  Q_PROPERTY(bool isInsufficientStartingPoint READ isInsufficientStartingPoint WRITE setIsInsufficientStartingPoint NOTIFY isInsufficientStartingPointChanged)
  Q_PROPERTY(bool isAboveMinimumStartingPoint READ isAboveMinimumStartingPoint WRITE setIsAboveMinimumStartingPoint NOTIFY isAboveMinimumStartingPointChanged)

public:
  Q_INVOKABLE explicit UtilityNetworkTraceController(QObject* parent = nullptr);

  ~UtilityNetworkTraceController() override;

  QObject* geoView() const;
  void setGeoView(QObject* mapView);

  GenericListModel* utilityNetworks() const;

  UtilityNetwork* selectedUtilityNetwork() const;
  void setSelectedUtilityNetwork(UtilityNetwork* selectedUtilityNetwork);

  QAbstractItemModel* startingPoints() const;

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

  bool isInsufficientStartingPoint() const;
  void setIsInsufficientStartingPoint(bool isInsufficientStartingPoint);

  bool isAboveMinimumStartingPoint() const;
  void setIsAboveMinimumStartingPoint(bool isAboveMinimumStartingPoint);

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
  void isInsufficientStartingPointChanged();
  void isAboveMinimumStartingPointChanged();

private slots:
  void onTraceCompleted();
  void onSelectedUtilityNetworkError(const Esri::ArcGISRuntime::Error& e);
  void onFeaturesForElementsCompleted();

private:
  void populateUtilityNetworksFromMap();
  void addStartingPoint(ArcGISFeature* identifiedFeature, Point mapPoint);
  void setupUtilityNetworks();
  void applyStartingPointWarnings();

  QObject* m_geoView = nullptr;
  QObject* m_startingPointParent = nullptr;
  GraphicsOverlay* m_startingPointsGraphicsOverlay = nullptr;
  UtilityNetwork* m_selectedUtilityNetwork = nullptr;
  GenericListModel* m_utilityNetworks = nullptr;
  QStringList m_traceConfigurationNames;
  QList<UtilityNamedTraceConfiguration*> m_traceConfigurations;
  UtilityNamedTraceConfiguration* m_selectedTraceConfiguration = nullptr;
  UtilityNetworkTraceStartingPointsModel* m_startingPoints;
  UtilityTraceResultListModel* m_traceResults = nullptr;
  bool m_isTraceInProgress = false;
  bool m_isAddingStartingPointEnabled = false; // if so, user can select points on the map to become starting points
  bool m_isAddingStartingPointInProgress = false; // if so, it's processing selected points on the map to be starting points
  Symbol* m_startingPointSymbol;
  Point m_mapPoint;
  QMap<QUuid, QMetaObject::Connection> m_traceConfigConnection;
  bool m_isInsufficientStartingPoint = true; // during initialization, it cannot be sufficient
  bool m_isAboveMinimumStartingPoint = false; // during initialization, it cannot be more than minimum
  UtilityTraceParameters* m_utilityTraceParameters = nullptr;
  GraphicsOverlay* m_resultsGraphicsOverlay = nullptr;
};

} // Toolkit
} // // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H
