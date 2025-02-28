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
#include "PopupAttachmentImageProvider.h"

// Qt headers
#include <QFileSystemModel>
#include <QFuture>
#include <QtGlobal>
#include <QIcon>
#include <QMimeDatabase>

#include <QStandardPaths>

// Maps SDK headers
#include <Item.h>
#include <PopupAttachment.h>

// Toolkit headers
#include "PopupAttachmentItem.h"

namespace Esri::ArcGISRuntime::Toolkit {

namespace {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
class PopupAttachmentImageResponse : public QQuickImageResponse
{
public:
  PopupAttachmentImageResponse(PopupAttachmentItem* popupAttachmentItem, QSize requestedSize);
  ~PopupAttachmentImageResponse() override;
  QQuickTextureFactory* textureFactory() const override;

private:
  QPointer<PopupAttachmentItem> m_popupAttachmentItem;
  QSize m_requestedSize;
};

PopupAttachmentImageResponse::PopupAttachmentImageResponse(PopupAttachmentItem* popupAttachmentItem, QSize requestedSize) :
  m_popupAttachmentItem(popupAttachmentItem),
  m_requestedSize(std::move(requestedSize))
{
  if (!m_popupAttachmentItem)
  {
    emit finished();
    return;
  }

  auto thumbnail = popupAttachmentItem->thumbnail();
  if (!thumbnail.isNull())
  {
    // We have a thumbnail to display so just go ahead and display this.
    emit finished();
    return;
  }

  auto item = m_popupAttachmentItem;
  if (!item)
  {
    emit finished();
    return;
  }

  auto itemThumbnail = item->thumbnail();
  if (!itemThumbnail.isNull())
  {
    // We have a good thumbnail.
    emit finished();
    return;
  }

  auto popupAttachment = item->popupAttachment();
  if (popupAttachment->popupAttachmentType() != PopupAttachmentType::Image)
  {
    emit finished();
    return;
  }
}

PopupAttachmentImageResponse::~PopupAttachmentImageResponse() = default;

QQuickTextureFactory* PopupAttachmentImageResponse::textureFactory() const
{
  QImage thumbnail;
  QIcon icon;

  if (m_popupAttachmentItem)
  {
    thumbnail = m_popupAttachmentItem->thumbnail();
  }

  // Handles edge case for Mobile since QAbstractFileIconProvider returns a Null QIcon.
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
  if (thumbnail.isNull() || m_popupAttachmentItem->popupAttachmentType() != PopupAttachmentType::Image)
  {
    switch (m_popupAttachmentItem->popupAttachmentType()) {
      case PopupAttachmentType::Image:
        icon = QIcon(QStringLiteral(":/esri.com/imports/Calcite/images/image.svg"));
        break;
      case PopupAttachmentType::Video:
        icon = QIcon(QStringLiteral(":/esri.com/imports/Calcite/images/video.svg"));
        break;
      case PopupAttachmentType::Document:
        icon = QIcon(QStringLiteral(":/esri.com/imports/Calcite/images/file.svg"));
        break;
      case PopupAttachmentType::Other:
        [[fallthrough]];
      default:
        icon = QIcon(QStringLiteral(":/esri.com/imports/Calcite/images/other.svg"));
        break;
    }

    auto pixmap = icon.pixmap(32, 32);
    thumbnail = pixmap.toImage();
    return QQuickTextureFactory::textureFactoryForImage(thumbnail);
  }
#endif

  if (thumbnail.isNull() && m_popupAttachmentItem->dataFetched())
  {
    QFileSystemModel fileSystemModel;
    auto index = fileSystemModel.index(m_popupAttachmentItem->localData().toLocalFile());
    icon = fileSystemModel.fileIcon(index);

    auto pixmap = icon.pixmap(32, 32);
    thumbnail = pixmap.toImage();
  }

  return QQuickTextureFactory::textureFactoryForImage(m_requestedSize.isValid() ? thumbnail.scaled(m_requestedSize) : thumbnail);
}
}

const QString PopupAttachmentImageProvider::PROVIDER_ID = QStringLiteral("PopupAttachmentItems");

PopupAttachmentImageProvider* PopupAttachmentImageProvider::instance()
{
  // It is assumed that the instance is passed to QQuickEngine image providers -- which will take
  // ownership and delete this object, otherwise a memory leak will occur.
  static PopupAttachmentImageProvider* self = new PopupAttachmentImageProvider;
  return self;
}

PopupAttachmentImageProvider::PopupAttachmentImageProvider() :
  m_internalObject(new QObject)
{
}

PopupAttachmentImageProvider::~PopupAttachmentImageProvider()
{
  delete m_internalObject;
}

bool PopupAttachmentImageProvider::registerItem(PopupAttachmentItem* item)
{
  if (!item)
    return false;

  const auto key = item->id();

  if (m_itemMap.contains(key))
    return false;

  m_itemMap.insert(key, item);
  QObject::connect(item, &QObject::destroyed, m_internalObject, [this, key]
  {
    m_itemMap.remove(key);
  });
  return true;
}

bool PopupAttachmentImageProvider::deregisterItem(PopupAttachmentItem* item)
{
  if (!item)
    return false;
  QObject::disconnect(item, nullptr, m_internalObject, nullptr);
  return m_itemMap.remove(item->id()) > 0;
}

QQuickImageResponse* PopupAttachmentImageProvider::requestImageResponse(const QString& id, const QSize& requestedSize)
{
  auto itemIt = m_itemMap.find(QUuid::fromString(id));
  return new PopupAttachmentImageResponse(itemIt == std::end(m_itemMap) ? nullptr : itemIt.value(), requestedSize);
}

} // Esri::ArcGISRuntime::Toolkit
