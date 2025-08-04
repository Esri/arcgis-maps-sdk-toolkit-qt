/*******************************************************************************
 *  Copyright 2012-2022 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

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
