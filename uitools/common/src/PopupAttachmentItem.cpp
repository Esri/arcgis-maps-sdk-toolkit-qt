
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

#include <QFuture>

#include <PopupAttachment.h>
#include <Attachment.h>

namespace Esri::ArcGISRuntime::Toolkit {

namespace {

  QString formatFileSize(qint64 bytes) {
    const char* units[] = { "B", "KB", "MB", "GB", "TB" };
    double size = static_cast<double>(bytes);
    int unitIndex = 0;

    // Loop to find the appropriate unit
    while (size >= 1024 && unitIndex < 4) {
      size /= 1024;
      unitIndex++;
    }

    // Format the output string
    return QString::number(size, 'f', 2) + " " + units[unitIndex];
  }

}

PopupAttachmentItem::PopupAttachmentItem(PopupAttachment* popupAttachment, QObject *parent)
  : QObject{parent}, m_popupAttachment{popupAttachment}, m_fetchingAttachment{false}
{
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
  return m_popupAttachment->attachment()->isDataFetched();
}

bool PopupAttachmentItem::fetchingAttachment() const
{
  return m_fetchingAttachment;
}

void PopupAttachmentItem::downloadAttachment()
{
  QByteArray imageData;
  m_fetchingAttachment = true;
  emit popupAttachmentItemChanged();
  m_popupAttachment->attachment()->fetchDataAsync().then([&imageData, this] (const QByteArray& data)
  {
    qDebug() << "FetchDataCompleted";
    qDebug() << m_popupAttachment->attachment()->name();
    m_fetchingAttachment = false;
    emit popupAttachmentItemChanged();
  });

  // auto future = attachment1->fetchDataAsync().then(
  // [&imageData](const QByteArray& data)
  // {
  //   imageData = data;
  // });
}

}
