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

#include "LocalSceneQuickViewProxy.h"

#include <Scene.h>
#include <LocalSceneQuickView.h>

/*!
    \internal
    \class LocalSceneQuickViewProxy
    \brief This class exposes LocalSceneQuickViewProxy such that LocalSceneView.scene is accessible from QML
    This is registered as an extension to LocalSceneQuickView in main.cpp.
*/

LocalSceneQuickViewProxy::LocalSceneQuickViewProxy(QObject* parent) :
  QObject(parent),
  m_localSceneQuickView(qobject_cast<Esri::ArcGISRuntime::LocalSceneQuickView*>(parent)),
  m_scene(nullptr)
{
  if (m_localSceneQuickView)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_localSceneQuickView, &LocalSceneQuickView::sceneChanged, this, &LocalSceneQuickViewProxy::sceneChangedProxy);
  }
}

LocalSceneQuickViewProxy::~LocalSceneQuickViewProxy() = default;

GeoModelProxy* LocalSceneQuickViewProxy::scene()
{
  if (m_localSceneQuickView)
  {
    auto scene = m_localSceneQuickView->arcGISScene();
    if (!m_scene || m_scene->parent() != scene)
    {
      m_scene = new GeoModelProxy(scene);
    }
    return m_scene;
  }
  return nullptr;
}
