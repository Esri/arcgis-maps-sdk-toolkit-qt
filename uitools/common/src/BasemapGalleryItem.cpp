
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
#ifndef QRT_DISABLE_DEPRECATED_WARNINGS
#define QRT_DISABLE_DEPRECATED_WARNINGS
#endif
#include "BasemapGalleryItem.h"

// Toolkit headers
#include "BasemapGalleryImageProvider.h"
#include "DoOnLoad.h"

// ArcGISRuntime headers
#include <Basemap.h>
#include <Geometry.h>
#include <Item.h>
#include <Polygon.h>

// Qt headers
#include <QIcon>
#include <QFuture>

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
// Qt headers
#include <QQmlContext>
#endif // CPP_ARCGISRUNTIME_TOOLKIT

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {
#ifdef CPP_ARCGISRUNTIME_TOOLKIT
    /*!
      \internal
      \brief Given a BasemapGalleryItem, \a self, registers it with the BasemapGalleryImageProvider
      for thumbnail URLs.
     */
    void registerItem(BasemapGalleryItem* self)
    {
      BasemapGalleryImageProvider::instance()->registerItem(self);
    }

    /*!
      \internal
      \brief Given a BasemapGalleryItem, \a self, unregisters it with the BasemapGalleryImageProvider
      for thumbnail URLs.
     */
    void deregisterItem(BasemapGalleryItem* self)
    {
      BasemapGalleryImageProvider::instance()->deregisterItem(self);
    }
#else
    /*!
      \internal
      \brief No-op. Thumbnail Urls are not required in the Widget's case.
     */
    void registerItem(BasemapGalleryItem* /*self*/)
    {
    }

    /*!
      \internal
      \brief No-op. Thumbnail Urls are not required in the Widget's case.
     */
    void deregisterItem(BasemapGalleryItem* /*self*/)
    {
    }
#endif // CPP_ARCGISRUNTIME_TOOLKIT
  }

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  BasemapGalleryItem::BasemapGalleryItem(QObject* parent) :
    BasemapGalleryItem(nullptr, {}, {}, parent)
  {
  }

  BasemapGalleryItem::BasemapGalleryItem(Basemap* basemap, QObject* parent) :
    BasemapGalleryItem(basemap, {}, {}, parent)
  {
  }

  BasemapGalleryItem::BasemapGalleryItem(Basemap* basemap, QImage thumbnail, QString tooltip, QObject* parent) :
    QObject(parent),
    m_thumbnail{std::move(thumbnail)},
    m_tooltip{std::move(tooltip)},
    m_id{QUuid::createUuid()}
  {
    registerItem(this);
    connect(this, &BasemapGalleryItem::basemapChanged, this, &BasemapGalleryItem::thumbnailChanged);
    connect(this, &BasemapGalleryItem::thumbnailOverrideChanged, this, &BasemapGalleryItem::thumbnailChanged);
    connect(this, &BasemapGalleryItem::basemapChanged, this, &BasemapGalleryItem::tooltipChanged);
    connect(this, &BasemapGalleryItem::tooltipOverrideChanged, this, &BasemapGalleryItem::tooltipChanged);
    setBasemap(basemap);
  }

  BasemapGalleryItem::BasemapGalleryItem(Basemap* basemap, QImage thumbnail, QString tooltip, bool is3D, QObject* parent) :
    QObject(parent),
    m_thumbnail{std::move(thumbnail)},
    m_tooltip{std::move(tooltip)},
    m_id{QUuid::createUuid()},
    m_is3D{is3D}
  {
    registerItem(this);
    connect(this, &BasemapGalleryItem::basemapChanged, this, &BasemapGalleryItem::thumbnailChanged);
    connect(this, &BasemapGalleryItem::thumbnailOverrideChanged, this, &BasemapGalleryItem::thumbnailChanged);
    connect(this, &BasemapGalleryItem::basemapChanged, this, &BasemapGalleryItem::tooltipChanged);
    connect(this, &BasemapGalleryItem::tooltipOverrideChanged, this, &BasemapGalleryItem::tooltipChanged);
    connect(this, &BasemapGalleryItem::is3DChanged, this, &BasemapGalleryItem::is3DChanged);
    setBasemap(basemap);
  }

  BasemapGalleryItem::~BasemapGalleryItem()
  {
    deregisterItem(this);
  }

  Basemap* BasemapGalleryItem::basemap() const
  {
    return m_basemap;
  }

  void BasemapGalleryItem::setBasemap(Basemap* basemap)
  {
    if (basemap == m_basemap)
      return;

    if (m_basemap)
    {
      if (auto item = m_basemap->item())
      {
        disconnect(item, nullptr, this, nullptr);
      }

      disconnect(m_basemap, nullptr, this, nullptr);
    }

    m_basemap = basemap;

    if (m_basemap)
    {
      doOnLoaded(m_basemap.data(), this, [this, basemap]
               {
                 emit basemapChanged();
                 auto item = basemap->item();
                 if (!item)
                 {
                   return;
                 }

                 auto itemThumbnail = item->thumbnail();
                 if (!itemThumbnail.isNull())
                 {
                   // We have a good thumbnail.

                   return;
                 }

                 // fetchThumbnailAsync returns a single future, so don't keep attaching continuations
                 // if it's already running
                 auto future = item->fetchThumbnailAsync();
                 if (!future.isRunning())
                 {
                   future.then(this, [this](const QImage&)
                   {
                     emit basemapChanged();
                   });
                 }
               });
    }
    emit basemapChanged();
  }

  QString BasemapGalleryItem::name() const
  {
    return m_basemap ? m_basemap->name() : QString{};
  }

  QImage BasemapGalleryItem::thumbnail() const
  {
    if (m_thumbnail.isNull() && m_basemap)
    {
      if (auto item = m_basemap->item())
      {
        return item->thumbnail();
      }
      else
      {
        QIcon icon{QStringLiteral(":/Esri/ArcGISRuntime/Toolkit/basemap.svg")};
        return icon.pixmap(200, 133).toImage();
      }
    }
    return m_thumbnail;
  }

  QImage BasemapGalleryItem::thumbnailOverride() const
  {
    return m_thumbnail;
  }

  void BasemapGalleryItem::setThumbnailOverride(QImage thumbnail)
  {
    if (thumbnail == m_thumbnail)
      return;

    m_thumbnail = std::move(thumbnail);
    emit thumbnailOverrideChanged();
  }

  QString BasemapGalleryItem::tooltip() const
  {
    if (m_tooltip.isEmpty() && m_basemap)
    {
      if (auto item = m_basemap->item())
      {
        return item->description();
      }
    }

    return m_tooltip;
  }

  QString BasemapGalleryItem::tooltipOverride() const
  {
    return m_tooltip;
  }

  void BasemapGalleryItem::setTooltipOverride(QString tooltip)
  {
    if (tooltip == m_tooltip)
      return;

    m_tooltip = std::move(tooltip);
    emit tooltipOverrideChanged();
  }

  bool BasemapGalleryItem::is3D() const
  {
    return m_is3D;
  }

  void BasemapGalleryItem::setIs3D(bool is3D)
  {
    m_is3D = is3D;
  }

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
  QUrl BasemapGalleryItem::thumbnailUrl() const
  {
    return QUrl::fromUserInput(QString("image://%1/%2").arg(BasemapGalleryImageProvider::PROVIDER_ID, m_id.toString(QUuid::StringFormat::WithoutBraces)));
  }
#endif // CPP_ARCGISRUNTIME_TOOLKIT

  QUuid BasemapGalleryItem::id() const
  {
    return m_id;
  }

} // Esri::ArcGISRuntime::Toolkit
