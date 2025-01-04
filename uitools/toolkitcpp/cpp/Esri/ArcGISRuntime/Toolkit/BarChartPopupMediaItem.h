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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BARCHARTPOPUPMEDIAITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BARCHARTPOPUPMEDIAITEM_H

// Qt headers
#include <QJsonArray>
#include <QObject>

// Other headers
#include "PopupMediaItem.h"

class QBarSet;

namespace Esri::ArcGISRuntime::Toolkit {

class BarChartPopupMediaItem : public PopupMediaItem
{
  Q_OBJECT
  Q_PROPERTY(QList<QBarSet*> barSets READ barSets NOTIFY barChartPopupMediaItemChanged)
  Q_PROPERTY(qreal maxValue READ maxValue NOTIFY barChartPopupMediaItemChanged)
  Q_PROPERTY(qreal minValue READ minValue NOTIFY barChartPopupMediaItemChanged)

public:
  explicit BarChartPopupMediaItem(PopupMedia* popupMedia, const QJsonArray& jsonColorArray, QObject* parent = nullptr);
  ~BarChartPopupMediaItem() override = default;

private:
  QList<QBarSet*> barSets();
  qreal maxValue() const;
  qreal minValue() const;

signals:
  void barChartPopupMediaItemChanged();

private:
  QList<QBarSet*> m_barSets;
  QJsonArray m_jsonArr;
  qreal m_maxValue = 0.0;
  qreal m_minValue = 0.0;
};

} // Esri::ArcGISRuntime::Toolkit
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BARCHARTPOPUPMEDIAITEM_H
