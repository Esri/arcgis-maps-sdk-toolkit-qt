/*******************************************************************************
 *  Copyright 2012-2024 Esri
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
#include "PopupElementViewItem.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
PopupElementViewItem::PopupElementViewItem(QObject *parent)
  : QObject{parent}
{
}

PopupElementViewItem::PopupElementViewItem(PopupElement* popupElement, QObject* parent)
  : QObject{parent}
  , m_popupElement{std::move(popupElement)}
{
}

PopupElementViewItem::~PopupElementViewItem() = default;

QmlEnums::PopupElementType PopupElementViewItem::popupElementType() const
{
  return static_cast<QmlEnums::PopupElementType>(m_popupElement->popupElementType());
}

PopupElement* PopupElementViewItem::popupElement() const
{
  return m_popupElement ? m_popupElement : nullptr;
}

void PopupElementViewItem::setPopupElement(PopupElement* popupElement)
{
  if (m_popupElement == popupElement)
    return;

  if (m_popupElement)
    disconnect(m_popupElement, nullptr, this, nullptr);

  m_popupElement = popupElement;

  if (m_popupElement)
    connect(m_popupElement,
            &QObject::destroyed,
            this,
            &PopupElementViewItem::popupElementChanged);

  emit popupElementChanged();
}

} // namespace Esri::ArcGISRuntime::Toolkit
