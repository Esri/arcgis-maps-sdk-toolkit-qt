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

   /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem
    \brief An item contained within \l FloorFilterController::levels. This class wraps
    a \c FloorLevel for easy manipulation/inspection within an AbstractItemModel.
   */

  /*!
    \brief Constructs a new empty FloorFilterLevelItem object with a given \a parent.
   */
  FloorFilterLevelItem::FloorFilterLevelItem(QObject* parent) :
    FloorFilterLevelItem(nullptr, parent)
  {
  }

  /*!
    \brief Constructs a new empty FloorFilterLevelItem object with a given \a parent and
    \a floorLevel to wrap.
   */
  FloorFilterLevelItem::FloorFilterLevelItem(FloorLevel* floorLevel, QObject* parent) :
    QObject(parent),
    m_floorLevel(floorLevel)
  {
  }

  /*!
   \brief Destructor.
   */
  FloorFilterLevelItem::~FloorFilterLevelItem()
  {
  }

  /*!
   \brief Change the underlying wrapped item to \a floorLevel.
   */
  void FloorFilterLevelItem::setFloorLevel(FloorLevel* floorLevel)
  {
    if (m_floorLevel == floorLevel)
      return;

    m_floorLevel = floorLevel;
    emit floorLevelChanged();
  }

  /*!
   \brief Returns the currently wrapped \c floorLevel.
   */
  FloorLevel* FloorFilterLevelItem::floorLevel() const
  {
    return m_floorLevel;
  }

  /*!
   * \brief Returns the \c shortName from the wrapped item.
   */
  QString FloorFilterLevelItem::shortName() const
  {
    return m_floorLevel ? m_floorLevel->shortName() : QString{};
  }

  /*!
   * \brief Returns the \c longName from the wrapped item.
   */
  QString FloorFilterLevelItem::longName() const
  {
    return m_floorLevel ? m_floorLevel->longName() : QString{};
  }

  /*!
   * \brief Returns the \c levelId from the wrapped item.
   */
  QString FloorFilterLevelItem::modelId() const
  {
    return m_floorLevel ? m_floorLevel->levelId() : QString{};
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem::floorLevelChanged()
    \brief Signal emitted when the wrapped \l floorLevel changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem::modelId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem::shortName
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterLevelItem::longName
   */

} // Toolkit
} // ArcGISRuntime
} // Esri
