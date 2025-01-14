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
#include "BarChartPopupMediaItem.h"

// Qt headers
#include <QBarSet>
#include <QColor>

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
BarChartPopupMediaItem::BarChartPopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
}

BarChartPopupMediaItem::~BarChartPopupMediaItem() = default;

QList<QBarSet*> BarChartPopupMediaItem::barSets()
{
  QList<QBarSet*> barSets;
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();
  const auto chartColors = mediaValue->chartColors();
  const auto colorsHasLessThanLabels = chartColors.size() < popupMediaValueDataLength;

  for (int i = 0; i < popupMediaValueDataLength; i++)
  {
    const auto label = mediaValue->labels().at(i);
    const auto value = mediaValue->data().at(i).toReal();

    if (value >= m_maxValue)
      m_maxValue = value;

    if (value <= m_minValue)
      m_minValue = value;

    const auto barset = new QBarSet(label);
    barset->append(value);

    if (!chartColors.isEmpty() && !colorsHasLessThanLabels)
    {
      const auto color = chartColors.at(i);
      barset->setColor(color);
      barset->setBorderColor(color);
    }
    barSets.append(barset);
  }
  emit barChartPopupMediaItemChanged();
  return barSets;
}

qreal BarChartPopupMediaItem::maxValue() const
{
  return m_maxValue;
}

qreal BarChartPopupMediaItem::minValue() const
{
  return m_minValue;
}

} // namespace Esri::ArcGISRuntime::Toolkit
