
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

// Maps SDK headers
#include <PopupElement.h>
#include <TextPopupElement.h>

// Toolkit headers
#include "PopupViewController.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
TextPopupElementViewController::TextPopupElementViewController(
    TextPopupElement* textPopupElement, PopupViewController* popupViewController, QObject* parent)
  : PopupElementViewItem{textPopupElement, parent}
{
  // bubble up signal to PopupViewController
  connect(this, &TextPopupElementViewController::clickedUrl, popupViewController, &PopupViewController::clickedUrl);
}

TextPopupElementViewController::~TextPopupElementViewController() = default;

QString TextPopupElementViewController::text() const
{
  return popupElement() ? static_cast<TextPopupElement*>(popupElement())->text() : nullptr;
}

} // namespace Esri::ArcGISRuntime::Toolkit
