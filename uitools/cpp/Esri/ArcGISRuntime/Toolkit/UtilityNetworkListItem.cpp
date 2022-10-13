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
#include "UtilityNetworkListItem.h"

// ArcGISRuntime headers
#include <UtilityNetwork.h>

namespace Esri::ArcGISRuntime::Toolkit {

    /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::UtilityNetworkListItem
    \brief An item contained within \l UtilityNetworkTraceController::utilityNetworks. This class wraps
    a \c UtilityNetwork for easy manipulation/inspection within an AbstractItemModel.
   */

    /*!
    \brief Constructs a new empty UtilityNetworkListItem object with a given \a parent.
   */
    UtilityNetworkListItem::UtilityNetworkListItem(QObject* parent) :
      UtilityNetworkListItem(nullptr, parent)
    {
    }

    /*!
    \brief Constructs a new UtilityNetworkListItem object with a given \a utilityNetwork and
    \a parent.
   */
      UtilityNetworkListItem::UtilityNetworkListItem(UtilityNetwork* utilityNetwork, QObject* parent) :
        QObject(parent),
        m_utilityNetwork(utilityNetwork)
      {
        connect(this, &UtilityNetworkListItem::utilityNetworkChanged, this, &UtilityNetworkListItem::nameChanged);
      }

    /*!
    \brief Destructor.
   */
    UtilityNetworkListItem::~UtilityNetworkListItem() = default;

    /*!
    \brief Change the underlying item to \a utilityNetwork.
   */
    void UtilityNetworkListItem::setUtilityNetwork(UtilityNetwork* utilityNetwork)
    {
      if (m_utilityNetwork == utilityNetwork)
      {
        return;
      }

      m_utilityNetwork = utilityNetwork;
      emit utilityNetworkChanged();
    }

    /*!
    \brief Returns the current \c utilityNetwork.
   */
    UtilityNetwork* UtilityNetworkListItem::utilityNetwork() const
    {
      return m_utilityNetwork;
    }

    /*!
    \property Esri::ArcGISRuntime::Toolkit::UtilityNetworkListItem::name
    \brief Returns the name of the item.
   */
    QString UtilityNetworkListItem::name() const
    {
      return m_utilityNetwork ? m_utilityNetwork->name() : QString{};
    }

    /*!
    \fn void Esri::ArcGISRuntime::Toolkit::UtilityNetworkListItem::utilityNetworkChanged()
    \brief Signal emitted when the \l utilityNetwork changes.
   */

    /*!
    \fn void Esri::ArcGISRuntime::Toolkit::UtilityNetworkListItem::nameChanged()
    \brief Signal emitted when the \l utilityNetwork name changes.
   */

} // Esri::ArcGISRuntime::Toolkit
