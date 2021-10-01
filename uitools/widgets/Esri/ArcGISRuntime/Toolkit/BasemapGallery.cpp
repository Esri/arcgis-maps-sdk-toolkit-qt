#include "BasemapGallery.h"
#include "ui_BasemapGallery.h"


//Toolkit controllers

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
BasemapGallery::BasemapGallery(GeoModel* geomodel, QWidget *parent) :
  QFrame(parent),
  m_ui(new Ui::BasemapGallery), m_controller(new BasemapGalleryController(this))
{
  m_ui->setupUi(this);
  //setting the controller basemap and geoview
  m_controller->setCurrentBasemap(geomodel->basemap());
  m_controller->setGeoModel(geomodel);

  GenericListModel* model = m_controller->gallery();
  m_ui->listView->setModel(model);
  /*have to set the property names, so the controller will know how to match the properties from
  basemapgalleryitem with the specific Qt::<namespace> invoked in the .data() from the View (ListView) obj*/
  model->setDisplayPropertyName("name");
  model->setDecorationPropertyName("thumbnail");
  model->setTooltipPropertyName("tooltip");
  //delegate

  //signal-slots
  connect(m_ui->listView, &QListView::clicked, this, &BasemapGallery::clickedItem);
  }

  BasemapGallery::~BasemapGallery()
  {
    delete m_ui;
  }

  BasemapGalleryController* BasemapGallery::controller() const {
    return m_controller;
  }

  /*slot used to receive the clicked event on the listview
   *
   **/
  void BasemapGallery::clickedItem(const QModelIndex &index){
    QObject* qvar = m_controller->gallery()->element(index);
    BasemapGalleryItem* item = qobject_cast<BasemapGalleryItem*>(qvar);
    //setting the basemap calculated from the current index. this will also modify the geoview
    m_controller->setCurrentBasemap(item->basemap());

  }
}
}
}
