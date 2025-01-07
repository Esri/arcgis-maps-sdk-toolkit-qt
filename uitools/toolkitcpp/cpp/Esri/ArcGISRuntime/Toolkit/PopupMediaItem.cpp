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

  \class Esri::ArcGISRuntime::Toolkit::PopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief This is the base class for the following PopupMediaItems, \l
  {Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem} {BarChartPopupMediaItem}, and \l
  {Esri::ArcGISRuntime::Toolkit::PieChartPopupMediaItem} {PieChartPopupMediaItem}, and \l
  {Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem} {LineChartPopupMediaItem}.

  It contains a \c PopupMedia and exposes the properties \c PopupElementType, \c title, \c caption. This is used to build a
  GenericListModel to be provided to MediaPopupElementView which can access the individual PopupMediaItems and
  construct the appropriate QML components.
*/

/*!
  \internal
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
  \internal
  \brief Returns the title of the \c MediaPopupElement.
 */
QString PopupMediaItem::title() const
{
  return m_popupMedia->title();
}

/*!
  \internal
  \brief Returns the caption of the \c MediaPopupElement.
 */
QString PopupMediaItem::caption() const
{
  return m_popupMedia->caption();
}

/*!
  \internal
  \brief Returns the popupMediaType of the \c MediaPopupElement.
 */
PopupMediaType PopupMediaItem::popupMediaType() const
{
  return m_popupMedia->popupMediaType();
}

/*!
  \internal
  \brief Returns the PopupMedia of the \c MediaPopupElement.
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

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::title
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::caption
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::PopupMediaItem::popupMediaType
 */
