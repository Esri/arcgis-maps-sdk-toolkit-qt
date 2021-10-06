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

 /*!
  \class Esri::ArcGISRuntime::Toolkit::BasemapGallery
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \since Esri.ArcGISRuntime 100.13
  \brief The user interface for the BasemapGallery.
  The BasemapGallery displays a collection of items representing basemaps from either ArcGIS Online, a user-defined portal,
  or an array of Basemaps. When the user selects a basemap from the BasemapGallery, the  basemap rendered in the current
  geoModel is removed from the given map/scene and replaced with the basemap selected in the gallery.
  */

 /*!
  \brief Constructor.
  \list
    \li \a parent Parent widget.
  \endlist

  View mantains its associated controller, sets up the view itself, its model from the controller and 
  connects listview clicked event to \l BasemapGallery::clickedItem.
  \note geomodel should be manually set by calling \l setGeoModel.
  */
  BasemapGallery::BasemapGallery(QWidget* parent) :
    QFrame(parent),
    m_ui(new Ui::BasemapGallery),
    m_controller(new BasemapGalleryController(this))
  {
    m_ui->setupUi(this);
    GenericListModel* model = m_controller->gallery();
    m_ui->listView->setModel(model);

    //signal-slots
    connect(m_ui->listView, &QListView::clicked, this, &BasemapGallery::clickedItem);
  }

  /*!
  \brief Constructor with geomodel parameter.
  \list
    \li \a geomodel Geomodel.
    \li \a parent Parent widget.
  \endlist

  Calls single QWidget constructor and sets the currentBasemap in the controller as the \a geomodel basemap passed. 
  Sets the geomodel in the controller as \a geomodel.
  */
  BasemapGallery::BasemapGallery(GeoModel* geomodel, QWidget* parent) :
    BasemapGallery(parent)
  {
    m_controller->setCurrentBasemap(geomodel->basemap());
    m_controller->setGeoModel(geomodel);
    //set slot for inital basemap loaded
    connect(geomodel->basemap(), &Basemap::doneLoading, this, &BasemapGallery::initialBasemapLoaded);
    //highlight the intial one
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
    \brief Sets the \a geomodel used by the controller/ This will reset the active basemap
    \sa currentBasemap
   */
  void BasemapGallery::setGeoModel(GeoModel* geomodel)
  {
    if (geomodel != nullptr)
    {
      m_controller->setCurrentBasemap(geomodel->basemap());
      m_controller->setGeoModel(geomodel);
    }
  }

/*!
    \brief Getter of current \c GeoModel .
    returns \c GeoModel stored in the model managed by the controller.
   */
  GeoModel* BasemapGallery::geoModel()
  {
    return m_controller->geoModel();
  }

  /*!
  \internal
  \brief Slot that sets the current basemap with \a index.
  Once linked to the clicked Listview event, receives \c QModelIndex \a index and uses it
  to set its basemap into the controller.
  */
  void BasemapGallery::clickedItem(const QModelIndex& index)
  {
    BasemapGalleryItem* item = m_controller->gallery()->element<BasemapGalleryItem>(index);
    // setting the basemap calculated from the current index. this will also modify the geoview.
    // have to manually check that the flag is noitem because the click from the listview will still be fired
    // by not querying any flag.
    if (item->flags() != Qt::NoItemFlags)
      m_controller->setCurrentBasemap(item->basemap());
  }

  //assuming currentbasemap in controller is initial one
  void BasemapGallery::initialBasemapLoaded()
  {
    Basemap* initial = m_controller->currentBasemap();
    auto model = m_controller->gallery();
    qDebug() << model->rowCount();
    auto index = model->index(2, 0);
    if (index.isValid())
      m_ui->listView->selectionModel()->select(index, QItemSelectionModel::Select);

    //    for(int i = 0; i < model->rowCount(); ++i){
    //      auto index = model->index(i, 0);
    //      if ( index.isValid() )
    //        m_ui->listView->selectionModel()->select(index, QItemSelectionModel::Select);
    //        model->selectionModel()->select( index, QItemSelectionModel::Select );
    //    }
  }
} //Toolkit
} //ArcGISRuntime
} //Esri
