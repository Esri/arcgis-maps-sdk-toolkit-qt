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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_LINECHARTPOPUPMEDIAITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_LINECHARTPOPUPMEDIAITEM_H

// Qt headers
#include <QColor>
#include <QJsonArray>
#include <QObject>
#include <QPointF>
#include <QVariant>

// Other headers
#include "PopupMediaItem.h"

namespace Esri::ArcGISRuntime::Toolkit {

class LineChartPopupMediaItem : public PopupMediaItem
{
  Q_OBJECT
  Q_PROPERTY(QVariantList linePoints READ linePoints NOTIFY lineChartPopupMediaItemChanged)
  Q_PROPERTY(QColor color READ color NOTIFY lineChartPopupMediaItemChanged)
  Q_PROPERTY(qreal maxValue READ maxValue NOTIFY lineChartPopupMediaItemChanged)
  Q_PROPERTY(qreal minValue READ minValue NOTIFY lineChartPopupMediaItemChanged)
  Q_PROPERTY(bool chartColorsEmpty READ chartColorsEmpty NOTIFY lineChartPopupMediaItemChanged)

public:
  explicit LineChartPopupMediaItem(PopupMedia* popupMedia, QObject* parent = nullptr);
  ~LineChartPopupMediaItem() override;

private:
  QVariantList linePoints() const;
  QColor color() const;
  qreal maxValue() const;
  qreal minValue() const;
  bool chartColorsEmpty() const;

signals:
  void lineChartPopupMediaItemChanged();

private:
  QVariantList m_linePoints;
  QColor m_color;
  qreal m_maxValue = 0.0;
  qreal m_minValue = 0.0;
};

} // Esri::ArcGISRuntime::Toolkit
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_LINECHARTPOPUPMEDIAITEM_H
