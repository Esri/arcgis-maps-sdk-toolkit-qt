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
#include <FloorSite.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

    /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem
    \brief An item contained within \l FloorFilterController::facilities. This class wraps
    a \c FloorFacility for easy manipulation/inspection within an AbstractItemModel.
   */

  /*!
    \brief Constructs a new empty FloorFilterFacilityItem object with a given \a parent.
   */
  FloorFilterFacilityItem::FloorFilterFacilityItem(QObject* parent) :
    FloorFilterFacilityItem(nullptr, parent)
  {
  }

  /*!
    \brief Constructs a new empty FloorFilterFacilityItem object with a given \a parent and
    \a floorFacility to wrap.
   */
  FloorFilterFacilityItem::FloorFilterFacilityItem(FloorFacility* floorFacility, QObject* parent) :
    QObject(parent),
    m_floorFacility(floorFacility)
  {
  }

  /*!
   \brief Destructor.
   */
  FloorFilterFacilityItem::~FloorFilterFacilityItem()
  {
  }

  /*!
   \brief Change the underlying wrapped item to \a floorFacility.
   */
  void FloorFilterFacilityItem::setFloorFacility(FloorFacility* floorFacility)
  {
    if (m_floorFacility == floorFacility)
      return;

    m_floorFacility = floorFacility;
    emit floorFacilityChanged();
  }

  /*!
   \brief Returns the currently wrapped \c floorFacility.
   */
  FloorFacility* FloorFilterFacilityItem::floorFacility() const
  {
    return m_floorFacility;
  }

  /*!
   * \brief Returns the \c facilityId from the wrapped item.
   */
  QString FloorFilterFacilityItem::modelId() const
  {
    return m_floorFacility ?  m_floorFacility->facilityId() : QString{};
  }

  /*!
   * \brief Returns the name from the wrapped item.
   */
  QString FloorFilterFacilityItem::name() const
  {
    return m_floorFacility ? m_floorFacility->name() : QString{};
  }

  /*!
   * \brief Returns the name of the parent site.
   */
  QString FloorFilterFacilityItem::parentSiteName() const
  {
    auto site = m_floorFacility ? m_floorFacility->site() : nullptr;
    return site ? site->name() : QString{};
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem::floorFacilityChanged()
    \brief Signal emitted when the wrapped \l floorFacility changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem::modelId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem::name
   */

   /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterFacilityItem::parentSiteName
   */
} // Toolkit
} // ArcGISRuntime
} // Esri
