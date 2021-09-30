#ifndef ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H
#define ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H

#include <QFrame>

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
  explicit BasemapGallery(QWidget *parent = nullptr);
  ~BasemapGallery();

  BasemapGalleryController* controller() const;
  //DEBUG
  void tests(){
    qDebug("signal called");
  }
private:
  BasemapGalleryController* m_controller = nullptr;
  Ui::BasemapGallery *m_ui;
};


}
}

}


#endif // ESRI_ARCGISRUNTIME_BASEMAPGALLERY_H
