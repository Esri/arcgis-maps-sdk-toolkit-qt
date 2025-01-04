
/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
#include "ImagePopupMediaItem.h"

// Maps SDK headers
#include <PopupMedia.h>
#include <PopupMediaValue.h>

// Toolkit headers
#include <PopupMediaItem.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::ImagePopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media} to be used in MediaPopupElementView.
  */

/*!
  \brief Constructor
  \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to expose the image related properties.
    \li \a parent - The optional parent QObject.
  \endlist
 */
ImagePopupMediaItem::ImagePopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
}

/*!
  \brief Returns the sourceUrl of the \c PopupMediaValue.
 */
QUrl ImagePopupMediaItem::sourceUrl() const
{
  return popupMediaItem()->value()->sourceUrl();
}

/*!
  \brief Returns the linkUrl of the \c PopupMediaValue.
 */
QUrl ImagePopupMediaItem::linkUrl() const
{
  return popupMediaItem()->value()->linkUrl();
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::ImagePopupMediaItem::imagePopupMediaItemChanged()
  \brief Signal emitted when the underlying \c PopupMedia changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::ImagePopupMediaItem::sourceUrl
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::ImagePopupMediaItem::linkUrl
 */
