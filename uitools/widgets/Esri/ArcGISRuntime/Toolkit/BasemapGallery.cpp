#include "BasemapGallery.h"
#include "ui_BasemapGallery.h"


//Toolkit controllers
#include "BasemapGalleryController.h"
namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
BasemapGallery::BasemapGallery(QWidget *parent) :
  QFrame(parent),
  m_ui(new Ui::BasemapGallery), m_controller(new BasemapGalleryController(this))
{
  m_ui->setupUi(this);
  GenericListModel* model = m_controller->gallery();
  m_ui->listView->setModel(model);
  model->setDisplayPropertyName("name");
  model->setDecorationPropertyName("thumbnail");
  model->setTooltipPropertyName("tooltip");
  //delegate

  //signal-slots
  connect(m_ui->listView, &QListView::clicked, this, &BasemapGallery::tests);
  }

  BasemapGallery::~BasemapGallery()
  {
    delete m_ui;
  }

  BasemapGalleryController* BasemapGallery::controller() const {
    return m_controller;
  }
}
}
}

void basemapChanged(){
  qDebug() << "basemap changed";
}
