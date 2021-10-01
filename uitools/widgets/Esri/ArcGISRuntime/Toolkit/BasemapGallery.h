#ifndef ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H
#define ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H

#include <QFrame>
#include "Basemap.h"
#include "BasemapGalleryController.h"
namespace Ui {
class BasemapGallery;
}

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
class BasemapGalleryController;

class BasemapGallery : public QFrame
{
  Q_OBJECT

public:
  explicit BasemapGallery(GeoModel* basemap, QWidget *parent = nullptr);
  ~BasemapGallery();

  BasemapGalleryController* controller() const;

  void clickedItem(const QModelIndex &index);
private:
  BasemapGalleryController* m_controller = nullptr;
  Ui::BasemapGallery *m_ui;
  Basemap* _currentBasemap = nullptr;
};


}
}

}


#endif // ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H
