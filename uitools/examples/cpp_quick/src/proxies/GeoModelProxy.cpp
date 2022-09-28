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

#include "GeoModelProxy.h"

#include <GeoModel.h>

/*!
    \internal
    \class GeoModelProxy
    \brief This class exposes GeoModel such that GeoModel.loadStatus is accessible from QML.
    I.e. in the case of wanting to access: `MapView.map.loadStatus` or `SceneView.scene.loadStatus`
    from QML.
*/

GeoModelProxy::GeoModelProxy(QObject* parent) :
  QObject(parent),
  m_geoModel(qobject_cast<Esri::ArcGISRuntime::GeoModel*>(parent))
{
  if (m_geoModel)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_geoModel, &GeoModel::loadStatusChanged, this, &GeoModelProxy::loadStatusChanged);
  }
}

GeoModelProxy::~GeoModelProxy()
{
}

EnumsProxy::LoadStatus GeoModelProxy::loadStatus() const
{
  if (m_geoModel)
  {
    return static_cast<EnumsProxy::LoadStatus>(m_geoModel->loadStatus());
  }
  return EnumsProxy::LoadStatus::LoadStatusUnknown;
}
