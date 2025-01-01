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
#include <PopupMediaItem.h>
#include <PopupMediaValue.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media} to be used for a LineSeries in QML.
  */

/*!
  \brief Constructor
  \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to populate the list of points.
    \li \a jsonColorArray - The JSON array containing the color information for the line.
    \li \a parent - The optional parent QObject.
  \endlist
 */
LineChartPopupMediaItem::LineChartPopupMediaItem(PopupMedia* popupMedia, const QJsonArray& jsonColorArray, QObject* parent)
  : PopupMediaItem{popupMedia, parent},
    m_jsonArr{jsonColorArray}
{
  auto mediaValue = popupMediaItem()->value();
  for (int i = 0; i < mediaValue->data().length(); i++ )
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

  if (!m_jsonArr.isEmpty())
  {
    // color scheme is [Red, Green, Blue, Alpha]
    m_color = QColor(m_jsonArr[0][0].toInt(), m_jsonArr[0][1].toInt(), m_jsonArr[0][2].toInt(), m_jsonArr[0][3].toInt());
  }
}

/*!
  \brief Returns the list of \c QPointF objects.
 */
QList<QPointF> LineChartPopupMediaItem::linePoints() const
{
  return m_linePoints;
}

/*!
  \brief Returns the color of the line.
 */
QColor LineChartPopupMediaItem::color() const
{
  return m_color;
}

/*!
  \brief Returns the maxValue of the points.
 */
qreal LineChartPopupMediaItem::maxValue() const
{
  return m_maxValue;
}

/*!
  \brief Returns the minValue of the points.
 */
qreal LineChartPopupMediaItem::minValue() const
{
  return m_minValue;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::lineChartPopupMediaItemChanged()
  \brief Signal emitted when the underlying \c PopupMedia changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::linePoints
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::color
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::maxValue
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::LineChartPopupMediaItem::minValue
 */
