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
     \brief Convenience template which returns a Map or Scene based on the type of `T`.
     */
    template <typename T>
    auto getMapOrScene(T* item);

    /*!
     \internal
     \brief Variant that returns a map.
     */
    template <>
    auto getMapOrScene(MapViewToolkit* item)
    {
      return item->map();
    }

    /*!
     \internal
     \brief Variant that returns a scene.
     */
    template <>
    auto getMapOrScene(SceneViewToolkit* item)
    {
      return item->arcGISScene();
    }

    /*! 
     \internal
     Convenience function which creates a QPointer for a given type T which avoids explicitly stating what T is.
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
    void connectToMapScene(BasemapGalleryController* self, T* mapOrScene)
    {
      static_assert(std::is_same<T, Scene>::value || std::is_same<T, Map>::value, "Type must be a Map or Scene.");

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
                       [self, listenToLoadSignals](Basemap* oldBasemap)
                       {
                         QObject::disconnect(self, nullptr, oldBasemap, nullptr);
                         listenToLoadSignals(); // Connect to new basemap.
                         emit self->currentBasemapChanged();
                       });

      listenToLoadSignals();
    }

    /*!
     \internal
     Connect to [Scene/Map]View and associated Map/Scene and associated Basemap.

     1. We Connect up to the current map/scene.
     2. If the map/scene changes we disconnect from the old one.
     3. We connect up to the new map/scene.
     4. We emit a basemapChanged signal whenever the map or scene changes.
     */
    template <typename T, typename F>
    void connectToView(BasemapGalleryController* self, T* view, F mapOrSceneChangedSignal)
    {
      static_assert(std::is_same<T, SceneViewToolkit>::value || std::is_same<T, MapViewToolkit>::value, "Type must be a MapView or SceneView.");

      auto item = getMapOrScene(view);
      connectToMapScene(self, item);

      // If the current map/scene changes, disconnect all connections to the old ones.
      auto oldItem = qPointerFrom(item);
      QObject::connect(view, mapOrSceneChangedSignal, self,
                       [oldItem, self, view]
                       {
                         if (oldItem)
                         {
                           QObject::disconnect(oldItem, nullptr, self, nullptr);
                           if (Basemap* basemap = oldItem->basemap())
                           {
                             QObject::disconnect(basemap, nullptr, self, nullptr);
                           }
                         }
                         // Connect up to new signals.
                         connectToMapScene(self, getMapOrScene(view));
                         emit self->currentBasemapChanged();
                       });
    }

    /*!
     \internal
     1. Disconnect from [Scene/Map]View.
     2. Disconnect from the associated Map/Scene.
     3. Disconnect from the associated Basemap.
     */
    template <typename T>
    void disconnectFromView(BasemapGalleryController* self, T* view)
    {
      static_assert(std::is_same<T, SceneViewToolkit>::value || std::is_same<T, MapViewToolkit>::value, "Type must be a MapView or SceneView.");
      if (view)
      {
        QObject::disconnect(view, nullptr, self, nullptr);
        if (auto mapOrScene = getMapOrScene(view))
        {
          QObject::disconnect(mapOrScene, nullptr, self, nullptr);
          if (Basemap* basemap = mapOrScene->basemap())
          {
            QObject::disconnect(basemap, nullptr, self, nullptr);
          }
        }
      }
    }

    /*!
     \internal
     Triggered when a basemap is added to the gallery.

     1. We listen for GalleryItem changes.
     2. We force the basemap to load if not already.
     3. We emit `currentBasemapChanged` if the current basemap was
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
        // If the currently active basemap was added to the gallery, we let
        // downstream consumers know the currently active basemap has changed also to
        // trigger UI updates.
        emit self->currentBasemapChanged();
      }
    }

    /*!
     \internal
     Triggered when a basemap is removed from the gallery.

     1. We disconnect from the GalleryItem.
     2. We emit `currentBasemapChanged` if the current basemap was
        removed from the gallery.
     3. We delete the GalerryItem if we are the parent.
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
     Calls `Portal::fetchDeveloperBasemaps` on the portal. Note that we do
     not call `Portal::fetchBasemaps`. The former call is for retrieving the modern API-key
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
  Basemap of an associated GeoView.
  */

  /*!
   Constructs a new controller object with a given \a parent.
   The controller will automatically populate itself with the deveoper basemaps from AGOL.

   The given default basemaps require either an API key or named-user to be signed into the app.
   */
  BasemapGalleryController::BasemapGalleryController(QObject* parent) :
    QObject(parent),
    m_portal(new Portal(QUrl("https://arcgis.com"), this)),
    m_gallery(new GenericListModel(&BasemapGalleryItem::staticMetaObject, this))
  {
    connect(this, &BasemapGalleryController::geoViewChanged, this, &BasemapGalleryController::currentBasemapChanged);

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
   \brief Destructor
   */
  BasemapGalleryController::~BasemapGalleryController()
  {
  }

  /*!
  \brief Returns the \c GeoView as a \c QObject.
 */
  QObject* BasemapGalleryController::geoView() const
  {
    return m_geoView;
  }

  /*!
  \brief Set the GeoView object this Controller uses.
  
  Internally this is cast to a \c MapView or \c SceneView using \c qobject_cast,
  which is why the paremeter is of form \c QObject and not \c GeoView.
  
  \list
    \li \a geoView \c Object which must inherit from \c{GeoView*} and 
        \c{QObject*}. 
  \endlist
 */
  void BasemapGalleryController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
    {
      disconnect(m_geoView, nullptr, this, nullptr);

      // Disconnect from old map/scene if it exists.
      if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        disconnectFromView(this, sceneView);
      }
      else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        disconnectFromView(this, mapView);
      }
    }

    m_geoView = geoView;

    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      connectToView(this, sceneView, &SceneViewToolkit::sceneChanged);
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      connectToView(this, mapView, &MapViewToolkit::mapChanged);
    }
    emit geoViewChanged();
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
   * \brief Returns the current basemap associated with the map/scene
   * of the given GeoView.
   *
   * It is possible for the current basemap to not be in the gallery.
   */
  Basemap* BasemapGalleryController::currentBasemap() const
  {
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      if (auto scene = sceneView->arcGISScene())
      {
        return scene->basemap();
      }
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      if (auto map = mapView->map())
      {
        return map->basemap();
      }
    }
    return nullptr;
  }

  /*!
   * \brief Sets the current basemap associated with the map/scene
   * of the given GeoView to \a basemap.
   *
   * It is possible for the current basemap to not be in the gallery.
   */
  void BasemapGalleryController::setCurrentBasemap(Basemap* basemap)
  {
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      auto scene = sceneView->arcGISScene();
      if (scene)
      {
        scene->setBasemap(basemap);
      }
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      auto map = mapView->map();
      if (map)
      {
        map->setBasemap(basemap);
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
  controller->gallery()->append(new BasemapGalleryItem(basemap), controller);
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
            controller->gallery()->append(new BasemapGalleryItem(basemap, thumbnail, tooltip), controller);
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
   match the spatial reference of the GeoView (and therefore if it appropriate to apply
   as the current basemap.)
   */
  bool BasemapGalleryController::basemapMatchesCurrentSpatialReference(Basemap* basemap) const
  {
    if (!basemap)
      return false;

    SpatialReference sp;
    if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
    {
      auto scene = sceneView->arcGISScene();
      if (scene)
      {
        sp = scene->spatialReference();
      }
    }
    else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
    {
      auto map = mapView->map();
      if (map)
      {
        sp = map->spatialReference();
      }
    }

    // If no spatial reference is set, any basemap can be applied.
    if (sp.isEmpty())
      return true;

    // Test if all layers match the spatial reference.
    // From the spec we are guaranteed the homogenity of the spatial references of these layers.
    // https://developers.arcgis.com/web-map-specification/objects/spatialReference/
    const auto& layers = *basemap->baseLayers();
    return std::all_of(std::begin(layers), std::end(layers), [&sp](Layer* layer)
                       {
                         const auto sp2 = layer->spatialReference();
                         return sp2.isEmpty() || sp == sp2;
                       });
  }

/*!
 \fn void Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoViewChanged()
 \brief Emitted when the geoView has changed.
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
\property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoView
\brief The geoview the controller is listening for basemap changes.
\sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::geoView()
*/

/*!
\property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::portal
\brief The optional portal the controller queries for basemaps
\sa Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::portal()
*/

/*!
\property Esri::ArcGISRuntime::Toolkit::BasemapGalleryController::currentBasemap
\brief The current basemap of the scene/map in the current geoView.
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
