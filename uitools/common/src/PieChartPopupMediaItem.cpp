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
#include "PieChartPopupMediaItem.h"

// Qt headers
#include <QColor>
#include <QPieSlice>

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
PieChartPopupMediaItem::PieChartPopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
}

PieChartPopupMediaItem::~PieChartPopupMediaItem() = default;

QList<QPieSlice*> PieChartPopupMediaItem::pieSlices()
{
  QList<QPieSlice*> pieSlices;
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();
  const auto chartColors = mediaValue->chartColors();

  for (int i = 0; i < popupMediaValueDataLength; i++)
  {
    const auto label = mediaValue->labels().at(i);
    const auto value = mediaValue->data().at(i).toReal();
    auto* pieSlice = new QPieSlice(label, value);

    if (!chartColors.isEmpty())
    {
      const auto color = chartColors.at(i);
      pieSlice->setColor(color);
      pieSlice->setBorderColor(color);
    }
    pieSlices.append(pieSlice);
  }
  emit pieChartPopupMediaItemChanged();
  return pieSlices;
}

} // namespace Esri::ArcGISRuntime::Toolkit
