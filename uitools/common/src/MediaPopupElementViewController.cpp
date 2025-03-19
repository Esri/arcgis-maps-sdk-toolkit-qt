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

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
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

    switch (popupMediaType)
    {
      case Esri::ArcGISRuntime::PopupMediaType::Image:
      {
        const auto imageMedia = new ImagePopupMediaItem(popupMedia, media);
        // bubble up signal to PopupViewController
        // bubble up linkUrl signal to PopupViewController. This is the linkUrl to be used for opening in an external browser
        connect(imageMedia, &ImagePopupMediaItem::clickedUrl, this, &MediaPopupElementViewController::clickedUrl);

        // bubble up sourceUrl signal to PopupViewController. This is the sourceUrl used to display the image inside the PopupView
        connect(imageMedia, &ImagePopupMediaItem::mediaImageSourceUrl, this, &MediaPopupElementViewController::mediaImageSourceUrl);

        m_popupMediaItems->append(imageMedia);
        break;
      }
      case Esri::ArcGISRuntime::PopupMediaType::BarChart:
      {
        m_popupMediaItems->append(new BarChartPopupMediaItem(
                                    popupMedia,
                                    media)
                                  );
        break;
      }
      case Esri::ArcGISRuntime::PopupMediaType::ColumnChart:
      {
        m_popupMediaItems->append(new BarChartPopupMediaItem(
                                    popupMedia,
                                    media)
                                  );
        break;
      }
      case Esri::ArcGISRuntime::PopupMediaType::PieChart:
      {
        m_popupMediaItems->append(new PieChartPopupMediaItem(
                                    popupMedia,
                                    media)
                                  );
        break;
      }
      case Esri::ArcGISRuntime::PopupMediaType::LineChart:
      {
        m_popupMediaItems->append(new LineChartPopupMediaItem(
                                    popupMedia,
                                    media)
                                  );
        break;
      }
      case Esri::ArcGISRuntime::PopupMediaType::Unknown:
      {
        Q_UNIMPLEMENTED();
        break;
      }
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
  auto title = static_cast<MediaPopupElement*>(popupElement())->title();
  return !title.isEmpty() ? title : QStringLiteral("Media");
}

GenericListModel* MediaPopupElementViewController::popupMediaItems() const
{
  return m_popupMediaItems;
}

} // namespace Esri::ArcGISRuntime::Toolkit
