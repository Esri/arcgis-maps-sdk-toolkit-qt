/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYCONTROLLER_H

// Toolkit headers
#include "BasemapGalleryItem.h"
#include "Internal/GenericListModel.h"

// ArcGISRuntime headers
#include <Basemap.h>
#include <Portal.h>

// Qt headers
#include <QObject>

// Qt forward declarations
class QAbstractListModel;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class BasemapGalleryController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(Portal* portal READ portal WRITE setPortal NOTIFY portalChanged)
  Q_PROPERTY(Basemap* currentBasemap READ currentBasemap NOTIFY currentBasemapChanged)
  Q_PROPERTY(QAbstractListModel* gallery READ gallery CONSTANT)
public:
  Q_INVOKABLE BasemapGalleryController(QObject* parent = nullptr);

  ~BasemapGalleryController() override;

  QObject* geoView() const;

  void setGeoView(QObject* mapView);

  Portal* portal() const;

  void setPortal(Portal* portal);

  Basemap* currentBasemap() const;
  
  GenericListModel* gallery() const;

  Q_INVOKABLE void setCurrentBasemap(Basemap* basemap);

  Q_INVOKABLE bool append(Basemap* basemap);

  Q_INVOKABLE bool append(Basemap* basemap, QImage thumbnail, QString tooltip = {});

  Q_INVOKABLE int basemapIndex(Basemap* basemap) const;

  Q_INVOKABLE bool basemapMatchesCurrentSpatialReference(Basemap* basemap) const;

signals:
  void geoViewChanged();
  void portalChanged();
  void currentBasemapChanged();

private:
  QObject* m_geoView = nullptr;
  Portal* m_portal = nullptr;
  GenericListModel* m_gallery = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYCONTROLLER_H
