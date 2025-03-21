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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_IMAGEPOPUPMEDIAITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_IMAGEPOPUPMEDIAITEM_H

// Qt headers
#include <QJsonObject>
#include <QObject>

// Other headers
#include "PopupMediaItem.h"

namespace Esri::ArcGISRuntime::Toolkit {

class PopupViewController;

class ImagePopupMediaItem : public PopupMediaItem
{
  Q_OBJECT
  Q_PROPERTY(QUrl sourceUrl READ sourceUrl NOTIFY imagePopupMediaItemChanged)
  Q_PROPERTY(QUrl linkUrl READ linkUrl NOTIFY imagePopupMediaItemChanged)

public:
  explicit ImagePopupMediaItem(PopupMedia* popupMedia, PopupViewController* popupViewController, QObject* parent = nullptr);
  ~ImagePopupMediaItem() override;

private:
  QUrl linkUrl() const;
  QUrl sourceUrl() const;

signals:
  void imagePopupMediaItemChanged();
  void clickedUrl(const QUrl& url);
  void mediaImageSourceUrl(const QUrl& sourceUrl);
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_IMAGEPOPUPMEDIAITEM_H
