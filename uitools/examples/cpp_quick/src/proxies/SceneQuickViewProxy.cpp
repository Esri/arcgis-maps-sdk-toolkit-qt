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
#include "SceneQuickViewProxy.h"

#include <Scene.h>
#include <SceneQuickView.h>

/*!
    \internal
    \class SceneQuickViewProxy
    \brief This class exposes SceneQuickViewProxy such that SceneView.scene is accessible from QML
    This is registered as an extension to SceneQuickView in main.cpp.
*/

SceneQuickViewProxy::SceneQuickViewProxy(QObject* parent) :
  QObject(parent),
  m_sceneQuickView(qobject_cast<Esri::ArcGISRuntime::SceneQuickView*>(parent)),
  m_scene(nullptr)
{
  if (m_sceneQuickView)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_sceneQuickView, &SceneQuickView::sceneChanged, this, &SceneQuickViewProxy::sceneChangedProxy);
  }
}

SceneQuickViewProxy::~SceneQuickViewProxy()
{
}

GeoModelProxy* SceneQuickViewProxy::scene()
{
  if (m_sceneQuickView)
  {
    auto scene = m_sceneQuickView->arcGISScene();
    if (!m_scene || m_scene->parent() != scene)
    {
      m_scene = new GeoModelProxy(scene);
    }
    return m_scene;
  }
  return nullptr;
}
