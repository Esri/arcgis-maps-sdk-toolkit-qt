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
import QtQml 2.15
import Esri.ArcGISRuntime 100.15
import QtQml.Models 2.15

/*!
   \qmltype BasemapGalleryController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.12
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief The controller part of a BasemapGallery. This class handles the
   management of the BasemapGalleryItem objects, and listening to changes to the current
   Basemap of an associated GeoModel.
 */

QtObject {
    id: controller
    /*!
      \qmlproperty GeoModel geoModel
      \brief The geomodel the controller is listening for basemap changes.
     */
    property GeoModel geoModel: null;

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
      \brief The current basemap of the scene/map in the current geoModel.
     */
    readonly property alias currentBasemap: internal.currentBasemap

    /*!
      \qmlproperty ListModel gallery
      \brief The gallery of BasemapGalleryItem objects.

      Internally, the gallery is a QML ListModel containing elements of type ListElement.
      Each ListElement has a single property `modelData`, which maps to a BasemapGalleryItem.
     */
    readonly property alias gallery: internal.gallery

    /*!
       \qmlmethod void BasemapGalleryController::setCurrentBasemap(Basemap basemap)
       \brief Sets the current basemap associated with the map/scene
       of the given GeoModel to \a basemap.

       It is possible for the current basemap to not be in the gallery.
     */
    function setCurrentBasemap(basemap) {
        //set connection with matchescurrentsp to run after the basemap is loaded
        const layer = basemap.baseLayers.get(0);
        layer.loadStatusChanged.connect(internal.setCurrentBasemapMatchingSp.bind(null, basemap, layer));
        if (layer.loadStatus !== Enums.LoadStatusLoaded) {
            layer.load();
        } else {
            //call directly without waiting. Layer is already loaded
            internal.setCurrentBasemapMatchingSp(basemap, layer);
        }
    }

    /*!
       \brief Convenience method that takes a GeoView \a view and sets this controller's
       geoModel to the scene or map contained within. This method is for QML/C++ layer
       compatibility. It is better to set \c{BasemapGalleryController.geoModel} directly.
     */
    function setGeoModelFromGeoView(view) {
        if (view instanceof SceneView) {
            setCurrentBasemap(view.scene ? view.scene.basemap : null);
        } else if (view instanceof MapView) {
            setCurrentBasemap(view.map ? view.map.basemap : null);
        }
    }

    /*!
      \qmlmethod bool BasemapGalleryController::append(Basemap basemap, url thumbnailUrl, string tooltip)
      \brief Convenience function that appends a basemap to the gallery with an overloaded
      thumbnail and tooltip.

      Returns whether the append was successful.

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
      \qmlmethod int BasemapGalleryController::basemapIndex(Basemap basemap)
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
            if (b && b.basemap && b.basemap.name === basemap.name) {
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
      \qmlmethod bool BasemapGalleryController::basemapMatchesCurrentSpatialReference(Basemap basemap)
      \brief Given a \a basemap, returns whether the spatial reference of its layers
      match the spatial reference of the GeoModel (and therefore if it appropriate to apply
      as the current basemap.)
     */
    function basemapMatchesCurrentSpatialReference(basemap) {
        if (basemap === null) {
            return false;
        }

        if (geoModel === null) {
            return false;
        }

        let sp = geoModel.spatialReference;

        if (sp === null) {
            return true;
        }
        let item = basemap.item;
        if (item !== null) {
            let it_sp = item.spatialReference;
            if (item !== null && it_sp !== null) {
                return sp.equals(item.spatialReference);
            }
        }

        let layers = basemap.baseLayers;

        if (layers.count <= 0)
            return false;

        let layer = layers.get(0);

        // scene case
        if (geoModel instanceof Scene) {
            let sp2 = layer.spatialReference;
            if (sp2 === null)
                return true;
            let svts = geoModel.sceneViewTilingScheme;
            switch(svts) {
            case Enums.SceneViewTilingSchemeGeographic:
                return sp2.isGeographic;
            case Enums.SceneViewTilingSchemeWebMercator:
                return sp2.equals(Factory.SpatialReference.createWebMercator());
            default:
                console.log("a new sceneViewTilingScheme has been used!");
            }
            return false;
        }

        // map case
        //check layer null?
        let sp2 = layer.spatialReference;
        return sp2 === null || sp.equals(sp2);
    }

    /*! \internal */
    property QtObject internal : QtObject {
        id: internal

        property Connections portalConnections: Connections {
            target: controller.portal
            function onFetchDeveloperBasemapsStatusChanged() {
                if (controller.portal.fetchDeveloperBasemapsStatus === Enums.TaskStatusCompleted) {
                    let basemaps = controller.portal.developerBasemaps;

                    internal.sortBasemapsAndAddToGallery(basemaps);
                }
            }
            function onFetchBasemapsStatusChanged() {
                if (controller.portal.fetchBasemapsStatus === Enums.TaskStatusCompleted) {
                    let basemaps = controller.portal.basemaps;

                    internal.sortBasemapsAndAddToGallery(basemaps);
                }
            }
        }
        property Connections geoModelConnections: Connections {
            target: geoModel
            function onBasemapChanged() {
                internal.currentBasemap = geoModel.basemap;
            }
        }

        function setCurrentBasemapMatchingSp(basemap, layer) {
            if (layer.loadStatus === Enums.LoadStatusLoaded) {
                layer.loadStatusChanged.disconnect(setCurrentBasemapMatchingSp);
                if (!basemapMatchesCurrentSpatialReference(basemap)) {
                    // manually setting the currbasemap to trigger the event onCurrentBasemapChanged
                    internal.currentBasemap = internal.currentBasemap;
                    internal.gallery.dataChanged(internal.gallery.index(basemapIndex(basemap), 0), internal.gallery.index(basemapIndex(basemap), 0));
                } else {
                    geoModel.basemap = basemap;
                }
            }
        }

        property var currentBasemap: null;

        property ListModel gallery: ListModel { }

        property Component galleryItem: Component {
            BasemapGalleryItem { }
        }

        function sortBasemapsAndAddToGallery(basemaps) {
            // Copy each element of the BasemapListModel from the portal (i.e. basemaps) into a new array.
            let basemapsArray = [];
            basemaps.forEach(element => basemapsArray.push(element));

            // Copy the array to ensure that elements are not deleted prematurely by the garbage collector
            basemapsArray = basemapsArray.map(element => { return {element: element, item: element.item}});

            // Sort the basemaps in basemapVector alphabetically using the title.
            basemapsArray.sort(function(b1, b2) {
                // Check validity of basemap.item and if title is empty. If either is true, push to end of list.
                if (!b1.item || b1.item.title === "")
                    return 1;
                else if (!b2.item || b2.item.title === "")
                    return -1;
                else {
                    // localeCompare returns 1 if b1 > b2, 0 if b1 = b2, and -1 if b1 < b2.
                    return b1.item.title.localeCompare(b2.item.title)
                }
            });

            // Add each basemap to the Basemap Gallery.
            basemapsArray.forEach(element => controller.append(element.element));
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
