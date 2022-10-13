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
  class UtilityNamedTraceConfiguration;
  class UtilityNetwork;
  class UtilityNetworkListModel;

namespace Toolkit {

  class UtilityNetworkTraceStartingPoint;
  class UtilityNetworkTraceOperationResult;

  class UtilityNetworkTraceController : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
    Q_PROPERTY(UtilityNetwork* selectedUtilityNetwork READ selectedUtilityNetwork WRITE setSelectedUtilityNetwork NOTIFY selectedUtilityNetworkChanged)
    Q_PROPERTY(QList<UtilityNetworkTraceStartingPoint*> startingPoints READ startingPoints WRITE setStartingPoints NOTIFY startingPointsChanged)
    Q_PROPERTY(UtilityNamedTraceConfiguration* selectedTraceConfiguration READ selectedTraceConfiguration WRITE setSelectedTraceConfiguration NOTIFY selectedTraceConfigurationChanged)
    Q_PROPERTY(bool isTraceInProgress READ isTraceInProgress WRITE setIsTraceInProgress NOTIFY isTraceInProgressChanged)
    Q_PROPERTY(Symbol* startingPointSymbol READ startingPointSymbol WRITE setStartingPointSymbol NOTIFY startingPointSymbolChanged)

  public:
    Q_INVOKABLE explicit UtilityNetworkTraceController(QObject* parent = nullptr);

    ~UtilityNetworkTraceController() override;

    QObject* geoView() const;
    void setGeoView(QObject* mapView);

    GenericListModel* utilityNetworks() const;

    UtilityNetwork* selectedUtilityNetwork() const;
    void setSelectedUtilityNetwork(UtilityNetwork* selectedUtilityNetwork);

    QList<UtilityNetworkTraceStartingPoint*> startingPoints() const;
    void setStartingPoints(QList<UtilityNetworkTraceStartingPoint*> startingPoints);

    UtilityNamedTraceConfiguration* selectedTraceConfiguration() const;
    void setSelectedTraceConfiguration(UtilityNamedTraceConfiguration* selectedTraceConfiguration);

    bool isTraceInProgress() const;
    void setIsTraceInProgress(bool isTraceInProgress);

    Symbol* startingPointSymbol() const;
    void setStartingPointSymbol(Symbol* startingPointSymbol);

    Q_INVOKABLE void runTrace(const QString& name);

    Q_INVOKABLE QList<Esri::ArcGISRuntime::UtilityNamedTraceConfiguration*> traceConfigurations() const;

    Q_INVOKABLE QList<Esri::ArcGISRuntime::Toolkit::UtilityNetworkTraceOperationResult*> traceResults();

    Q_INVOKABLE void refresh();

  signals:
    void geoViewChanged();
    void selectedUtilityNetworkChanged(Esri::ArcGISRuntime::UtilityNetwork* newValue);
    void startingPointsChanged();
    void selectedTraceConfigurationChanged();
    void isTraceInProgressChanged();
    void startingPointSymbolChanged();

  private:
    void populateUtilityNetworksFromMap();
    void addStartingPoint(ArcGISFeature* identifiedFeature, Point mapPoint);
    void removeStartingPoint(UtilityNetworkTraceStartingPoint* startingPoint);
    void setupUtilityNetworks();

    QObject* m_geoView = nullptr;
    GraphicsOverlay* m_startingPointsGraphicsOverlay = nullptr;
    UtilityNetwork* m_selectedUtilityNetwork = nullptr;
    GenericListModel* m_utilityNetworks = nullptr;
    QList<UtilityNamedTraceConfiguration*> m_traceConfigurations;
    UtilityNamedTraceConfiguration* m_selectedTraceConfiguration = nullptr;
    QList<UtilityNetworkTraceStartingPoint*> m_startingPoints;
    QList<UtilityNetworkTraceOperationResult*> m_traceResults;
    bool m_isTraceInProgress = false;
    Symbol* m_startingPointSymbol;
    Point m_mapPoint;
    QMap<QUuid, QMetaObject::Connection> m_traceConfigConnection;
  };

} // Toolkit
} // // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H
