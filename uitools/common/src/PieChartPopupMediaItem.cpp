
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

QColor determineLabelColor(const QColor& sliceColor)
{
  // Extract RGB values from the QColor
  int red = sliceColor.red();
  int green = sliceColor.green();
  int blue = sliceColor.blue();

  // Calculate the relative luminance of the color using
  // Rec. 709, https://en.wikipedia.org/wiki/Rec._709
  double luminance = (0.2126 * red + 0.7152 * green + 0.0722 * blue) / 255;

  // Check if the luminance is closer to black or white
  return (luminance > 0.5) ? QColor(0, 0, 0) : QColor(255, 255, 255);
}

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
PieChartPopupMediaItem::PieChartPopupMediaItem(PopupMedia* popupMedia, QObject* parent)
  : PopupMediaItem{popupMedia, parent}
{
}

PieChartPopupMediaItem::~PieChartPopupMediaItem() = default;

QVariantList PieChartPopupMediaItem::pieSlices()
{
  QVariantList pieSlices;
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();
  const auto chartColors = mediaValue->chartColors();
  const auto colorsHasLessThanLabels = chartColors.size() < popupMediaValueDataLength;

  for (int i = 0; i < popupMediaValueDataLength; i++)
  {
    const auto label = mediaValue->labels().at(i);
    const auto value = mediaValue->data().at(i).toReal();
    auto* pieSlice = new QPieSlice(label, value);

    if (!chartColors.isEmpty() && !colorsHasLessThanLabels)
    {
      const auto color = chartColors.at(i);
      const auto labelColor = determineLabelColor(color);
      pieSlice->setColor(color);
      pieSlice->setBorderColor(color);
      pieSlice->setLabelColor(labelColor);
    }
    pieSlices.append(QVariant::fromValue(pieSlice));
  }
  emit pieChartPopupMediaItemChanged();
  return pieSlices;
}

} // namespace Esri::ArcGISRuntime::Toolkit
