
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
#include "FloorFilterSiteItem.h"

// ArcGISRuntime headers
#include <FloorSite.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

  FloorFilterSiteItem::FloorFilterSiteItem(QObject* parent) :
    FloorFilterSiteItem(nullptr, parent)
  {
  }

  FloorFilterSiteItem::FloorFilterSiteItem(FloorSite* floorSite, QObject* parent) :
    QObject(parent),
    m_floorSite(floorSite)
  {
  }

  FloorFilterSiteItem::~FloorFilterSiteItem()
  {
  }

  void FloorFilterSiteItem::setFloorSite(FloorSite* floorSite)
  {
    if (m_floorSite == floorSite)
      return;

    m_floorSite = floorSite;
    emit floorSiteChanged();
  }

  FloorSite* FloorFilterSiteItem::floorSite() const
  {
    return m_floorSite;
  }

  QString FloorFilterSiteItem::modelId() const
  {
    return m_floorSite ? m_floorSite->siteId() : QString{};
  }

  QString FloorFilterSiteItem::name() const
  {
    return m_floorSite ? m_floorSite->name() : QString{};
  }

} // Esri::ArcGISRuntime::Toolkit
