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

#include <PopupMedia.h>
#include <PopupMediaValue.h>

namespace Esri::ArcGISRuntime::Toolkit {


PopupMediaItem::PopupMediaItem(QObject *parent)
  : QObject{parent}
{
}

PopupMediaItem::PopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : QObject{parent}, m_popupMedia{popupMedia}
{
}

QString PopupMediaItem::title() const
{
  return m_popupMedia->title();
}

QUrl PopupMediaItem::sourceUrl() const
{
  return m_popupMedia->value()->sourceUrl();
}

QUrl PopupMediaItem::linkUrl() const
{
  return m_popupMedia->value()->linkUrl();
}

QString PopupMediaItem::caption() const
{
  return m_popupMedia->caption();
}

PopupMediaType PopupMediaItem::popupMediaType() const
{
  return m_popupMedia->popupMediaType();
}

} // namespace Esri::ArcGISRuntime::Toolkit
