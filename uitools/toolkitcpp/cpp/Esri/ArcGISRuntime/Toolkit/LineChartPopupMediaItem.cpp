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
#include "LineChartPopupMediaItem.h"

// Qt headers
#include <QPointF>

// Maps SDK headers
#include <PopupMedia.h>
#include <PopupMediaValue.h>

// Toolkit headers
#include <PopupMediaItem.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  \class Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media} to be used for a LineSeries in QML.
  */

/*!
  \internal
  \brief Constructor
  \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to populate the list of points.
    \li \a jsonColorArray - The JSON array containing the color information for the line.
    \li \a parent - The optional parent QObject.
  \endlist
 */
LineChartPopupMediaItem::LineChartPopupMediaItem(PopupMedia* popupMedia, const QJsonArray& jsonColorArray, QObject* parent)
  : PopupMediaItem{popupMedia, parent},
    m_jsonColorArr{jsonColorArray}
{
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();

  for (int i = 0; i < popupMediaValueDataLength; i++ )
  {
    const auto label = mediaValue->labels().at(i);
    const auto value = mediaValue->data().at(i).toReal();

    if (value >= m_maxValue)
      m_maxValue = value;

    if (value <= m_minValue)
      m_minValue = value;

    const auto point = QPointF(i, value);
    m_linePoints.append(point);
  }

  m_color = extractColorFromJson(m_jsonColorArr, 0);
}

/*!
  \internal
  \brief Returns the list of \c QPointF objects.
 */
QList<QPointF> LineChartPopupMediaItem::linePoints() const
{
  return m_linePoints;
}

/*!
  \internal
  \brief Returns the color of the line.
 */
QColor LineChartPopupMediaItem::color() const
{
  return m_color;
}

/*!
  \internal
  \brief Returns the maxValue of the points.
 */
qreal LineChartPopupMediaItem::maxValue() const
{
  return m_maxValue;
}

/*!
  \internal
  \brief Returns the minValue of the points.
 */
qreal LineChartPopupMediaItem::minValue() const
{
  return m_minValue;
}

/*!
  \internal
  \brief Returns true if the color was found in the JSON array.
 */
bool LineChartPopupMediaItem::colorFromJsonFound() const
{
  return !m_jsonColorArr.isEmpty();
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \internal
  \fn void Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::lineChartPopupMediaItemChanged()
  \brief Signal emitted when the underlying \c PopupMedia changes.
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::linePoints
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::color
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::maxValue
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::minValue
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::colorFromJsonFound
 */
