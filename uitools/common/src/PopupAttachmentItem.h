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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPATTACHMENTITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPATTACHMENTITEM_H

// Qt headers
#include <QImage>
#include <QObject>
#include <QTemporaryDir>
#include <QUrl>
#include <QtCore/quuid.h>

// Other headers
#include "QmlEnums.h"

namespace Esri::ArcGISRuntime {

class PopupAttachment;

namespace Toolkit
{

class PopupViewController;

class PopupAttachmentItem : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool dataFetched READ dataFetched NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(bool fetchingAttachment READ fetchingAttachment NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(PopupAttachmentType popupAttachmentType READ popupAttachmentType NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(QString contentType READ contentType NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(QString name READ name NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(QString size READ size NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(QUrl localData READ localData NOTIFY popupAttachmentItemChanged)
  Q_PROPERTY(QUrl thumbnailUrl READ thumbnailUrl NOTIFY popupAttachmentItemChanged)

public:
  explicit PopupAttachmentItem(PopupAttachment* popupAttachment, PopupViewController* popupViewController, QObject* parent = nullptr);
  ~PopupAttachmentItem() override;

  Q_INVOKABLE void downloadAttachment();

private:
  bool fetchingAttachment() const;
  QString contentType() const;
  QString name() const;
  QString size() const;
  QUrl thumbnailUrl() const;

public:
  bool dataFetched() const;
  PopupAttachment* popupAttachment() const;
  PopupAttachmentType popupAttachmentType() const;
  QImage thumbnail() const;
  QUrl localData() const;
  QUuid id() const;
  void setThumbnail(const QImage& thumbnail);

signals:
  void popupAttachmentItemChanged();
  void attachmentDataFetched(const QByteArray& attachmentData, const QString& name);

private:
  bool m_fetchingAttachment{false};
  PopupAttachment* m_popupAttachment = nullptr;
  QImage m_thumbnail;
  QUrl m_localData;
  const QUuid m_id;
};

} // Toolkit
} // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPATTACHMENTITEM_H
