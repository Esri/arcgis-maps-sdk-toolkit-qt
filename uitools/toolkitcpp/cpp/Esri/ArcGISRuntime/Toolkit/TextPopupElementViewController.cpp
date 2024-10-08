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
  : PopupElementViewItem{parent}
{
}

/*!
  \brief Destructor.
  */
TextPopupElementViewController::~TextPopupElementViewController() = default;

/*!
\brief Constructor. Takes a \a PopupElement and \a parent object.
  */
TextPopupElementViewController::TextPopupElementViewController(
    QPointer<TextPopupElement> textPopupElement, QObject *parent)
  : PopupElementViewItem{QPointer<PopupElement>(textPopupElement), parent}
{
}

/*!
  \brief Returns the text of the \c TextPopupElement.
  */
QString TextPopupElementViewController::text() const
{
  return popupElement() ? static_cast<TextPopupElement *>(popupElement().get())->text() : nullptr;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::TextPopupElementViewController::textPopupElementChanged()
  \brief Signal emitted when the \c text changes.
 */
