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
#include "FloorFilterFacilityItem.h"

// ArcGISRuntime headers
#include <FloorFacility.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  FloorFilterFacilityItem::FloorFilterFacilityItem(QObject* parent) :
    FloorFilterFacilityItem(nullptr, parent)
  {
  }

  FloorFilterFacilityItem::FloorFilterFacilityItem(FloorFacility* floorFacility, QObject* parent) :
    QObject(parent),
    m_floorFacility(floorFacility)
  {
  }

  FloorFilterFacilityItem::~FloorFilterFacilityItem()
  {
  }

  void FloorFilterFacilityItem::setFloorFacility(FloorFacility* floorFacility)
  {
    if (m_floorFacility == floorFacility)
      return;

    m_floorFacility = floorFacility;
    emit floorFacilityChanged();
  }

  FloorFacility* FloorFilterFacilityItem::floorFacility() const
  {
    return m_floorFacility;
  }

  QString FloorFilterFacilityItem::facilityId() const
  {
    return m_floorFacility ?  m_floorFacility->facilityId() : QString{};
  }

  QString FloorFilterFacilityItem::name() const
  {
    return m_floorFacility ? m_floorFacility->name() : QString{};
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
