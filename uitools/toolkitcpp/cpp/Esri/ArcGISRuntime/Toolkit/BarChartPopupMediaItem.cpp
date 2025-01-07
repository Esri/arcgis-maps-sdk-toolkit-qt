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
  \class Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem
  \inmodule ArcGISRuntimeToolkit
  \brief Represents the contents of a \c{MediaPopupElement::media} to be used for a BarSeries in QML.
  */

/*!
  \brief Constructor
  \list
    \li \a popupMedia - The \l {Esri::ArcGISRuntime::PopupMedia} {PopupMedia} used to populate the view.
    \li \a jsonColorArray - The JSON array containing the color information for each bar set.
    \li \a parent - The optional parent QObject.
  \endlist
 */
BarChartPopupMediaItem::BarChartPopupMediaItem(PopupMedia* popupMedia, const QJsonArray& jsonColorArray, QObject* parent)
  : PopupMediaItem{popupMedia, parent},
    m_jsonColorArr{jsonColorArray}
{
}

/*!
  \brief Returns the list of \c QBarSet objects.
 */
QList<QBarSet*> BarChartPopupMediaItem::barSets()
{
  QList<QBarSet*> barSets;
  auto mediaValue = popupMediaItem()->value();
  const auto popupMediaValueDataLength = mediaValue->data().length();

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

    if (!m_jsonColorArr.isEmpty())
    {
      const auto color = jsonColorExtractor(m_jsonColorArr, i);
      barset->setColor(color);
      barset->setBorderColor(color);
    }
    barSets.append(barset);
  }
  emit barChartPopupMediaItemChanged();
  return barSets;
}

/*!
  \brief Returns the maximum value of all QBarSets.
 */
qreal BarChartPopupMediaItem::maxValue() const
{
  return m_maxValue;
}

/*!
  \brief Returns the minimum value of all QBarSets.
 */
qreal BarChartPopupMediaItem::minValue() const
{
  return m_minValue;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem::barChartPopupMediaItemChanged()
  \brief Signal emitted when the underlying \c PopupMedia changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem::barSets
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem::maxValue
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BarChartPopupMediaItem::minValue
 */
