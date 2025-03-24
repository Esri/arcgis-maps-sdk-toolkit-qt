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
#include <PopupViewController.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
ImagePopupMediaItem::ImagePopupMediaItem(PopupMedia* popupMedia, PopupViewController* popupViewController, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
  // bubble up imageClicked signal to PopupViewController. This is the sourceUrl & linkUrl used for ImagePopupMediaItems.
  connect(this, &ImagePopupMediaItem::imageClicked, popupViewController, &PopupViewController::imageClicked);
}

ImagePopupMediaItem::~ImagePopupMediaItem() = default;

QUrl ImagePopupMediaItem::sourceUrl() const
{
  return popupMediaItem()->value()->sourceUrl();
}

QUrl ImagePopupMediaItem::linkUrl() const
{
  return popupMediaItem()->value()->linkUrl();
}

} // namespace Esri::ArcGISRuntime::Toolkit


