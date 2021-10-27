#include "SceneQuickViewProxy.h"

#include <SceneQuickView.h>
#include <Scene.h>

SceneQuickViewProxy::SceneQuickViewProxy(QObject* parent) :
  QObject(parent),
  m_sceneQuickView(qobject_cast<Esri::ArcGISRuntime::SceneQuickView*>(parent)),
  m_scene(nullptr)
{
  if (m_sceneQuickView)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_sceneQuickView, &SceneQuickView::sceneChanged, this, &SceneQuickViewProxy::sceneChanged);
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
