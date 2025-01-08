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
#include "MediaPopupElementViewController.h"

// Qt headers
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// Maps SDK headers
#include <MediaPopupElement.h>
#include <PopupMedia.h>
#include <PopupMediaListModel.h>
#include <PopupTypes.h>

// Toolkit headers
#include <PopupMediaItem.h>
#include <ImagePopupMediaItem.h>
#include <BarChartPopupMediaItem.h>
#include <PieChartPopupMediaItem.h>
#include <LineChartPopupMediaItem.h>

namespace Esri::ArcGISRuntime::Toolkit {

namespace {

  /*!
    \internal
    This class is an internal implementation detail and is subject to change.
   */
  bool isNullOrUndefined(const QJsonValue& value) {
      return value.isNull() || value.isUndefined();
  }

  // Get color information for each popup media. If no color is selected when defining a popup, GraphsTheme.Theme.MixSeries will
  // be applied to the GraphsView.
  // s.a. Web Map Spec: https://developers.arcgis.com/web-map-specification/objects/popupElement_media
  QJsonArray colorsArrayFromJson(const QByteArray& mediaPopupElementJson, const int index)
  {
    if (mediaPopupElementJson.isEmpty())
    {
      qCritical() << QString("MediaPopupElement JSON is empty.");
      return QJsonArray{};
    }

    const QJsonDocument mediaPopupElementJsonDoc = QJsonDocument::fromJson(mediaPopupElementJson);
    if (mediaPopupElementJsonDoc.isNull())
    {
      qCritical() << QString("Error parsing input json");
      return QJsonArray{};
    }

    const auto mediaInfos = mediaPopupElementJsonDoc["mediaInfos"];
    if (isNullOrUndefined(mediaInfos))
    {
      qCritical() << QString("`mediaInfos` array was not found.");
      return QJsonArray{};
    }

    const auto mediaInfo = mediaInfos[index];
    if (isNullOrUndefined(mediaInfo))
    {
      qCritical() << QString("`mediaInfo` at index:%1 was not found in mediaInfos.").arg(index);
      return QJsonArray{};
    }

    const auto value = mediaInfo["value"];
    if (isNullOrUndefined(value))
    {
      qCritical() << QString("`value` object in was not found in mediaInfo.");
      return QJsonArray{};
    }

    const auto colorsArray = value["colors"].toArray();
    if (colorsArray.isEmpty())
    {
      return QJsonArray{};
    }

    return colorsArray;
  }
}

MediaPopupElementViewController::MediaPopupElementViewController(
    MediaPopupElement* mediaPopupElement, QObject* parent)
  : PopupElementViewItem{mediaPopupElement, parent},
    m_popupMediaItems{new GenericListModel(&PopupMediaItem::staticMetaObject, this)}
{
  auto* media = mediaPopupElement->media();
  const auto mediaListModelSize = media->size();

  for (int i = 0; i < mediaListModelSize; i++)
  {
    auto* popupMedia = media->at(i);
    const auto popupMediaType = popupMedia->popupMediaType();
    const auto mediaPopupElementJson = mediaPopupElement->toJson().toUtf8();

    switch (popupMediaType)
    {
      case Esri::ArcGISRuntime::PopupMediaType::Image:
        m_popupMediaItems->append(new ImagePopupMediaItem(
                                    popupMedia,
                                    media)
                                  );
        break;
      case Esri::ArcGISRuntime::PopupMediaType::BarChart:
        m_popupMediaItems->append(new BarChartPopupMediaItem(
                                    popupMedia,
                                    colorsArrayFromJson(mediaPopupElementJson, i),
                                    media)
                                  );
        break;
      case Esri::ArcGISRuntime::PopupMediaType::ColumnChart:
        m_popupMediaItems->append(new BarChartPopupMediaItem(
                                    popupMedia,
                                    colorsArrayFromJson(mediaPopupElementJson, i),
                                    media)
                                  );
        break;
      case Esri::ArcGISRuntime::PopupMediaType::PieChart:
        m_popupMediaItems->append(new PieChartPopupMediaItem(
                                    popupMedia,
                                    colorsArrayFromJson(mediaPopupElementJson, i),
                                    media)
                                  );
        break;
      case Esri::ArcGISRuntime::PopupMediaType::LineChart:
        m_popupMediaItems->append(new LineChartPopupMediaItem(
                                    popupMedia,
                                    colorsArrayFromJson(mediaPopupElementJson, i),
                                    media)
                                  );
        break;
      case Esri::ArcGISRuntime::PopupMediaType::Unknown:
        Q_UNIMPLEMENTED();
        break;
    }
  }
}

MediaPopupElementViewController::~MediaPopupElementViewController() = default;

QString MediaPopupElementViewController::description() const
{
  return static_cast<MediaPopupElement*>(popupElement())->description();
}

QString MediaPopupElementViewController::title() const
{
  return static_cast<MediaPopupElement*>(popupElement())->title();
}

GenericListModel* MediaPopupElementViewController::popupMediaItems() const
{
  return m_popupMediaItems;
}

} // namespace Esri::ArcGISRuntime::Toolkit
