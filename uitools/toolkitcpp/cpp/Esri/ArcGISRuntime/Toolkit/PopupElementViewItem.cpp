
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
  \class Esri::ArcGISRuntime::Toolkit::PopupElementViewItem
  \inmodule ArcGISRuntimeToolkit
  \brief This is the base class for a number of different PopupElementViewControllers, such as \l
    {Esri::ArcGISRuntime::Toolkit::TextPopupElementViewController} {TextPopupElementViewController}, and \l
    {Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController} {FieldsPopupElementViewController}, and \l
    {Esri::ArcGISRuntime::Toolkit::AttachmentsPopupElementViewController} {AttachmentsPopupElementViewController}, and \l
    {Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController} {MediaPopupElementViewController}.

    It contains a \c PopupElement and provides access to it's \c PopupElementType. This is used to build a
    GenericListModel to be provided to PopupView which can access the individual PopupElementViewControllers and
    pass them along to the correct QML view.
 */

/*!
 \brief Constructor
 \list
   \li \a parent Parent owning \c QObject.
 \endlist
  */
PopupElementViewItem::PopupElementViewItem(QObject *parent)
  : QObject{parent}
{
}

/*!
\brief Constructor. Takes a \a PopupElement and \a parent object.
  */
PopupElementViewItem::PopupElementViewItem(QPointer<PopupElement> popupElement, QObject *parent)
  : QObject{parent}
  , m_popupElement{popupElement}
{
}

/*!
 \brief Returns the c/ PopupElementType of the c/ PopupElement.
 */
QmlEnums::PopupElementType PopupElementViewItem::popupElementType() const
{
  return static_cast<QmlEnums::PopupElementType>(m_popupElement->popupElementType());
}

/*!
  \brief Returns the \c PopupElement.
*/
QPointer<PopupElement> PopupElementViewItem::popupElement() const
{
  return m_popupElement ? m_popupElement : nullptr;
}

/*!
  \brief Sets the \c PopupElement.
  \list
  \li \a popup To deliver data from.
  \endlist
 */
void PopupElementViewItem::setPopupElement(QPointer<PopupElement> popupElement)
{
  if (m_popupElement == popupElement)
    return;

  if (m_popupElement)
    disconnect(m_popupElement.data(), nullptr, this, nullptr);

  m_popupElement = popupElement;

  if (m_popupElement)
    connect(m_popupElement.data(),
            &QObject::destroyed,
            this,
            &PopupElementViewItem::popupElementChanged);

  emit popupElementChanged();
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupElementViewItem::popupElementChanged()
  \brief Signal emitted when the \c PopupElement changes.
 */
