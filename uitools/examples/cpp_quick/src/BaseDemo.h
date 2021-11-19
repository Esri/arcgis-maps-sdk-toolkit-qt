// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEDEMO_H

#include <QObject>
#include <QQmlEngine>

namespace Esri {
namespace ArcGISRuntime {
  class GeoView;
  class GeoModel;
  class Map;
  class MapQuickView;
  class Scene;
  class SceneQuickView;
}
}

class BaseDemo : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QObject* geoView READ geoView_ WRITE setGeoView_ NOTIFY geoViewChanged)
  QML_ELEMENT
public:
  Q_INVOKABLE explicit BaseDemo(QObject* parent = nullptr);
  ~BaseDemo() override;

  Esri::ArcGISRuntime::GeoView* geoView() const;
  void setGeoView(Esri::ArcGISRuntime::GeoView* geoView);

  Esri::ArcGISRuntime::GeoModel* geoModel() const;
  bool setGeoModel(Esri::ArcGISRuntime::Map* map);
  bool setGeoModel(Esri::ArcGISRuntime::Scene* scene);

protected:
  virtual Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const;
  virtual Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const;

  // Helper util that allows a function `f` that takes a single `GeoView` parameter to
  // work agnostically on a SceneView or MapView. This reduces boilerplate around signals.
  // I.E.:
  // \begincode
  //  apply([this](auto geoView)
  //        {
  //          using ViewType = std::remove_pointer_t<decltype(geoView)>;
  //          connect(geoView, &ViewType::attributionTextChanged, this, []
  //                  {
  //                    qDebug() << "Attribution rect changed";
  //                  });
  //        });
  // \endcode
  template <typename Func>
  auto apply(Func&& f)
  {
    using namespace Esri::ArcGISRuntime;
    if (auto mapView = qobject_cast<MapQuickView*>(m_geoView))
    {
      return f(mapView);
    }
    else if (auto sceneView = qobject_cast<SceneQuickView*>(m_geoView))
    {
      return f(sceneView);
    }
    else
    {
      // Since `f` is for agnostic calls the type of the nullptr
      // doesn't matter if it's a MapQuickView or SceneQuickView.
      return f(static_cast<MapQuickView*>(nullptr));
    }
  }

private:
  QObject* geoView_() const;
  void setGeoView_(QObject* geoView);

signals:
  void geoViewChanged();

private:
  QObject* m_geoView;
};

#endif // #ifndef #ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEDEMO_H
