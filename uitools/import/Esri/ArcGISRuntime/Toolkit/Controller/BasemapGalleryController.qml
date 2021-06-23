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
import QtQml 2.12
import Esri.ArcGISRuntime 100.12
import QtQml.Models 2.15

/*!
   \qmltype BasemapGalleryController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRutime 100.12
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief The controller part of a BasemapGallery. This class handles the
   management of the BasemapGalleryItem objects, and listening to changes to the current
   Basemap of an associated GeoView.
 */


QtObject {
    id: controller
    /*!
      \qmlproperty GeoView geoView
      \brief The geoview the controller is listening for basemap changes.
     */
    property var geoView: null;

    /*!
      \qmlproperty Portal portal
      \brief The optional portal the controller queries for basemaps
     */
    property Portal portal: Portal {
        url: "https://arcgis.com"

        // On startup with the default portal,
        // get the developer basemaps.
        Component.onCompleted: {
            if (this.loadStatus === Enums.LoadStatusLoaded) {
                this.fetchDeveloperBasemaps();
            } else {
                this.loadStatusChanged.connect(
                         () => {
                             if (this.loadStatus === Enums.LoadStatusLoaded) {
                                 fetchDeveloperBasemaps();
                             }
                         });
                this.load();
            }
        }
    }

    /*!
      \qmlproperty Basemap currentBasemap
      \brief The current basemap of the scene/map in the current geoView.
     */
    readonly property alias currentBasemap: internal.currentBasemap

    /*!
      \qmlproperty ListModel gallery
      \brief The gallery of BasemapGalleryItem objects.

      Internally, the gallery is a QML ListModel containing elements of type ListElement.
      Each ListElement has a single property `modelData`, which maps ot a BasemapGalleryItem.
     */
    readonly property alias gallery: internal.gallery

    /*!
       \qmlmethod BasemapGalleryController::setCurrentBasemap(Basemap basemap)
       \brief Sets the current basemap associated with the map/scene
       of the given GeoView to \a basemap.

       It is possible for the current basemap to not be in the gallery.
     */
    function setCurrentBasemap(basemap) {
        if (geoView === null)
            return;

        if (geoView.map !== undefined) {
            // This is a MapView.
            geoView.map.basemap = basemap;
        }
        else if (geoView.scene !== undefined) {
            // This is a SceneView.
            geoView.scene.basemap = basemap;
        }
    }

    /*!
        \qmlmethod BasemapGalleryController::append(Basemap basemap, url thumbnailUrl, string tooltip)
        \brief Convenience function that appends a basemap to the gallery with an overloaded
        thumbnail and tooltip.

        \list
        \li \a basemap Basemap to add to the gallery.
        \li \a thumbnailUrl Thumbnail to display in the gallery. Optional.
        \li \a tooltip Tooltip to show when mouse hovers over the gallery item. Optional.
        \endlist
     */
    function append(basemap, thumbnailUrl, tooltip) {
        if (basemap === null) {
            return false;
        }
        internal.gallery.append(
                    { "modelData" : internal.galleryItem.createObject(
                                        this,
                                        {
                                            basemap: basemap,
                                            thumbnailUrlOverride: thumbnailUrl,
                                            tooltipOverride: tooltip
                                        })
                    });
        return true;
    }

    /*!
        \internal
        \qmlmethod BasemapGalleryController::basemapIndex(Basemap basemap)
        \brief Given a \a basemap, returns the index of the basemap within the gallery.
        This comparison is performed via name and itemId comparison (if available).
        If the basemap is not found then \c{-1} is returned.
     */
    function basemapIndex(basemap) {
        if (basemap === null) {
            return -1;
        }

        // We do a name and ItemID comparisons, since the ArcGIS QML API disallows
        // pointer comparisons.
        for (let i = 0; i < gallery.count; i++) {
            let b = gallery.get(i).modelData;
            if (b &&  b.basemap && b.basemap.name === basemap.name) {
                // If there is an item involved, we can check the itemId to confirm the sameness.
                if ((b.basemap.item && basemap.item && b.basemap.item.itemId === basemap.item.itemId)
                        || (b.basemap.item === null && basemap.item === null))
                    return i;
            }
        }
        return -1;
    }

    /*!
      \internal
      \qmlmethod BasemapGalleryController::basemapMatchesCurrentSpatialReference(Basemap basemap)
      \brief Given a \a basemap, returns whether the spatial reference of its layers
      match the spatial reference of the GeoView (and therefore if it appropriate to apply
      as the current basemap.)
     */
    function basemapMatchesCurrentSpatialReference(basemap) {
        if (basemap === null) {
            return false;
        }

        if (geoView === null) {
            return false;
        }

        let sp1 = null;

        if (geoView.map !== undefined) {
            // This is a MapView.
            sp1 = geoView.map.spatialReference;
        }
        else if (geoView.scene !== undefined) {
            // This is a SceneView.
            sp1 = geoView.scene.spatialReference;
        }

        if (sp1 === null) {
            return true;
        }

        for (let i = 0; i < basemap.baseLayers.count; i++) {
            let layer = basemap.baseLayers.get(i);
            if (layer === null) {
                continue;
            }

            let sp2 = layer.spatialReference;
            if (sp2 === null) {
                continue;
            }

            if (!sp2.equals(sp1)) {
                return false;
            }
        }
        return true;
    }

    /*! \internal */
    property QtObject internal : QtObject {
        id: internal

        property Connections portalConnections: Connections {
            target: controller.portal
            function onFetchDeveloperBasemapsStatusChanged() {
                if (controller.portal.fetchDeveloperBasemapsStatus === Enums.TaskStatusCompleted) {
                    let basemaps = controller.portal.developerBasemaps;
                    for (let i = 0; i < basemaps.count; i++) {
                        controller.append(basemaps.get(i));
                    }
                }
            }
            function onFetchBasemapsStatusChanged() {
                if (controller.portal.fetchBasemapsStatus === Enums.TaskStatusCompleted) {
                    let basemaps = controller.portal.basemaps;
                    for (let i = 0; i < basemaps.count; i++) {
                        controller.append(basemaps.get(i));
                    }
                }
            }
        }

        property var currentBasemap: null;

        property Binding mapBinding: Binding {
            when: geoView !== null && geoView.map !== undefined
            target: internal
            property: "currentBasemap"
            value: geoView.map.basemap
        }

        property Binding sceneBinding: Binding {
            when: geoView !== null && geoView.scene !== undefined
            target: internal
            property: "currentBasemap"
            value: geoView.scene ? geoView.scene.basemap : null
        }

        property Binding nullBinding: Binding {
            when: geoView === null
            target: internal
            property: "currentBasemap"
            value: null
        }

        property ListModel gallery: ListModel { }

        property Component galleryItem: Component {
            BasemapGalleryItem { }
        }
    }

    // When the portal is set, we grab the basemaps from it,
    // if applicable.
    onPortalChanged: {
        gallery.clear();

        if (portal === null) {
            return;
        }

        if (portal.loadStatus === Enums.LoadStatusLoaded) {
            if (portal.basemaps.count === 0) {
                portal.fetchBasemaps();
            } else {
                for (let i = 0; i < portal.basemaps.count; i++) {
                    controller.append(portal.basemaps.get(i));
                }
            }
        } else {
            portal.loadStatusChanged.connect(
                        () => {
                            if (portal.loadStatus === Enums.LoadStatusLoaded) {
                                portal.fetchBasemaps();
                            }
                        });
            portal.load();
        }
    }
}
