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
  This class is an internal implementation detail and is subject to change.
 */
LineChartPopupMediaItem::LineChartPopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();
  const auto chartColors = mediaValue->chartColors();

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

  if (!chartColors.isEmpty())
  {
    m_color = chartColors.at(0);
  }
}

LineChartPopupMediaItem::~LineChartPopupMediaItem() = default;

QList<QPointF> LineChartPopupMediaItem::linePoints() const
{
  return m_linePoints;
}

QColor LineChartPopupMediaItem::color() const
{
  return m_color;
}

qreal LineChartPopupMediaItem::maxValue() const
{
  return m_maxValue;
}

qreal LineChartPopupMediaItem::minValue() const
{
  return m_minValue;
}

bool LineChartPopupMediaItem::chartColorsEmpty() const
{
  return popupMediaItem()->value()->chartColors().isEmpty();
}

} // namespace Esri::ArcGISRuntime::Toolkit
