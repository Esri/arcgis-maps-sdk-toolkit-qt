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
#include "FloorFilterLevelItem.h"

// ArcGISRuntime headers
#include <FloorLevel.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  FloorFilterLevelItem::FloorFilterLevelItem(QObject* parent) :
    FloorFilterLevelItem(nullptr, parent)
  {
  }

  FloorFilterLevelItem::FloorFilterLevelItem(FloorLevel* floorLevel, QObject* parent) :
    QObject(parent),
    m_floorLevel(floorLevel)
  {
  }

  FloorFilterLevelItem::~FloorFilterLevelItem()
  {
  }

  void FloorFilterLevelItem::setFloorLevel(FloorLevel* floorLevel)
  {
    if (m_floorLevel == floorLevel)
      return;

    m_floorLevel = floorLevel;
    emit floorLevelChanged();
  }

  FloorLevel* FloorFilterLevelItem::floorLevel() const
  {
    return m_floorLevel;
  }

  QString FloorFilterLevelItem::shortName() const
  {
    return m_floorLevel ? m_floorLevel->shortName() : QString{};
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
