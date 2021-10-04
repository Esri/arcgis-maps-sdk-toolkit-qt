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
#include "BasemapGallery.h"

#include "ui_BasemapGallery.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  BasemapGallery::BasemapGallery(QWidget* parent) :
    QFrame(parent),
    m_ui(new Ui::BasemapGallery),
    m_controller(new BasemapGalleryController(this))
  {
    m_ui->setupUi(this);
    GenericListModel* model = m_controller->gallery();
    m_ui->listView->setModel(model);

    /*have to set the property names, so the controller will know how to match the properties from
    basemapgalleryitem with the specific Qt::<namespace> invoked in the .data() from the View (ListView) obj*/
    model->setDisplayPropertyName("name");
    model->setDecorationPropertyName("thumbnail");
    model->setTooltipPropertyName("tooltip");

    //signal-slots
    connect(m_ui->listView, &QListView::clicked, this, &BasemapGallery::clickedItem);
  }

  BasemapGallery::BasemapGallery(GeoModel* geomodel, QWidget* parent) :
    BasemapGallery(parent)
  {
    m_controller->setCurrentBasemap(geomodel->basemap());
    m_controller->setGeoModel(geomodel);
  }

  BasemapGallery::~BasemapGallery()
  {
    delete m_ui;
  }

  BasemapGalleryController* BasemapGallery::controller() const
  {
    return m_controller;
  }

  void BasemapGallery::setGeoModel(GeoModel* geomodel)
  {
    if (geomodel != nullptr)
    {
      m_controller->setCurrentBasemap(geomodel->basemap());
      m_controller->setGeoModel(geomodel);
    }
  }

  GeoModel* BasemapGallery::getCurrentGeoModel()
  {
    return m_controller->geoModel();
  }

  void BasemapGallery::clickedItem(const QModelIndex& index)
  {
    BasemapGalleryItem* item = m_controller->gallery()->element<BasemapGalleryItem>(index);
    //setting the basemap calculated from the current index. this will also modify the geoview
    m_controller->setCurrentBasemap(item->basemap());
  }
}
}
}
