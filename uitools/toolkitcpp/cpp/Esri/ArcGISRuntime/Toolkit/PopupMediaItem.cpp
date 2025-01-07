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
#include "PopupMediaItem.h"

// Qt headers
#include <QColor>
#include <QJsonArray>

// Maps SDK headers
#include <ChartImage.h>
#include <ChartImageParameters.h>
#include <PopupMedia.h>
#include <PopupMediaValue.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
PopupMediaItem::PopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : QObject{parent}, m_popupMedia{popupMedia}
{
}

/*!
  \internal
 */
QString PopupMediaItem::title() const
{
  return m_popupMedia->title();
}

/*!
  \internal
 */
QString PopupMediaItem::caption() const
{
  return m_popupMedia->caption();
}

/*!
  \internal
 */
PopupMediaType PopupMediaItem::popupMediaType() const
{
  return m_popupMedia->popupMediaType();
}

/*!
  \internal
 */
PopupMedia* PopupMediaItem::popupMediaItem() const
{
  return m_popupMedia;
}

/*!
  \internal
 */
QColor PopupMediaItem::extractColorFromJson(const QJsonArray& colorArray, const int index)
{
  if (!colorArray.isEmpty() && index < colorArray.size())
  {
    // color scheme is [Red, Green, Blue, Alpha]
    // https://developers.arcgis.com/web-map-specification/objects/color
    const auto colorArrayAtIndex = colorArray[index];
    const auto r = colorArrayAtIndex[0].toInt();
    const auto g = colorArrayAtIndex[1].toInt();
    const auto b = colorArrayAtIndex[2].toInt();
    const auto a = colorArrayAtIndex[3].toInt();

    return QColor(r, g, b, a);
  }
  return QColor();
}

} // namespace Esri::ArcGISRuntime::Toolkit
