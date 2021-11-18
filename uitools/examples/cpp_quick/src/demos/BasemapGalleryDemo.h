#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  class BasemapGalleryController;
}
}
}

class BasemapGalleryDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller READ controller CONSTANT)
public:
  Q_INVOKABLE BasemapGalleryDemo(QObject* parent = nullptr);
  ~BasemapGalleryDemo() override;

  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller() const;

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;

private:
  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* m_controller;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H
