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
  \class Esri::ArcGISRuntime::Toolkit::PieChartPopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media} to be used for a PieSeries in QML.
  */

/*!
  \brief Constructor
  \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to populate the list of pie slices.
    \li \a jsonColorArray - The JSON array containing the color information for each pie slice.
    \li \a parent - The optional parent QObject.
  \endlist
 */
PieChartPopupMediaItem::PieChartPopupMediaItem(PopupMedia* popupMedia, const QJsonArray& jsonColorArray, QObject* parent)
  : PopupMediaItem{popupMedia, parent},
    m_jsonColorArr{jsonColorArray}
{
}

// When appending QList<QPieSlice*> to the QPieSeries, the ownership is transferred to the QPieSeries. This requires us to recreate the QList<QPieSlice*>
// every time the data is requested because the ListView will recycle the component as needed for performance which will delete the underlying QPieSlices
// originally added to the QList. This also applies to QBarSeries and QBarSet for BarChartPopupMediaItem.
// s.a https://github.com/qt/qtgraphs/blob/08393457537bb45acf3620986039e516f1ac6f2b/src/graphs2d/piechart/qpieseries.cpp#L635-L672

/*!
  \brief Returns the list of \c QPieSlice objects.
 */
QList<QPieSlice*> PieChartPopupMediaItem::pieSlices()
{
  QList<QPieSlice*> pieSlices;
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();

  for (int i = 0; i < popupMediaValueDataLength; i++)
  {
    const auto label = mediaValue->labels().at(i);
    const auto value = mediaValue->data().at(i).toReal();
    auto* pieSlice = new QPieSlice(label, value);

    if (!m_jsonColorArr.isEmpty())
    {
      const auto color = jsonColorExtractor(m_jsonColorArr, i);
      pieSlice->setColor(color);
      pieSlice->setBorderColor(color);
    }
    pieSlices.append(pieSlice);
  }
  emit pieChartPopupMediaItemChanged();
  return pieSlices;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PieChartPopupMediaItem::pieChartPopupMediaItemChanged()
  \brief Signal emitted when the underlying \c PopupMedia changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PieChartPopupMediaItem::pieSlices
 */
