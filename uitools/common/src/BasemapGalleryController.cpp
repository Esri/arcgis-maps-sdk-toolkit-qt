
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

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {
    /*!
      \internal
     */
    template <typename T>
    auto qPointerFrom(T* t)
    {
      return QPointer<T>{t};
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
    template <typename T>
    void connectToBasemap(BasemapGalleryController* self, T* geoModel)
    {
      static_assert(std::is_base_of<GeoModel, T>::value, "Must be a GeoModel.");

      if (!geoModel)
        return;

      auto listenToLoadSignals = [self](Basemap* basemap)
      {
        if (basemap)
        {
          if (basemap->loadStatus() != LoadStatus::Loaded)
            QObject::connect(basemap, &Basemap::doneLoading, self, &BasemapGalleryController::currentBasemapChanged);
        }
      };

      // If basemap changes on map or scene, disconnect from basemap and
      // signal that basemap has changed.
      QObject::connect(geoModel, &T::basemapChanged, self,
                       [self, listenToLoadSignals, geoModel](Basemap* oldBasemap)
                       {
                         QObject::disconnect(self, nullptr, oldBasemap, nullptr);
                         auto newBasemap = geoModel->basemap();
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
      if (auto scene = qobject_cast<Scene*>(geoModel))
      {
        connectToBasemap(self, scene);
      }
      else if (auto map = qobject_cast<Map*>(geoModel))
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
        return;

      auto pIndex = QPersistentModelIndex(index);
      auto notifyChange = [pIndex, gallery]
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

      auto basemap = galleryItem->basemap();

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
        return;

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
      Takes a BasemapListModel*, sorts them alphabetically, and adds them to the basemap gallery.

      Because the basemaps are initially unloaded, Basemap->item() must be used to access the
      basemap metadata. The basemaps are sorted using Basemap->item()->title().
     */
    void sortBasemapsAndAddToGallery(BasemapGalleryController* self, BasemapListModel* basemaps, bool is3D = false)
    {
      // Convert BasemapListModel into a Basemap* vector and sort basemaps alphabetically using the title
      std::vector<Basemap*> basemapsVector;
      basemapsVector.reserve(basemaps->rowCount());
      std::copy(std::cbegin(*basemaps), std::cend(*basemaps), std::back_inserter(basemapsVector));
      std::sort(std::begin(basemapsVector), std::end(basemapsVector), [](Basemap* b1, Basemap* b2)
                {
                  // Check validity of basemap->item() and if title() is empty. If either is true, push to end of list.
                  if (!b1->item() || b1->item()->title().isEmpty())
                    return false;
                  else if (!b2->item() || b2->item()->title().isEmpty())
                    return true;
                  else
                    return b1->item()->title() < b2->item()->title();
                });

      for (auto basemap : basemapsVector)
      {
        self->append(basemap, is3D);
      }
    }

    /*!
      \internal
      Calls Portal::fetchDeveloperBasemapsAsync on the portal. Note that we do
      not call Portal::fetchBasemapsAsync. The former call is for retrieving the modern API-key
      metered basemaps, while the latter returns older-style basemaps. The latter is required
      only when the user applies a custom portal, as it is also the call for retrieving an
      enterprises's custom basemaps if set.
     */
    void setToDefaultBasemaps(BasemapGalleryController* self, Portal* portal)
    {
      // Load the portal and kick-off the group discovery.
      QObject::connect(portal, &Portal::doneLoading, self, [portal, self](Error e)
                       {
                         if (!e.isEmpty())
                           return;

                         portal->fetchDeveloperBasemapsAsync().then(
                         [portal, self]()
                         {
                           // Sort and append the basemaps to the gallery.
                           BasemapListModel* basemaps = portal->developerBasemaps();
                           sortBasemapsAndAddToGallery(self, basemaps);
                           // Notify the demo that the basemaps have changed.
                           emit self->basemapsChanged();
                         });

                         if (qobject_cast<Scene*>(self->geoModel()))
                         {
                           portal->fetch3DBasemapsAsync().then(
                                 [portal, self]()
                           {
                             // Sort and append the basemaps to the gallery.
                             BasemapListModel* basemaps = portal->basemaps3D();
                             sortBasemapsAndAddToGallery(self, basemaps, true);
                             // Notify the demo that the basemaps have changed.
                             emit self->basemapsChanged();
                           });
                         }
                       });
      portal->load();
    }
  }

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryController
    \internal

    This class is an internal implementation detail and is subject to change.
   */

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
                if (auto galleryItem = m_gallery->element<BasemapGalleryItem>(index))
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
                if (auto galleryItem = m_gallery->element<BasemapGalleryItem>(index))
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
    setToDefaultBasemaps(this, m_portal);
    // Have to set the property names, so the controller will know how to match the properties from
    // basemapgalleryitem with the specific Qt::<namespace> invoked in the .data() from the View (ListView) obj
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
      return;

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
      return;

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
      // If portal basemaps are populated, add the contents to the gallery.
      // Otherwise attempt a fetch of the contents then add to the gallery.
      doOnLoaded(m_portal, this, [this]
               {
                 if (m_portal->basemaps()->rowCount() > 0)
                 {
                   for (auto basemap : *m_portal->basemaps())
                   {
                     append(basemap);
                   }
                 }
                 else
                 {
                   m_portal->fetchBasemapsAsync().then(
                   [this]()
                   {
                     BasemapListModel* basemaps = m_portal->basemaps();
                     sortBasemapsAndAddToGallery(this, basemaps);
                     emit basemapsChanged();
                   });
                 }

                 if (qobject_cast<Scene*>(m_geoModel))
                 {
                   if (m_portal->basemaps3D()->rowCount() > 0)
                   {
                     for (auto basemap : *m_portal->basemaps3D())
                     {
                       append(basemap);
                     }
                   }
                   else
                   {
                     m_portal->fetch3DBasemapsAsync().then(
                     [this]()
                     {
                       BasemapListModel* basemaps = m_portal->basemaps3D();
                       sortBasemapsAndAddToGallery(this, basemaps, true);
                       emit basemapsChanged();
                     });
                   }
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
    auto apply = [basemap, this](Error e)
    {
      if (e.isEmpty())
      {
        if (basemap == m_currentBasemap)
          return;
        if (!basemapMatchesCurrentSpatialReference(basemap))
        {
          // force redraw for the single basemapGalleryItem updated
          emit m_gallery->dataChanged(
              m_gallery->index(basemapIndex(basemap)), m_gallery->index(basemapIndex(basemap)));
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
        qDebug() << "problem in loading the layer";
      }
    };
    if (basemap->baseLayers()->size() > 0)
    {
      if (basemap->baseLayers()->first()->loadStatus() != LoadStatus::Loaded)
      {
        singleShotConnection(
            basemap->baseLayers()->first(), &Layer::doneLoading, this, apply);
        basemap->baseLayers()->first()->load();
      }
      else
      {
        apply(Error{});
      }
    }
  }

  bool BasemapGalleryController::append(Basemap* basemap)
  {
    std::lock_guard<std::mutex> lock(m_galleryAccessMutex);
    return m_gallery->append(new BasemapGalleryItem(basemap, this));
  }

  bool BasemapGalleryController::append(Basemap* basemap, bool is3D)
  {
    std::lock_guard<std::mutex> lock(m_galleryAccessMutex);
    return m_gallery->append(new BasemapGalleryItem(basemap, {}, {}, is3D, this));
  }

  bool BasemapGalleryController::append(Basemap* basemap, QImage thumbnail, QString tooltip)
  {
    std::lock_guard<std::mutex> lock(m_galleryAccessMutex);
    return m_gallery->append(new BasemapGalleryItem(basemap, std::move(thumbnail), std::move(tooltip), this));
  }

  int BasemapGalleryController::basemapIndex(Basemap* basemap) const
  {
    for (int i = 0; i < m_gallery->rowCount(); ++i)
    {
      auto index = m_gallery->index(i);
      auto b = m_gallery->element<BasemapGalleryItem>(index);
      if (basemap == b->basemap())
        return i;
    }
    return -1;
  }

  bool BasemapGalleryController::basemapMatchesCurrentSpatialReference(Basemap* basemap) const
  {
    if (!basemap)
      return false;

    SpatialReference sp;
    if (m_geoModel)
    {
      sp = m_geoModel->spatialReference();
    }

    // If no spatial reference is set, any basemap can be applied.
    if (sp.isEmpty())
      return true;
    auto item = basemap->item();

    if (item)
    {
      auto it_sp = item->spatialReference();
      if (item && !it_sp.isEmpty())
        return sp == item->spatialReference();
    }

    const auto layers = basemap->baseLayers();
    if (layers->size() <= 0)
      return false;

    //scene case:
    if (auto scene = qobject_cast<Scene*>(m_geoModel))
    {
      const auto sp2 = basemap->baseLayers()->first()->spatialReference();
      if (sp2.isEmpty()) //case used by the listview painter
        return true;
      auto svts = scene->sceneViewTilingScheme();
      switch (svts)
      {
      case SceneViewTilingScheme::Geographic:
        return sp2.isGeographic();

      case SceneViewTilingScheme::WebMercator:
        return sp2 == SpatialReference::webMercator();

      default:
        qDebug() << "a new sceneviewTilingScheme has been used";
        break;
      }
      return false;
    }

    // Test if first layer matches the spatial reference.
    // From the spec we are guaranteed the homogeneity of the spatial references of these layers.
    // https://developers.arcgis.com/web-map-specification/objects/spatialReference/

    const auto layer = layers->first();
    const auto sp2 = layer->spatialReference();
    return sp2.isEmpty() || sp == sp2;

    return false;
  }

  void BasemapGalleryController::setGeoModelFromGeoView(QObject* view)
  {
    //  Workaround as MapQuickView does not expose the map property in QML.
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(view))
    {
      setGeoModel(sceneView->arcGISScene());
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(view))
    {
      setGeoModel(mapView->map());
    }
  }

} // Esri::ArcGISRuntime::Toolkit
