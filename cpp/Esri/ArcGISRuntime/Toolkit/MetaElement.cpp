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

void MetaElement::emitDataChanged()
{
  if (!m_trackedObject || !m_index.isValid())
    return;
  else
    emit m_parentModel->dataChanged(m_index, m_index, { Qt::UserRole, m_customRole });
}

} // Esri
} // ArcGISRuntime
} // Toolkit
