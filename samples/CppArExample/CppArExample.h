// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.

#ifndef CppArExample_H
#define CppArExample_H

#include <QObject>
#include "ArcGISArView.h"

namespace Esri {
namespace ArcGISRuntime {
class Scene;
class SceneQuickView;
} // ArcGISRuntime namespace
} // Esri namespace

class CppArExample : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView READ arcGISArView
             WRITE setArcGISArView NOTIFY arcGISArViewChanged)
  Q_PROPERTY(Esri::ArcGISRuntime::SceneQuickView* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)

public:
  explicit CppArExample(QObject* parent = nullptr);
  ~CppArExample() override;

  Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView() const;
  void setArcGISArView(Esri::ArcGISRuntime::Toolkit::ArcGISArView* arcGISArView);

  Esri::ArcGISRuntime::SceneQuickView* sceneView() const;
  void setSceneView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

  Q_INVOKABLE void createEmptyScene();
  Q_INVOKABLE void createStreetsScene();
  Q_INVOKABLE void createImageryScene();
  Q_INVOKABLE void createFullScaleTestScene();
  Q_INVOKABLE void createPointCloudScene();
  Q_INVOKABLE void createYosemiteScene();
  Q_INVOKABLE void createBorderScene();
  Q_INVOKABLE void createBrestScene();
  Q_INVOKABLE void createBerlinScene();
  Q_INVOKABLE void createTabletopTestScene();

signals:
  void arcGISArViewChanged();
  void sceneViewChanged();

private:
  Q_DISABLE_COPY(CppArExample)

  void createSurfaceWithElevation();
  void changeScene(bool withLocationDataSource = false);

  Esri::ArcGISRuntime::Toolkit::ArcGISArView* m_arcGISArView = nullptr;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
  Esri::ArcGISRuntime::Scene* m_scene = nullptr;
};

#endif // CppArExample_H
