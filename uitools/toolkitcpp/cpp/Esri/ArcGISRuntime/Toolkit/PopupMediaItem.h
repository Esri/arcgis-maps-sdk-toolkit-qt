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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPMEDIAITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPMEDIAITEM_H

// Qt headers
#include <QObject>

// Other headers
#include "Internal/QmlEnums.h"

namespace Esri::ArcGISRuntime {

class TextPopupElement;
class PopupElement;
class PopupMedia;

namespace Toolkit
{

class PopupMediaItem : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY popupMediaItemChanged)
  Q_PROPERTY(QString caption READ caption NOTIFY popupMediaItemChanged)
  Q_PROPERTY(PopupMediaType popupMediaType READ popupMediaType NOTIFY popupMediaItemChanged)

public:
  PopupMediaItem(QObject* parent = nullptr) = delete;
  explicit PopupMediaItem(PopupMedia* popupMedia, QObject* parent = nullptr);

  QString title() const;
  QString caption() const;
  PopupMediaType popupMediaType() const;
  PopupMedia* popupMediaItem() const;

  static QColor extractColorFromJson(const QJsonArray& colorArray, const int index);

signals:
  void popupMediaItemChanged();

private:
  PopupMedia* m_popupMedia = nullptr;
};

} // Toolkit
} // Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPMEDIAITEM_H
