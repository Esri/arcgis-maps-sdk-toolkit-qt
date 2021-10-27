#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  class BasemapGalleryController;
}
}
}

class BasemapGalleryDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller READ controller CONSTANT)
public:
  BasemapGalleryDemoModel(QObject* parent = nullptr);
  ~BasemapGalleryDemoModel() override;

  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller() const;

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;

private:
  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* m_controller;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMOMODEL_H
