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

// Toolkit headers
#include "BasemapGalleryController.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

 /*!
  \class Esri::ArcGISRuntime::Toolkit::BasemapGallery
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \since Esri.ArcGISRuntime 100.13
  \brief The user interface for the BasemapGallery.
  The BasemapGallery displays a collection of items representing basemaps from either ArcGIS Online, a user-defined portal,
  or an array of Basemaps. When the user selects a basemap from the BasemapGallery, the basemap rendered in the current
  geoModel is removed from the given map/scene and replaced with the basemap selected in the gallery.
  */

 /*!
  \brief Default constructor.
  \list
    \li \a parent Parent widget.
  \endlist

  View mantains its associated controller, sets up the view itself and its model.
  \note If this constructor is used, a \c GeoModel must be set separately using \l setGeoModel.
  */
 BasemapGallery::BasemapGallery(QWidget* parent) :
   QFrame(parent),
   m_controller(new BasemapGalleryController(this)),
   m_ui(new Ui::BasemapGallery)
 {
   m_ui->setupUi(this);
   GenericListModel* model = m_controller->gallery();
   m_ui->listView->setModel(model);

   //signal-slots
   // both are needed for setting the initial basemap or in case a new basemap is loaded by changing the geomodel.
   connect(m_ui->listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &BasemapGallery::onItemSelected);
   connect(m_controller, &BasemapGalleryController::currentBasemapChanged, this, &BasemapGallery::onCurrentBasemapChanged);
  }

  /*!
  \brief Constructor that takes a GeoModel.
  \list
    \li \a geomodel GeoModel.
    \li \a parent Parent widget.
  \endlist

  The provided GeoModel's basemap will be set as the BasemapGallery's currentBasemap.
  */
  BasemapGallery::BasemapGallery(GeoModel* geomodel, QWidget* parent) :
    BasemapGallery(parent)
  {
    m_controller->setGeoModel(geomodel);
  }

  /*!
    \brief Destructor.
   */
  BasemapGallery::~BasemapGallery()
  {
    delete m_ui;
  }

  /*!
    \brief Returns the controller.
   */
  BasemapGalleryController* BasemapGallery::controller() const
  {
    return m_controller;
  }

  /*!
    \brief Sets the \a geomodel used by the controller.

    Setting this will reset the active basemap in the gallery. When a new basemap
    item is selected from the gallery, the GeoModel will be updated with the associated
    basemap.
    \note If \a geomodel is passed as \c nullptr, the current geomodel is unset.
    \sa BasemapGalleryController::currentBasemap
   */
  void BasemapGallery::setGeoModel(GeoModel* geomodel)
  {
    m_controller->setGeoModel(geomodel);
  }

/*!
    \brief Returns the current \c GeoModel used by the BasemapGallery.
    \sa setGeoModel.
   */
  GeoModel* BasemapGallery::geoModel()
  {
    return m_controller->geoModel();
  }

  /*!
  \internal
  \brief Slot that sets the current basemap with \a index.
  Once linked to the clicked ListView event, this slot receives \c QModelIndex \a index and uses it
  to set its basemap into the controller.
  */
  void BasemapGallery::onItemSelected(const QModelIndex& index)
  {
    BasemapGalleryItem* item = m_controller->gallery()->element<BasemapGalleryItem>(index);
    m_controller->setCurrentBasemap(item->basemap());
  }

  /*!
   \internal
   \brief Slot that sets the selection on the BasemapGallery view when the current basemap changes.
   */
  void BasemapGallery::onCurrentBasemapChanged()
  {
    auto idx = m_controller->basemapIndex(m_controller->currentBasemap());
    auto index = m_controller->gallery()->index(idx);
    //index can be invalid in case the current basemap is not part of the gallery items (idx = -1)
    if (!index.isValid())
      return;
    m_ui->listView->selectionModel()->select(index, QItemSelectionModel::Select);
  }
} //Toolkit
} //ArcGISRuntime
} //Esri
