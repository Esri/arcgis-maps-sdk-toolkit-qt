/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#include "MetaElement.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \internal

  \inmodule EsriArcGISRuntimeToolkit
  \class Esri::ArcGISRuntime::Toolkit::MetaElement

  \brief The \c MetaElement is an implementation detail of \c GenericListModel
  that deals with helping the model emit dataChanged signals when a property
  updates.

  A \c MetaElement tracks an object in the \c GenericListModel, and updates for
  the given stored index/role fields when the \c propertyChanged signal is
  called.

  Qt connection functions do not allow for connecting a lambda to a
  \c QMetaMethod. This object takes the role of a lambda to track state
  information that needs triggered on a property changed signal.

  \sa Esri::ArcGISRuntime::Toolkit::GenericListModel
 */

/*!
  \brief Constructs a new \c MetaElement.

  \list
  \li \a index When emitDataChanged is called, this is the index used to
  trigger a dataChanged signal on the parent GenericListModel. This is stored
  internally as a QPersistentModelIndex so the index is correctly tracked.

  \li \a customRole When emitDataChanged is called, this is the role enum
  used to trigger a dataChanged signal on the parent GenericListModel.
  Qt::UserRole is always emitted along with this role.

  \li \a trackedObject This is the associated element in the GenericListModel
  which we are tracking.

  \li \a parent The parent ListModel.
  \endlist
 */
MetaElement::MetaElement(
  QModelIndex index,
  int customRole,
  QObject* trackedObject,
  QAbstractItemModel* parent):
  QObject(parent),
  m_index(std::move(index)),
  m_customRole(customRole),
  m_trackedObject(trackedObject),
  m_parentModel(parent)
{
  connect(this, &MetaElement::propertyChanged, this, &MetaElement::emitDataChanged);
  connect(m_trackedObject.data(), &QObject::destroyed, this, &QObject::deleteLater);
}

/*!
  \brief When triggered will emit a dataChangedSignal on the parent
  GenericListModel using the stored index and role as cues.
 */
void MetaElement::emitDataChanged()
{
  if (!m_trackedObject || !m_index.isValid())
    return;

  emit m_parentModel->dataChanged(m_index, m_index, { Qt::UserRole, m_customRole });
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::MetaElement::propertyChanged()
  \brief Signal that when called will trigger the \c emitDataChanged slot.

  In reality this signal takes on the role of a slot as it is invoked by the
  tracked object's property's notify signal.

  This is due to the fact that there is QMetaMethod::fromSignal method but
  no slot equivalent, so this signal takes on the role of a slot for
  \c QMetaMethod connection purposes.
 */

} // Esri
} // ArcGISRuntime
} // Toolkit
