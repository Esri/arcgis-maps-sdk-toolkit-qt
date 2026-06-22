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
#include "BasemapGalleryController.h"

// Toolkit headers
#include "DoOnLoad.h"
#include "GeoViews.h"
#include "SingleShotConnection.h"

// ArcGISRuntime headers
#include <Basemap.h>
#include <BasemapListModel.h>
#include <Error.h>
#include <Item.h>
#include <Layer.h>
#include <LayerListModel.h>
#include <Loadable.h>
#include <Map.h>
#include <Scene.h>
#include <SceneViewTypes.h>
#include <SpatialReference.h>

// Qt headers
#include <QFuture>
#include <QPersistentModelIndex>
#include <QPointer>
#include <QPromise>

// C++ headers
#include <memory>

namespace Esri::ArcGISRuntime::Toolkit
{

  namespace
  {
    /*!
      \internal
     */
    template<typename T>
    auto* qPointerFrom(T* t)
    {
      return QPointer<T>{t};
    }

    QList<BasemapGalleryItem*> galleryItems(GenericListModel* gallery)
    {
      QList<BasemapGalleryItem*> items;
      if (!gallery)
      {
        return items;
      }

      items.reserve(gallery->rowCount());
      for (int i = 0; i < gallery->rowCount(); ++i)
      {
        if (auto* item = gallery->element<BasemapGalleryItem>(gallery->index(i)))
        {
          items.push_back(item);
        }
      }
      return items;
    }

    void splitGalleryItemsByDimension(const QList<BasemapGalleryItem*>& items,
                                      QList<BasemapGalleryItem*>& items3D,
                                      QList<BasemapGalleryItem*>& items2D)
    {
      items3D.clear();
      items2D.clear();
      items3D.reserve(items.size());
      items2D.reserve(items.size());

      for (auto* item : items)
        {
          if (!item)
          {
            continue;
          }

          if (item->is3D())
          {
            items3D.push_back(item);
          }
          else
          {
            items2D.push_back(item);
          }
        }
      }

    /*!
      \internal
      Takes a map or scene, and connects to it and its basemap.
      Emits a basemapChanged signal when:
      - The map/scene basemapChanged signal fires.
      - The basemap load status has changed.

      We automatically disconnect from the map/scene's old basemap if the
      map/scene basemapChanged signal is fired.
     */
    template<typename T>
    void connectToBasemap(BasemapGalleryController* self, T* geoModel)
    {
      static_assert(std::is_base_of<GeoModel, T>::value, "Must be a GeoModel.");

      if (!geoModel)
      {
        return;
      }

      const auto listenToLoadSignals = [self](Basemap* basemap)
      {
        if (basemap)
        {
          if (basemap->loadStatus() != LoadStatus::Loaded)
          {
            QObject::connect(basemap, &Basemap::doneLoading, self, &BasemapGalleryController::currentBasemapChanged);
          }
        }
      };

      // If basemap changes on map or scene, disconnect from basemap and
      // signal that basemap has changed.
      QObject::connect(geoModel, &T::basemapChanged, self, [self, listenToLoadSignals, geoModel](Basemap* oldBasemap)
      {
        QObject::disconnect(self, nullptr, oldBasemap, nullptr);
        auto* newBasemap = geoModel->basemap();
        listenToLoadSignals(newBasemap); // Connect to new basemap.
        self->setCurrentBasemap(newBasemap);
      });

      listenToLoadSignals(geoModel->basemap());
    }

    /*!
      \internal
      Connect to [Scene/Map].

      1. Update our cached basemap with the Scene/Map basemap.
      2. Discover the runtime type of GeoModel
      3. Connect to that type's basemapChanged signal.
     */
    void connectToGeoModel(BasemapGalleryController* self, GeoModel* geoModel)
    {
      doOnLoaded(geoModel, self, [self, geoModel]
      {
        self->setCurrentBasemap(geoModel->basemap());
      });

      // TODO: Cleanup this when GeoModel itself exposes the
      // basemapChanged signal.
      if (auto* scene = qobject_cast<Scene*>(geoModel))
      {
        connectToBasemap(self, scene);
      }
      else if (auto* map = qobject_cast<Map*>(geoModel))
      {
        connectToBasemap(self, map);
      }
    }

    /*!
      \internal
      1. Disconnect from the associated Map/Scene.
      2. Disconnect from the associated Basemap.
     */
    void disconnectFromGeoModel(BasemapGalleryController* self, GeoModel* geoModel)
    {
      QObject::disconnect(geoModel, nullptr, self, nullptr);
      if (Basemap* basemap = geoModel->basemap())
      {
        QObject::disconnect(basemap, nullptr, self, nullptr);
      }
    }

    /*!
      \internal
      Triggered when a basemap is added to the gallery.

      1. We listen for GalleryItem changes.
      2. We force the basemap to load if not already.
      3. We emit BasemapGalleryController::currentBasemapChanged if the current basemap was
      added to the gallery.
     */
    void onBasemapAddedToGallery(BasemapGalleryController* self, GenericListModel* gallery, const QModelIndex& index, BasemapGalleryItem* galleryItem)
    {
      if (!galleryItem)
      {
        return;
      }

      const auto pIndex = QPersistentModelIndex(index);
      const auto notifyChange = [pIndex, gallery]
      {
        // Notify that the item has changed.
        if (pIndex.isValid())
        {
          emit gallery->dataChanged(pIndex, pIndex);
        }
      };

      QObject::connect(galleryItem, &BasemapGalleryItem::basemapChanged, self, notifyChange);
      QObject::connect(galleryItem, &BasemapGalleryItem::thumbnailChanged, self, notifyChange);
      QObject::connect(galleryItem, &BasemapGalleryItem::tooltipChanged, self, notifyChange);

      auto* basemap = galleryItem->basemap();

      if (basemap && basemap->loadStatus() != LoadStatus::Loaded)
      {
        basemap->load();
      }

      if (self->currentBasemap() == basemap)
      {
        // If the currently active basemap was added to the gallery, we notify
        // downstream consumers that the currently active basemap has changed also to
        // trigger UI updates
        emit self->currentBasemapChanged();
      }
    }

    /*!
      \internal
      Triggered when a basemap is removed from the gallery.

      1. We disconnect from the GalleryItem.
      2. We emit BasemapGalleryController::currentBasemapChanged if the current basemap was
      removed from the gallery.
      3. We delete the GalleryItem if we are the parent.
     */
    void onBasemapRemovedFromGallery(BasemapGalleryController* self, BasemapGalleryItem* galleryItem)
    {
      if (!galleryItem)
      {
        return;
      }

      QObject::disconnect(galleryItem, nullptr, self, nullptr);

      if (self->currentBasemap() == galleryItem->basemap())
      {
        // If the currently active basemap was added to the gallery, we let
        // downstream consumers the currently active basemap has changed also to
        // trigger UI updates.
        emit self->currentBasemapChanged();
      }

      if (galleryItem->parent() == self)
      {
        galleryItem->deleteLater();
      }
    }

    /*!
      \internal
      Refreshes basemaps shown in the gallery from the loaded portal. Removes any gallery items that are no longer in the portal basemap lists
      and preserves gallery items that are still in the portal basemap lists to preserve thumbnails and tooltips set by the developer.
    */
    void refreshGalleryBasemaps(BasemapGalleryController* self, bool useDeveloperBasemaps = false)
    {
      if (!self)
      {
        return;
      }

      auto* gallery = self->gallery();
      auto* portal = self->portal();
      if (!gallery || !portal || portal->loadStatus() != LoadStatus::Loaded)
      {
        return;
      }
      const auto currentItems = galleryItems(gallery);
      QList<BasemapGalleryItem*> current3DItems;
      QList<BasemapGalleryItem*> current2DItems;
      splitGalleryItemsByDimension(currentItems, current3DItems, current2DItems);

      // Remove gallery items that are no longer in the portal basemap lists, and keep gallery items that are
      // still in the portal basemap lists to preserve thumbnails and tooltips set by the developer.
      auto preserveAndSortGalleryItems = [self](const QList<BasemapGalleryItem*>& currentItems, BasemapListModel* portalBasemaps,
                                                bool is3D = false) -> QList<BasemapGalleryItem*>
      {
        if (!portalBasemaps)
        {
          return {};
        }
        QList<BasemapGalleryItem*> preservedItems;
        preservedItems.reserve(currentItems.size());

        for (int i = 0; i < portalBasemaps->size(); ++i)
        {
          auto* portalBasemap = portalBasemaps->at(i);
          {
            BasemapGalleryItem* existingItem = nullptr;
            for (auto* currentItem : currentItems)
              {
              if (currentItem && currentItem->basemap() == portalBasemap)
                {
                  existingItem = currentItem;
                  break;
                }
              }

            if (existingItem)
            {
              preservedItems.push_back(existingItem);
            }
            else
            {
              preservedItems.push_back(new BasemapGalleryItem(portalBasemap, {}, {}, is3D, self));
            }
          }
        }

        // Now sort the items
        std::sort(preservedItems.begin(), preservedItems.end(), [](BasemapGalleryItem* left, BasemapGalleryItem* right)
        {
          if (!left || !left->basemap() || !left->basemap()->item() || left->basemap()->item()->title().isEmpty())
          {
            return false;
          }
          if (!right || !right->basemap() || !right->basemap()->item() || right->basemap()->item()->title().isEmpty())
          {
            return true;
          }
          return left->basemap()->item()->title() < right->basemap()->item()->title();
        });

        return preservedItems;
      };

      QList<BasemapGalleryItem*> desiredItems;

      if (qobject_cast<Scene*>(self->geoModel()))
      {
        desiredItems = preserveAndSortGalleryItems(current3DItems, portal->basemaps3D(), true);
      }

      if (useDeveloperBasemaps)
      {
        desiredItems.append(preserveAndSortGalleryItems(current2DItems, portal->developerBasemaps()));
      }
      else
      {
        desiredItems.append(preserveAndSortGalleryItems(current2DItems, portal->basemaps()));
      }

      // If the current items in the gallery are in the same order and have the same classification (2D vs 3D) as the desired items,
      // then we can skip refreshing the gallery to avoid unnecessary UI updates that can cause flickering.
      auto changed = currentItems.size() != desiredItems.size();
      if (!changed)
      {
        for (int i = 0; i < currentItems.size(); ++i)
        {
          if (!currentItems.at(i) || !desiredItems.at(i) || currentItems.at(i)->basemap() != desiredItems.at(i)->basemap() ||
              currentItems.at(i)->is3D() != desiredItems.at(i)->is3D())
          {
            changed = true;
            break;
          }
        }
      }

      if (!changed)
      {
        return;
      }

      gallery->removeRows(0, gallery->rowCount());
      for (auto* item : desiredItems)
      {
        gallery->append(item);
      }

      emit self->basemapsChanged();
    }

    /*!
      \internal
      Fetches 2D and 3D basemaps from the current portal. Returns a boolean future that indicates when the fetch is complete and successful. 
     */
    QFuture<bool> fetchPortalBasemaps(BasemapGalleryController* self, Portal* portal, bool useDeveloperBasemaps)
    {
      auto promise = std::make_shared<QPromise<bool>>();
      promise->start();
      auto readyFuture = promise->future();

      auto complete = [promise](bool ok)
      {
        promise->addResult(ok);
        promise->finish();
      };

      if (!self || !portal)
      {
        complete(false);
        return readyFuture;
      }


      auto fetchBasemapsFromLoadedPortal = [portal, useDeveloperBasemaps, complete]()
      {
        QList<QFuture<void>> basemapFutures;
        if (useDeveloperBasemaps)
        {
        if (portal->developerBasemaps()->rowCount() == 0)
        {
          basemapFutures.append(portal->fetchDeveloperBasemapsAsync());
        }
        }
        else if (portal->basemaps()->rowCount() == 0)
        {
          basemapFutures.append(portal->fetchBasemapsAsync());
        }

        if (portal->basemaps3D()->rowCount() == 0)
        {
          basemapFutures.append(portal->fetch3DBasemapsAsync());
        }

        if (basemapFutures.isEmpty())
        {
          complete(true);
        }
        else if (basemapFutures.size() == 1)
        {
          basemapFutures.first().then([complete](const auto&)
          {
            complete(true);
          });
        }
        else
        {
          using FuturesVariant = std::variant<QFuture<void>, QFuture<void>>;
          QtFuture::whenAll(basemapFutures.first(), basemapFutures.last())
            .then([complete](const QList<FuturesVariant>&)
          {
            complete(true);
          });
        }
      };

      if (portal->loadStatus() == LoadStatus::Loaded)
      {
        fetchBasemapsFromLoadedPortal();
      }
      else
      {
        singleShotConnection(portal, &Portal::doneLoading, self, [fetchBasemapsFromLoadedPortal, complete](const Error& loadError)
        {
          if (!loadError.isEmpty())
          {
            qWarning() << "Failed to load portal with error:" << loadError.message() << loadError.additionalMessage();
            complete(false);
            return;
          }
          fetchBasemapsFromLoadedPortal();
        });

        portal->load();
      }

      return readyFuture;
    }
  } // namespace

  BasemapGalleryController::BasemapGalleryController(QObject* parent) :
    QObject(parent),
    m_portal(new Portal(QUrl("https://arcgis.com"), this)),
    m_gallery(new GenericListModel(&BasemapGalleryItem::staticMetaObject, this))
  {
    // Listen in to items added to the gallery.
    connect(m_gallery, &GenericListModel::rowsInserted, this, [this](const QModelIndex& parent, int first, int last)
    {
      if (parent.isValid())
      {
        return;
      }

      for (auto i = first; i <= last; ++i)
      {
        auto index = m_gallery->index(i);
        if (auto* galleryItem = m_gallery->element<BasemapGalleryItem>(index))
        {
          onBasemapAddedToGallery(this, m_gallery, index, galleryItem);
        }
      }
    });

    // Listen in to items removed from the gallery.
    connect(m_gallery, &GenericListModel::rowsRemoved, this, [this](const QModelIndex& parent, int first, int last)
    {
      if (parent.isValid())
      {
        return;
      }

      for (auto i = first; i <= last; ++i)
      {
        auto index = m_gallery->index(i);
        if (auto* galleryItem = m_gallery->element<BasemapGalleryItem>(index))
        {
          onBasemapRemovedFromGallery(this, galleryItem);
        }
      }
    });
    m_gallery->setFlagsCallback([this](const QModelIndex& index)
    {
      BasemapGalleryItem* galleryItem = m_gallery->element<BasemapGalleryItem>(index);
      if (!basemapMatchesCurrentSpatialReference(galleryItem->basemap()))
      {
        //disabled item flags
        return Qt::ItemFlags(Qt::NoItemFlags);
      }
      else
      {
        //enabled and selectable item flags
        return Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      }
    });
    fetchPortalBasemaps(this, m_portal, /*useDeveloperBasemaps=*/true)
      .then(this, [this](bool ready)
    {
      if (ready)
      {
        refreshGalleryBasemaps(this, true);
      }
    });
    // Have to set the property names, so the controller will know how to match the properties from
    // BasemapGalleryItem with the specific Qt::<namespace> invoked in the .data() from the View (ListView) obj
    m_gallery->setDisplayPropertyName("name");
    m_gallery->setDecorationPropertyName("thumbnail");
    m_gallery->setTooltipPropertyName("tooltip");
  }

  BasemapGalleryController::~BasemapGalleryController()
  {
  }

  GeoModel* BasemapGalleryController::geoModel() const
  {
    return m_geoModel;
  }

  void BasemapGalleryController::setGeoModel(GeoModel* geoModel)
  {
    if (geoModel == m_geoModel)
    {
      return;
    }

    if (m_geoModel)
    {
      disconnectFromGeoModel(this, m_geoModel);
    }

    m_geoModel = geoModel;

    if (m_geoModel)
    {
      connectToGeoModel(this, m_geoModel);
      // guard from nullptr direct access
      setCurrentBasemap(geoModel->basemap());
    }

    emit geoModelChanged();

    refreshGalleryBasemaps(this);
    //forcing all the items in the gallery to recalculate the ::ItemFlags for the view.
    emit m_gallery->dataChanged(m_gallery->index(0), m_gallery->index(std::max(m_gallery->rowCount() - 1, 0)));
  }

  GenericListModel* BasemapGalleryController::gallery() const
  {
    return m_gallery;
  }

  Portal* BasemapGalleryController::portal() const
  {
    return m_portal;
  }

  void BasemapGalleryController::setPortal(Portal* portal)
  {
    if (portal == m_portal)
    {
      return;
    }

    if (m_portal)
    {
      disconnect(m_portal, nullptr, this, nullptr);
      m_gallery->removeRows(0, m_gallery->rowCount());
      if (m_portal->parent() == this)
      {
        // If we own the Portal we can delete it when
        // we no longer require it.
        m_portal->deleteLater();
      }
      emit currentBasemapChanged(); // Resets the current basemap index.
    }

    m_portal = portal;

    if (m_portal)
    {
      fetchPortalBasemaps(this, m_portal, false)
        .then(this, [this](bool ready)
      {
        if (ready)
        {
          refreshGalleryBasemaps(this, false);
        }
      });
    }

    emit portalChanged();
  }

  Basemap* BasemapGalleryController::currentBasemap() const
  {
    return m_currentBasemap;
  }

  void BasemapGalleryController::setCurrentBasemap(Basemap* basemap)
  {
    const auto apply = [basemap, this](const Error& e)
    {
      if (e.isEmpty())
      {
        if (basemap == m_currentBasemap)
        {
          return;
        }
        if (!basemapMatchesCurrentSpatialReference(basemap))
        {
          // force redraw for the single basemapGalleryItem updated
          emit m_gallery->dataChanged(m_gallery->index(basemapIndex(basemap)), m_gallery->index(basemapIndex(basemap)));
          return;
        }
        m_currentBasemap = basemap;
        emit currentBasemapChanged();

        if (m_geoModel && m_geoModel->basemap() != m_currentBasemap)
        {
          m_geoModel->setBasemap(m_currentBasemap);
        }
      }
      else
      {
        qWarning() << "problem in loading the layer";
      }
    };
    if (basemap->baseLayers()->size() > 0)
    {
      if (basemap->baseLayers()->first()->loadStatus() != LoadStatus::Loaded)
      {
        singleShotConnection(basemap->baseLayers()->first(), &Layer::doneLoading, this, apply);
        basemap->baseLayers()->first()->load();
      }
      else
      {
        apply(Error{});
      }
    }
  }

  bool BasemapGalleryController::append(Basemap* basemap, bool is3D /* = false */)
  {
    std::lock_guard<std::mutex> lock(m_galleryAccessMutex);
    return m_gallery->append(new BasemapGalleryItem(basemap, {}, {}, is3D, this));
  }

  bool BasemapGalleryController::append(Basemap* basemap, QImage thumbnail, QString tooltip /* = {} */, bool is3D /* = false */)
  {
    std::lock_guard<std::mutex> lock(m_galleryAccessMutex);
    return m_gallery->append(new BasemapGalleryItem(basemap, std::move(thumbnail), std::move(tooltip), is3D, this));
  }

  int BasemapGalleryController::basemapIndex(Basemap* basemap) const
  {
    for (int i = 0; i < m_gallery->rowCount(); ++i)
    {
      const auto index = m_gallery->index(i);
      auto* b = m_gallery->element<BasemapGalleryItem>(index);
      if (basemap == b->basemap())
      {
        return i;
      }
    }
    return -1;
  }

  bool BasemapGalleryController::basemapMatchesCurrentSpatialReference(Basemap* basemap) const
  {
    if (!basemap || !basemap->baseLayers() || basemap->baseLayers()->isEmpty())
    {
      return false;
    }

    const SpatialReference basemapSR = basemap->baseLayers()->first()->spatialReference();

    if (basemapSR.isEmpty())
    {
      return true; // case used by the listview painter
    }

    if (!m_geoModel)
    {
      return true;
    }

    // For Global scenes using the Geographic tiling scheme, allow any geographic basemap SR.
    if (auto* scene = qobject_cast<Scene*>(m_geoModel))
    {
      bool containsTileLayer = false;
      for (auto* layer : *basemap->baseLayers())
      {
        if (layer->layerType() == LayerType::ArcGISVectorTiledLayer || layer->layerType() == LayerType::ImageTiledLayer)
        {
          containsTileLayer = true;
          break;
        }
      }
      if (!containsTileLayer)
      {
        // If the basemap does not contain any tile layers, we allow it to be applied to any scene.
        return true;
      }
      if (scene->sceneViewTilingScheme() == SceneViewTilingScheme::Geographic)
      {
        return basemapSR.isGeographic();
      }
      else if (scene->sceneViewTilingScheme() == SceneViewTilingScheme::WebMercator)
      {
        return basemapSR == SpatialReference::webMercator();
      }
    }

    // If no spatial reference is set, any basemap can be applied.
    if (m_geoModel->spatialReference().isEmpty())
    {
      return true;
    }

    return basemapSR == m_geoModel->spatialReference();
  }

  void BasemapGalleryController::setGeoModelFromGeoView(QObject* view)
  {
    //  Workaround as MapQuickView does not expose the map property in QML.
    if (auto* sceneView = qobject_cast<SceneViewToolkit*>(view))
    {
      setGeoModel(sceneView->arcGISScene());
    }
    else if (auto* localSceneView = qobject_cast<LocalSceneViewToolkit*>(view))
    {
      setGeoModel(localSceneView->arcGISScene());
    }
    else if (auto* mapView = qobject_cast<MapViewToolkit*>(view))
    {
      setGeoModel(mapView->map());
    }
  }
} // namespace Esri::ArcGISRuntime::Toolkit
