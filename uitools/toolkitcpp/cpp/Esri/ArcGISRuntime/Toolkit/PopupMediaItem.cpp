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
#include "PopupMediaItem.h"

// Maps SDK headers
#include <PopupMedia.h>
#include <PopupMediaValue.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::PopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media}. Can be accessed via
  \c{MediaPopupElementViewController::popupMediaItems}.

  The PopupMediaItem is a wrapper around a PopupMedia, along with any additional
  meta-information.
  */

/*!
 \brief Constructor
 \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to populate the view.
    \li \a parent - The optional parent QObject. 
\endlist
 */
PopupMediaItem::PopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : QObject{parent}, m_popupMedia{popupMedia}
{
}

/*!
  \brief Returns the title of the \c MediaPopupElement.
 */
QString PopupMediaItem::title() const
{
  return m_popupMedia->title();
}

/*!
  \brief Returns the sourceUrl of the \c MediaPopupElement.
 */
QUrl PopupMediaItem::sourceUrl() const
{
  return m_popupMedia->value()->sourceUrl();
}

/*!
  \brief Returns the linkUrl of the \c MediaPopupElement.
 */
QUrl PopupMediaItem::linkUrl() const
{
  return m_popupMedia->value()->linkUrl();
}

/*!
  \brief Returns the caption of the \c MediaPopupElement.
 */
QString PopupMediaItem::caption() const
{
  return m_popupMedia->caption();
}

/*!
  \brief Returns the popupMediaType of the \c MediaPopupElement.
 */
PopupMediaType PopupMediaItem::popupMediaType() const
{
  return m_popupMedia->popupMediaType();
}

} // namespace Esri::ArcGISRuntime::Toolkit


/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::title
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::sourceUrl
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::linkUrl
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::caption
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::popupMediaType
 */
