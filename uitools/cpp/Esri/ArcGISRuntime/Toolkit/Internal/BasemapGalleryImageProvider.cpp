/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#include "BasemapGalleryImageProvider.h"

// Toolkit headers
#include "BasemapGalleryItem.h"
#include "Internal/DoOnLoad.h"

// ArcGISRuntime headers
#include <Item.h>

#include <QIcon>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
  namespace {

    /*!
      \internal
      \inmodule EsriArcGISRuntimeToolkit
      \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryImageResponse
      \brief A response object which grabs the thumbnail image from a GalleryItem when it becomes
      available. Think of this like a poor man's future/promise.
     */
    class BasemapGalleryImageResponse : public QQuickImageResponse
    {
    public:
      BasemapGalleryImageResponse(BasemapGalleryItem* galleryItem, QSize requestedSize);
      ~BasemapGalleryImageResponse() override;
      QQuickTextureFactory* textureFactory() const override;

    private:
      QPointer<BasemapGalleryItem> m_galleryItem;
      QSize m_requestedSize;
    };

    /*!
      \internal
      \brief Constructor.
      \list
      \li \a galleryItem The item to grab a thumbnail for.
      \li \a requestedSize The size of image requested.
      \endlist

      Internally, we test the state of the GalleryItem. If there is a thumbnail available we return this immediately,
      otherwise we will attempt to fetch the thumbnail from the associated \c{Item} if and when it is available.
     */
    BasemapGalleryImageResponse::BasemapGalleryImageResponse(BasemapGalleryItem* galleryItem, QSize requestedSize) :
      m_galleryItem(galleryItem),
      m_requestedSize(std::move(requestedSize))
    {
      if (!m_galleryItem)
      {
        emit finished();
        return;
      }

      auto thumbnail = galleryItem->thumbnail();
      if (!thumbnail.isNull())
      {
        // We have a thumbnail to display so just go ahead and display this.
        emit finished();
        return;
      }

      // Otherwise attempt a thumbnail fetch.
      auto basemap = m_galleryItem->basemap();
      if (!basemap)
      {
        emit finished();
        return;
      }

      doOnLoaded(basemap, this, [this, basemap]
               {
                 auto item = basemap->item();
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
                 connect(item, &Item::fetchThumbnailCompleted, this, &BasemapGalleryImageResponse::finished);
                 item->fetchThumbnail();
               });
    }

    /*!
     \internal
     \brief Destructor
     */
    BasemapGalleryImageResponse::~BasemapGalleryImageResponse()
    {
    }

    /*!
      \internal
      \brief Returns the thumbnail as a TextureFactory. If the thumbnail is null, returns a default
      image instead.
     */
    QQuickTextureFactory* BasemapGalleryImageResponse::textureFactory() const
    {
      QImage thumbnail;

      if (m_galleryItem)
        thumbnail = m_galleryItem->thumbnail();

      if (thumbnail.isNull())
      {
        QIcon icon{QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/basemap.svg")};
        thumbnail = icon.pixmap(200, 133).toImage();
      }

      return QQuickTextureFactory::textureFactoryForImage(m_requestedSize.isValid() ? thumbnail.scaled(m_requestedSize) : thumbnail);
    }
  }

  /*!
    \internal
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryImageProvider
    \brief An asynchronous image provider, that when supplied an ID will fetch the appropriate
    basemap thumbnail. See \l QQuickImageProvider for a comprehensive overview.
   */

  /*!
    \internal
    \brief Images URL identifier.
   */
  const QString BasemapGalleryImageProvider::PROVIDER_ID = QStringLiteral("BasemapGalleryItems");

  /*!
    \internal
    \brief Returns the singleton instance of the BasemapGalleryImageProvider.

    Note that it is expected that the instance is passed to the QmlEngine like so:

    \code
    appEngine.addImageProvider(BasemapGalleryImageProvider::PROVIDER_ID, BasemapGalleryImageProvider::instance());
    \endcode

    making the instance owned by and QmlEngine which will destroy this instance when the application
    closes.
   */
  BasemapGalleryImageProvider* BasemapGalleryImageProvider::instance()
  {
    // It is assumed that the instance is passed to QQuickEngine image providers -- which will take
    // ownership and delete this object, otherwise a memory leak will occur.
    static BasemapGalleryImageProvider* self = new BasemapGalleryImageProvider;
    return self;
  }

  /*!
    \internal
    \brief Private constructor of singleton.
   */
  BasemapGalleryImageProvider::BasemapGalleryImageProvider() :
    m_internalObject(new QObject)
  {
  }

  /*!
    \internal
    \brief Destructor.
   */
  BasemapGalleryImageProvider::~BasemapGalleryImageProvider()
  {
    delete m_internalObject;
  }

  /*!
    \internal
    \brief Registers a BasemapGalleryItem with the image provider. This is done
    when the BasemapGalleryItem is constructed.
   */
  bool BasemapGalleryImageProvider::registerItem(BasemapGalleryItem* item)
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

  /*!
    \internal
    \brief Unregisters a BasemapGalleryItem with the image provider. This is done
    when the BasemapGalleryItem is destructed.
   */
  bool BasemapGalleryImageProvider::deregisterItem(BasemapGalleryItem* item)
  {
    if (!item)
      return false;
    QObject::disconnect(item, nullptr, m_internalObject, nullptr);
    return m_itemMap.remove(item->id()) > 0;
  }

  /*!
    \internal
    \brief Called when the QmlEngine requests an image.
   */
  QQuickImageResponse* BasemapGalleryImageProvider::requestImageResponse(const QString& id, const QSize& requestedSize)
  {
    auto itemIt = m_itemMap.find(QUuid::fromString(id));
    return new BasemapGalleryImageResponse(itemIt == std::end(m_itemMap) ? nullptr : itemIt.value(), requestedSize);
  }

#endif // CPP_ARCGISRUNTIME_TOOLKIT

} // namespace Toolkit
} // namespace ArcGISRuntime
} // namespace Esri
