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

// Toolkit headers
#include "UtilityNetworkListItem.h"
#include "Internal/GenericListModel.h"

// ArcGISRuntime headers
#include <GeoView.h>
#include <Portal.h>
#include <UtilityNetwork.h>

// Qt headers
#include <QObject>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class UtilityNetworkTrace;

  class UtilityNetworkTraceController : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
    Q_PROPERTY(QAbstractListModel* utilityNetworks READ utilityNetworks CONSTANT)
    Q_PROPERTY(Portal* portal READ portal WRITE setPortal NOTIFY portalChanged)

  public:
    Q_INVOKABLE UtilityNetworkTraceController(QObject* parent = nullptr);

    ~UtilityNetworkTraceController() override;

    QObject* geoView() const;
    void setGeoView(QObject* mapView);

    GenericListModel* utilityNetworks() const;

    Portal* portal() const;
    void setPortal(Portal* portal);

    //Q_INVOKABLE void zoomToBookmarkExtent(Esri::ArcGISRuntime::Toolkit::BookmarkListItem* bookmark);

  signals:
    void geoViewChanged();
    void portalChanged();

  private:
    void setupLoadingMap();
    void setupLoadingUtilityNetwork();

    QObject* m_geoView = nullptr;
    GenericListModel* m_utilityNetworks = nullptr;
    Portal* m_portal = nullptr;
    UtilityNetwork* m_utilityNetwork = nullptr;
    UtilityNetworkTrace* m_utilityNetworkTrace = nullptr;

  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_UTILITYNETWORKTRACECONTROLLER_H
