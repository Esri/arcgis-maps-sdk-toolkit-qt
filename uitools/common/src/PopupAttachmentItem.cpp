
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
#ifndef QRT_DISABLE_DEPRECATED_WARNINGS
#define QRT_DISABLE_DEPRECATED_WARNINGS
#endif
#include "PopupAttachmentItem.h"

// Qt headers
#include <QAbstractFileIconProvider>
#include <QFuture>
#include <QtGlobal>

// Maps SDK headers
#include <Attachment.h>
#include <PopupAttachment.h>

// Toolkit headers
#include "PopupAttachmentImageProvider.h"
#include <PopupViewController.h>

namespace Esri::ArcGISRuntime::Toolkit {

namespace {
  QString formatFileSize(qint64 bytes)
  {
    if (bytes < 0)
    {
        return "Invalid size";
    }
    if (bytes == 0)
    {
        return "0 B";
    }

    const std::array<const char*, 5> units = { "B", "KB", "MB", "GB", "TB" };
    auto size = static_cast<double>(bytes);
    int unitIndex = 0;

    while (size >= 1024 && unitIndex < 4)
    {
      size /= 1024;
      unitIndex++;
    }

    return QString::number(size, 'f', 2) + " " + units[unitIndex];
  }
}

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
PopupAttachmentItem::PopupAttachmentItem(PopupAttachment* popupAttachment, PopupViewController* popupViewController ,QObject *parent)
  : QObject{parent},
    m_fetchingAttachment{false},
    m_popupAttachment{popupAttachment},
    m_id{QUuid::createUuid()}
{
  // connect signal to bubble up attachment data and name to PopupViewController
  connect(this, &PopupAttachmentItem::attachmentDataFetched, popupViewController, &PopupViewController::attachmentDataFetched);

  PopupAttachmentImageProvider::instance()->registerItem(this);
}

PopupAttachmentItem::~PopupAttachmentItem()
{
  PopupAttachmentImageProvider::instance()->deregisterItem(this);
}

QString PopupAttachmentItem::name() const
{
  return m_popupAttachment->name();
}

QString PopupAttachmentItem::contentType() const
{
  return m_popupAttachment->contentType();
}

QString PopupAttachmentItem::size() const
{
  return formatFileSize(m_popupAttachment->size());
}

bool PopupAttachmentItem::dataFetched() const
{
  return m_popupAttachment->attachment()->isDataFetched() && m_popupAttachment->attachment()->attachmentUrl().isValid();
}

bool PopupAttachmentItem::fetchingAttachment() const
{
  return m_fetchingAttachment;
}

PopupAttachmentType PopupAttachmentItem::popupAttachmentType() const
{
  return m_popupAttachment->popupAttachmentType();
}

QUrl PopupAttachmentItem::localData() const
{
  return m_localData;
}

QUrl PopupAttachmentItem::thumbnailUrl() const
{
  QUrl defaultThumbnail;
  switch (popupAttachmentType()) {
    case PopupAttachmentType::Image:
      defaultThumbnail = QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/image.svg");
      break;
    case PopupAttachmentType::Video:
      defaultThumbnail = QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/video.svg");
      break;
    case PopupAttachmentType::Document:
      defaultThumbnail = QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/file.svg");
      break;
    case PopupAttachmentType::Other:
      [[fallthrough]];
    default:
      defaultThumbnail = QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/other.svg");
      break;
  }

  return dataFetched() ? QUrl::fromUserInput(QString("image://%1/%2").arg(PopupAttachmentImageProvider::PROVIDER_ID, m_id.toString(QUuid::StringFormat::WithoutBraces))) : defaultThumbnail;
}

void PopupAttachmentItem::downloadAttachment()
{
  m_fetchingAttachment = true;
  emit popupAttachmentItemChanged();
  m_popupAttachment->attachment()->fetchDataAsync().then(this, [this] (const QByteArray& attachmentData)
  {
    if (attachmentData.isEmpty())
    {
      m_fetchingAttachment = false;
      emit popupAttachmentItemChanged();
      return;
    }

    // emit signal to bubble up attachment data and name to PopupViewController
    emit attachmentDataFetched(attachmentData, name());
    m_localData = m_popupAttachment->attachment()->attachmentUrl();
    m_fetchingAttachment = false;
    // we delay the registration of this until the data has been fetched.
    // Otherwise the creating of the thumbnail/image will do this for us.
    if (m_popupAttachment->popupAttachmentType() == PopupAttachmentType::Image)
    {
      setThumbnail(QImage(m_localData.toLocalFile()));
    }
    else
    {
      setThumbnail(QImage());
    }
    emit popupAttachmentItemChanged();
  });
}

QUuid PopupAttachmentItem::id() const
{
  return m_id;
}

void PopupAttachmentItem::setThumbnail(const QImage& thumbnail)
{
  m_thumbnail = thumbnail;
  emit popupAttachmentItemChanged();
}

PopupAttachment* PopupAttachmentItem::popupAttachment() const
{
  return m_popupAttachment;
}

QImage PopupAttachmentItem::thumbnail() const
{
  return m_thumbnail;
}

} // Esri::ArcGISRuntime::Toolkit
