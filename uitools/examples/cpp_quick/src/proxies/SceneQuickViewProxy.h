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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H

#include "GeoModelProxy.h"

#include <QObject>
#include <QPointer>

namespace Esri {
namespace ArcGISRuntime {
  class SceneQuickView;
}
}

class SceneQuickViewProxy : public QObject
{
  Q_OBJECT
  Q_PROPERTY(GeoModelProxy* scene READ scene NOTIFY sceneChangedProxy)
public:
  Q_INVOKABLE explicit SceneQuickViewProxy(QObject* parent = nullptr);
  ~SceneQuickViewProxy() override;

private:
  GeoModelProxy* scene();

signals:
  void sceneChangedProxy();

private:
  Esri::ArcGISRuntime::SceneQuickView* m_sceneQuickView;
  QPointer<GeoModelProxy> m_scene;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SCENEQUCIKVIEWPROXY_H
