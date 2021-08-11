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

// ArcGISRuntime headers
#include <Map.h>
#include <Scene.h>

// Qt headers
#include <QPersistentModelIndex>
#include <QPointer>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    /*! 
      \internal
      \brief Convenience function which creates a QPointer for a given type \c{T} 
      which avoids explicitly stating what \c{T} is.
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
    void connectToBasemap(BasemapGalleryController* self, T* mapOrScene)
    {
      static_assert(std::is_base_of<GeoModel, T>::value, "Must be a GeoModel.");

      if (!mapOrScene)
        return;

      auto listenToLoadSignals = [self, mapOrScene]
      {
        if (auto basemap = mapOrScene->basemap())
        {
          if (basemap->loadStatus() != LoadStatus::Loaded)
            QObject::connect(basemap, &Basemap::doneLoading, self, &BasemapGalleryController::currentBasemapChanged);
        }
      };

      // If basemap changes on map or scene, disconnect from basemap and
      // signal that basemap has changed.
      QObject::connect(mapOrScene, &T::basemapChanged, self,
                       [self, listenToLoadSignals, mapOrScene](Basemap* oldBasemap)
                       {
                         QObject::disconnect(self, nullptr, oldBasemap, nullptr);
                         listenToLoadSignals(); // Connect to new basemap.
                         self->setCurrentBasemap(mapOrScene->basemap());
                       });

      listenToLoadSignals();
    }

    /*!
      \internal
      Connect to [Scene/Map].

      1. Update our cached basemap with the Scene/Map basemap.
      1. Discover the runtime type of GeoModel
      2. Connect to that type's basemapChanged signal.
     */
    void connectToGeoModel(BasemapGalleryController* self, GeoModel* geoModel)
    {
      if (geoModel->loadStatus() == LoadStatus::Loaded)
      {
        self->setCurrentBasemap(geoModel->basemap());
      }
      else
      {
        QObject::connect(geoModel, &GeoModel::doneLoading, self, [self, geoModel](Error e)
                         {
                           if (e.isEmpty())
                           {
                             self->setCurrentBasemap(geoModel->basemap());
                           }
                         });
      }

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
        // trigger UI updates.
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
      Calls Portal::fetchDeveloperBasemaps on the portal. Note that we do
      not call Portal::fetchBasemaps. The former call is for retrieving the modern API-key
      metered basemaps, while the latter returns older-style basemaps. The latter is required
      only when the user applies a custom portal, as it is also the call for retrieving an
      enterprises's custom basemaps if set.
     */
    void setToDefaultBasemaps(BasemapGalleryController* self, Portal* portal)
    {
      // For every "discovered" basemap we add it to our gallery.
      QObject::connect(
          portal, &Portal::developerBasemapsChanged, self, [portal, self]()
          {
            auto basemaps = portal->developerBasemaps();
            // For each discovered map, add it to our gallery.
            for (auto basemap : *basemaps)
            {
              self->append(basemap);
            }
          });

      // Load the portal and kick-off the group discovery.
      QObject::connect(portal, &Portal::doneLoading, self, [portal](Error e)
                       {
                         if (!e.isEmpty())
                           return;
                         portal->fetchDeveloperBasemaps();
                       });
      portal->load();
    }
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::BasemapGalleryController
    \brief The controller part of a BasemapGallery. This class handles the
    management of the BasemapGalleryItem objects, and listening to changes to the current
    Basemap of an associated GeoModel.
   */

  /*!
    \brief Constructs a new controller object with a given \a parent.

    The controller will automatically populate itself with the developer basemaps from AGOL.

     The given default basemaps require either an API key or named-user to be signed into the app.
   */
  BasemapGalleryController::BasemapGalleryController(QObject* parent) :
    QObject(parent),
    m_portal(new Portal(QUrl("https://arcgis.com"), this)),
    m_gallery(new GenericListModel(&BasemapGalleryItem::staticMetaObject, this))
  {
    connect(this, &BasemapGalleryController::geoModelChanged, this, &BasemapGalleryController::currentBasemapChanged);

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

    setToDefaultBasemaps(this, m_portal);
  }

  /*!
   \brief Destructor.
   */
  BasemapGalleryController::~BasemapGalleryController()
  {
  }

  /*!
  \brief Returns the \c GeoModel.
 */
  GeoModel* BasemapGalleryController::geoModel() const
  {
    return m_geoModel;
  }

  /*!
  \brief Set the GeoModel object this Controller uses to \a geoModel.
 */
  void BasemapGalleryController::setGeoModel(GeoModel* geoModel)
  {
    if (geoModel == m_geoModel)
      return;

    if (m_geoModel)
    {
      disconnectFromGeoModel(this, geoModel);
    }

    m_geoModel = geoModel;

    if (m_geoModel)
    {
      connectToGeoModel(this, m_geoModel);
    }

    emit geoModelChanged();
  }

  /*!
    \brief Returns the known list of available basemaps.
    Internally, this is a \c GenericListModel with an \c elementType of
    \c Basemap.
 */
  GenericListModel* BasemapGalleryController::gallery() const
  {
    return m_gallery;
  }

  /*!
    \brief Returns the current portal if set.
 */
  Portal* BasemapGalleryController::portal() const
  {
    return m_portal;
  }

  /*!
    \brief Sets the current portal. This resets the gallery.

    When \a portal is set, the basemaps of the Portal
    are fetched via \c{Portal::fetchBasemaps}.

    This is useful for displaying an organization's basemaps or to display a gallery of the old-style basemaps
    (which do not require an API key or named user.)

    To display the old-style basemaps do the following:

    \code
    controller->setPortal(new Portal(QUrl("https://arcgis.com"), this));
    \endcode
 */
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
      auto grabPortalBasemaps = [this]()
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
          connect(m_portal, &Portal::basemapsChanged, this, [this]
                  {
                    for (auto basemap : *m_portal->basemaps())
                    {
                      append(basemap);
                    }
                  });
          m_portal->fetchBasemaps();
        }
      };

      // If the portal is already loaded we grab the basemaps.
      // Otherwise, we load the portal then grab the contents.
      if (m_portal->loadStatus() == LoadStatus::Loaded)
      {
        grabPortalBasemaps();
      }
      else
      {
        connect(m_portal, &Portal::doneLoading, this, [grabPortalBasemaps](Error e)
                {
                  if (e.isEmpty())
                    grabPortalBasemaps();
                });
        m_portal->load();
      }
    }
    emit portalChanged();
  }

  /*!
    \brief Returns the current basemap selected in the BasemapGallery.

    If a GeoModel is set, this is also the basemap applied to that
    GeoModel.
   
    It is possible for the current basemap to not be in the gallery.
   */
  Basemap* BasemapGalleryController::currentBasemap() const
  {
    return m_currentBasemap;
  }

  /*!
    \brief Sets the current basemap associated with the map/scene
    of the given GeoModel to \a basemap.
   
    It is possible for the current basemap to not be in the gallery.
   */
  void BasemapGalleryController::setCurrentBasemap(Basemap* basemap)
  {
    if (basemap == m_currentBasemap)
      return;

    m_currentBasemap = basemap;
    emit currentBasemapChanged();

    if (m_geoModel)
    {
      if (auto scene = qobject_cast<Scene*>(m_geoModel))
      {
        scene->setBasemap(m_currentBasemap);
      }
      else if (auto map = qobject_cast<Map*>(m_geoModel))
      {
        map->setBasemap(m_currentBasemap);
      }
    }
  }

  /*!
    \brief Convenience function that appends a basemap to the gallery.

    \list
    \li \a basemap Basemap to add to the gallery.
    \endlist

    This is equivelent to calling

    \code
    controller->gallery()->append(new BasemapGalleryItem(basemap, controller));
    \endcode

    Returns \c true if successfully added, false otherwise.
  */
  bool BasemapGalleryController::append(Basemap* basemap)
  {
    return m_gallery->append(new BasemapGalleryItem(basemap, this));
  }

  /*!
    \brief Convenience function that appends a basemap to the gallery with an overloaded
    thumbnail and tooltip.

    \list
    \li \a basemap Basemap to add to the gallery.
    \li \a thumbnail Thumbnail to display in the gallery.
    \li \a tooltip Tooltip to show when mouse hovers over the gallery item.
    \endlist

    This is equivelent to calling

    \code
    controller->gallery()->append(new BasemapGalleryItem(basemap, thumbnail, tooltip, controller));
    \endcode
   */
  bool BasemapGalleryController::append(Basemap* basemap, QImage thumbnail, QString tooltip)
  {
    return m_gallery->append(new BasemapGalleryItem(basemap, std::move(thumbnail), std::move(tooltip), this));
  }

  /*!
    \internal
    \brief Given a \a basemap, returns the index of the basemap within the gallery.
    This comparison is performed via pointer comparison.
    If the basemap is not found then \c{-1} is returned.
   */
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

  /*!
   \internal
   \brief Given a \a basemap, returns whether the spatial reference of its layers
   match the spatial reference of the GeoModel (and therefore if it appropriate to apply
   as the current basemap.)
   */
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

    // Test if all layers match the spatial reference.
    // From the spec we are guaranteed the homogeneity of the spatial references of these layers.
    // https://developers.arcgis.com/web-map-specification/objects/spatialReference/
    const auto layers = basemap->baseLayers();
    return std::all_of(std::cbegin(*layers), std::cend(*layers), [&sp](Layer* layer)
                       {
                         const auto sp2 = layer->spatialReference();
                         return sp2.isEmpty() || sp == sp2;
                       });
  }

  /*!
   \brief Convenience function for QML/C++ users which allows the map/scene to be extracted from a
   SceneView or MapView assigned to \a view in QML code.

  This is only a concern as [Map/Scene]QuickView does not expose a [Map/Scene] property in QML.

   For example, to hook up BasemapGallery with a MapQuickView (which does not expose a map property):

    \code
     MapView {
        BasemapGallery {
            id: gallery
            anchors {
                left: view.left
                top: view.top
                margins: 5
        }
        onMapChanged: gallery.setGeoModelFromGeoView(this)
     }
    \endcode
  */
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

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoModelChanged()
  \brief Emitted when the geoModel has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::portalChanged()
  \brief Emitted when the portal has changed.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::currentBasemapChanged()
  \brief Emitted when the current basemap has changed.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoModel
  \brief The geoModel the controller is listening for basemap changes.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoModel()
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::portal
  \brief The optional portal the controller queries for basemaps
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::portal()
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::currentBasemap
  \brief The current basemap of in the current geoModel.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::currentBasemap()
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::gallery
  \brief The gallery of BasemapGalleryItem objects.
  \sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::gallery()
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
