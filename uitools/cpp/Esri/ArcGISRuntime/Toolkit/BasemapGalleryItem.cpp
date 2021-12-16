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
#include "BasemapGalleryController.h"

// Toolkit headers
#include "Internal/GeoViews.h"
#include "Internal/BasemapGalleryImageProvider.h"

// ArcGISRuntime headers
#include <Basemap.h>

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
// Qt headers
#include <QQmlContext>
#endif // CPP_ARCGISRUNTIME_TOOLKIT

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

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
  \inmodule EsriArcGISRuntimeToolkit
  \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem
  \brief Represents the contents of a BasemapGallery. Can be accessed via
  \c{BasemapGalleryController::gallery}.

  The BasemapGalleryItem is a wrapper around a Basemap, along with any additional
  meta-information.

  The BasemapGalleryItem allows for the optional application of a thumbnail and tooltip,
  which override the given thumbnail/tooltip of the Basemap.

  This is handy when creating Basemaps on the fly from a BasemapStyle or other means, as
  these basemaps do not come with a thumbnail or tooltip by default.
  */

/*!
  \brief Constructor. Takes a \a parent object.
  Constructs an empty item.
 */
BasemapGalleryItem::BasemapGalleryItem(QObject* parent) :
  BasemapGalleryItem(nullptr, {}, {}, parent)
{
}

/*!
  \brief Constructor. Takes a \a basemap and \a parent object.
  Constructs an item that represents the given basemap in the gallery.
 */
BasemapGalleryItem::BasemapGalleryItem(Basemap* basemap, QObject* parent) :
  BasemapGalleryItem(basemap, {}, {}, parent)
{
}

/*!
  \brief Constructor. Takes a \a basemap, \a thumbnail, \a tooltip, and \a parent object.

  Constructs an item that represents the given basemap in the gallery.
  The thumbnail and tooltip objects provided are used in place of the thumbnail and description
  of the basemap's item, if provided.
 */
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

/*!
  \brief Destructor.
 */
BasemapGalleryItem::~BasemapGalleryItem()
{
  deregisterItem(this);
}

/*!
  \brief Returns the basemap this gallery item represents.
 */
Basemap* BasemapGalleryItem::basemap() const
{
  return m_basemap;
}

/*!
  \brief Sets the \a basemap this gallery item represents.
 */
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
    auto doFetch = [this, basemap]
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
      connect(item, &Item::fetchThumbnailCompleted, this, &BasemapGalleryItem::basemapChanged);
      item->fetchThumbnail();
    };

    if (m_basemap->loadStatus() != LoadStatus::Loaded)
    {
      connect(m_basemap, &Basemap::doneLoading, this, doFetch);
      m_basemap->load();
    }
    else
    {
      doFetch();
    }
  }
  emit basemapChanged();
}


/*!
  \brief Returns the name of the basemap.
 */
QString BasemapGalleryItem::name() const
{
  return m_basemap ? m_basemap->name() : QString{};
}

/*!
  \brief Returns the thumbnail override of this item if valid. Otherwise
  returns the thumbnail on the basemap's item, if available. Otherwise, returns
  the default thumbnail.
  \sa BasemapGalleryItem::thumbnailOverride
 */
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
      return QImage(QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/generic_basemap_thumbnail.png"));
    }
  }
  return m_thumbnail;
}

/*!
  \brief Returns the thumbnail of this item which overrides
  the basemap's items' thumbnail in the gallery.
  \sa BasemapGalleryItem::thumbnail
 */
QImage BasemapGalleryItem::thumbnailOverride() const
{
  return m_thumbnail;
}

/*!
  \brief Sets the \a thumbnail override of this item, which replaces the
  thumbnail of the basemap when rendering the gallery.
 */
void BasemapGalleryItem::setThumbnailOverride(QImage thumbnail)
{
  if (thumbnail == m_thumbnail)
    return;

  m_thumbnail = std::move(thumbnail);
  emit thumbnailOverrideChanged();
}

/*!
  \brief Returns the tooltip override of this item if non-empty. Otherwise
  returns the description on the basemap's item, if available.
  \sa BasemapGalleryItem::tooltipOverride
 */
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

/*!
  \brief Returns the tooltip of this item which overrides
  the basemap's items' description in the gallery.
  \sa BasemapGalleryItem::tooltip
 */
QString BasemapGalleryItem::tooltipOverride() const
{
  return m_tooltip;
}

/*!
  \brief Sets the \a tooltip override of this item, which replaces the
  tooltip of the basemap when rendering the gallery.
 */
void BasemapGalleryItem::setTooltipOverride(QString tooltip)
{
  if (tooltip == m_tooltip)
    return;

  m_tooltip = std::move(tooltip);
  emit tooltipOverrideChanged();
}

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
/*!
  \internal
  \brief Returns a URL that points to the thumbnail that can be consumed by QML. Only needed in the C++/QML case.
 */
QUrl BasemapGalleryItem::thumbnailUrl() const
{
  return QUrl::fromUserInput(QString("image://%1/%2").arg(BasemapGalleryImageProvider::PROVIDER_ID, m_id.toString(QUuid::StringFormat::WithoutBraces)));
}
#endif // CPP_ARCGISRUNTIME_TOOLKIT

/*!
  \internal
  \brief Returns an ID that identifies the uniqueness of this item.
 */
QUuid BasemapGalleryItem::id() const
{
  return m_id;
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::basemapChanged()
  \brief Emitted when the basemap has changed.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltipChanged()
  \brief Emitted when the toolip has changed.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltipOverrideChanged()
  \brief Emitted when the toolip override has changed.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailChanged()
  \brief Emitted when the thumbnail has changed.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailOverrideChanged()
  \brief Emitted when the thumbnail override has changed.
*/

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::basemap
  \brief The basemap this item represents.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::basemap()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::name
  \brief The name of the basemap, if set.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::name()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltip
  \brief The tooltip the item will display in the gallery.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltip()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltipOverride
  \brief A tooltip the developer can set to replace the default tooltip.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::tooltipOverride()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnail
  \brief The thumbnail the item will display in the gallery.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnail()
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailOverride
  \brief A thumbnail the developer can set to replace the default thumbnail.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailOverride()
 */

/*!
  \internal
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailUrl
  \brief A thumbnail URL for QML rendering of images.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryItem::thumbnailUrl()
*/

} // Toolkit
} // ArcGISRuntime
} // Esri
