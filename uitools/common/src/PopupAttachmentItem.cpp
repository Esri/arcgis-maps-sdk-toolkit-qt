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
#include "PopupAttachmentItem.h"

// Qt headers
#include <QFile>
#include <QFuture>
#include <QStandardPaths>
#include <QtGlobal>

// Maps SDK headers
#include <Attachment.h>
#include <PopupAttachment.h>

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
namespace Esri::ArcGISRuntime::Toolkit {

namespace {

  QString formatFileSize(qint64 bytes) {
    if (bytes < 0) {
        return "Invalid size";
    }
    if (bytes == 0) {
        return "0 B";
    }

    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    auto size = static_cast<double>(bytes);
    int unitIndex = 0;

    while (size >= 1024 && unitIndex < 4)
    {
      size /= 1024;
      unitIndex++;
    }

    return QString::number(size, 'f', 2) + " " + units[unitIndex];
  }

  static QString homePath()
  {
    QString homePath;

  #ifdef Q_OS_IOS
    homePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  #else
    homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  #endif
    return homePath;
  }

}

PopupAttachmentItem::PopupAttachmentItem(PopupAttachment* popupAttachment, QObject *parent)
  : QObject{parent}, m_fetchingAttachment{false}, m_popupAttachment{popupAttachment}
{
}

PopupAttachmentItem::~PopupAttachmentItem() = default;

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
  return m_popupAttachment->attachment()->isDataFetched();
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

void PopupAttachmentItem::downloadAttachment()
{
  m_fetchingAttachment = true;
  emit popupAttachmentItemChanged();
  m_popupAttachment->attachment()->fetchDataAsync().then([this] (const QByteArray& data)
  {
    if (m_tempDir.isValid())
    {
#if defined Q_OS_ANDROID || defined Q_OS_IOS
      m_localData = QUrl::fromLocalFile(homePath() + "/" + m_popupAttachment->attachment()->name());
#else
      m_localData = QUrl::fromLocalFile(m_tempDir.path() + "/" + m_popupAttachment->attachment()->name());
#endif
      QFile file(m_localData.toLocalFile());
      if (file.open(QIODevice::WriteOnly))
      {
        file.write(data);
        file.close();
      }
    }
    m_fetchingAttachment = false;
    emit popupAttachmentItemChanged();
  });
}

} // Esri::ArcGISRuntime::Toolkit
