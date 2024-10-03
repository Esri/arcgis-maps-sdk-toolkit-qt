
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
#include "TextPopupElementViewController.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::TextPopupElementViewController
  \inmodule ArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief In MVC architecture, this is the controller for the corresponding
  \c TextPopupElementView.
 */

/*!
 \brief Constructor
 \list
   \li \a parent Parent owning \c QObject.
 \endlist
 */
TextPopupElementViewController::TextPopupElementViewController(QObject *parent)
    : QObject{parent}
{
}

  /*!
  \brief Constructor. Takes a \a PopupElement and \a parent object.
 */
TextPopupElementViewController::TextPopupElementViewController(QPointer<TextPopupElement> textPopupElement, QObject *parent) :
    QObject{parent},
    m_textPopupElement{textPopupElement}
{
}

/*!
  \brief Returns the text of the \c TextPopupElement.
*/
QString TextPopupElementViewController::text() const
{
    return m_textPopupElement ? m_textPopupElement.get()->text() : nullptr;
}

/*!
 \brief Returns the PopupElementType of the PopupElement as an int.

  \table
  \header
      \li PopupElementType
      \li Constant
  \row
      \li \c Unknown
      \li \c -1
  \row
      \li \c TextPopupElement
      \li \c 0
  \row
      \li \c FieldsPopupElement
      \li \c 1
  \row
      \li \c MediaPopupElement
      \li \c 2
  \row
      \li \c AttachmentsPopupElement
      \li \c 3
  \endtable

 */
int TextPopupElementViewController::popupElementType() const
{
    return m_textPopupElement ? static_cast<int>(m_textPopupElement->popupElementType()) : -1;
}

/*!
  \brief Returns the \c TextPopupElement.
*/
QPointer<TextPopupElement> TextPopupElementViewController::textPopupElement() const
{
    return m_textPopupElement ? m_textPopupElement : nullptr;
}

/*!
  \brief Sets the \c PopupElement.
  \list
  \li \a popup To deliver data from.
  \endlist
 */
void TextPopupElementViewController::setTextPopupElement(QPointer<TextPopupElement> popupElement)
{

    if (m_textPopupElement == popupElement)
        return;

    if (m_textPopupElement)
        disconnect(m_textPopupElement.data(), nullptr, this, nullptr);

    m_textPopupElement = popupElement;

    if (m_textPopupElement)
        connect(m_textPopupElement.data(), &QObject::destroyed, this, &TextPopupElementViewController::textPopupElementChanged);

    emit textPopupElementChanged();
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::TextPopupElementViewController::textPopupElementChanged()
  \brief Signal emitted when the \c PopupElement changes.
 */
