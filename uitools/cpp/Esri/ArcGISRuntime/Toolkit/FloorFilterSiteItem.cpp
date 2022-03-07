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

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

    /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::FloorFilterSiteItem
    \brief An item contained within \l FloorFilterController::sites. This class wraps
    a \c FloorLevel for easy manipulation/inspection within an AbstractItemModel.
   */

  /*!
    \brief Constructs a new empty FloorFilterSiteItem object with a given \a parent.
   */
  FloorFilterSiteItem::FloorFilterSiteItem(QObject* parent) :
    FloorFilterSiteItem(nullptr, parent)
  {
  }

  /*!
    \brief Constructs a new empty FloorFilterSiteItem object with a given \a parent and
    \a floorSite to wrap.
   */
  FloorFilterSiteItem::FloorFilterSiteItem(FloorSite* floorSite, QObject* parent) :
    QObject(parent),
    m_floorSite(floorSite)
  {
  }

  /*!
   \brief Destructor.
   */
  FloorFilterSiteItem::~FloorFilterSiteItem()
  {
  }

  /*!
   \brief Change the underlying wrapped item to \a floorSite.
   */
  void FloorFilterSiteItem::setFloorSite(FloorSite* floorSite)
  {
    if (m_floorSite == floorSite)
      return;

    m_floorSite = floorSite;
    emit floorSiteChanged();
  }

  /*!
   \brief Returns the currently wrapped \c floorSite.
   */
  FloorSite* FloorFilterSiteItem::floorSite() const
  {
    return m_floorSite;
  }

  /*!
   * \brief Returns the \c siteId from the wrapped item.
   */
  QString FloorFilterSiteItem::modelId() const
  {
    return m_floorSite ? m_floorSite->siteId() : QString{};
  }

  /*!
   * \brief Returns the \c name from the wrapped item.
   */
  QString FloorFilterSiteItem::name() const
  {
    return m_floorSite ? m_floorSite->name() : QString{};
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::FloorFilterSiteItem::floorSiteChanged()
    \brief Signal emitted when the wrapped \l floorSite changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterSiteItem::modelId
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::FloorFilterSiteItem::name
   */


} // Toolkit
} // ArcGISRuntime
} // Esri
