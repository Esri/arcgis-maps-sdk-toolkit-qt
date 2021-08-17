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
import QtQuick 2.12
import Esri.ArcGISRuntime 100.13

/*!
   \qmltype BasemapGalleryItem
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRutime 100.12
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief Represents the contents of a BasemapGallery. Can be accessed via
   \c{BasemapGalleryController.gallery}.

  The BasemapGalleryItem is a wrapper around a Basemap, along with any additional
  meta-information.

  The BasemapGalleryItem allows for the optional application of a thumbnail and tooltip,
  which override the given thumbnail/tooltip of the Basemap.

  This is handy when creating Basemaps on the fly from a BasemapStyle or other means, as
  these basemaps do not come with a thumbnail or tooltip by default.
 */


QtObject {
    id: galleryItem
    /*!
      \qmlproperty Basemap basemap.
      \brief The basemap this gallery item represents.
     */
    property var basemap: null;

    /*!
    \qmlproperty string name.
    \brief
    */
    readonly property string name: basemap ? basemap.name : ""

    /*!
     \qmlproperty string tooltip.
     \brief the tooltip override of this item if valid. Otherwise
        returns the description on the basemap's item, if available.
     */
    readonly property string tooltip: {
        if (tooltipOverride == "") {
            if (internal.item) {
                return internal.item.description;
            }
        }
        return tooltipOverride;
    }

    /*!
     \qmlproperty string tooltipOverride.
     \brief The tooltip override of this item, which replaces the
     thumbnail of the basemap when rendering the gallery.
     */
    property string tooltipOverride: ""

    /*!
     \qmlproperty url thumbnailUrl.
     \brief the thumbnail url of this item if valid. Otherwise
        returns the thumbnail url on the basemap's item, if available.
     */
    readonly property url thumbnailUrl: {
        if (thumbnailUrlOverride != "") {
            return thumbnailUrlOverride;
        }

        if (internal.item) {
            return internal.item.thumbnailUrl;
        }

        return "qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/generic_basemap_thumbnail.png";
    }

    /*!
     \qmlproperty url thumbnailUrlOverride.
     \brief The thumbnail url override of this item, which replaces the
     thumbnail of the basemap when rendering the gallery.
     */
    property url thumbnailUrlOverride: "";

    Component.onCompleted: {
        if (basemap.loadStatus !== Enums.LoadStatusLoaded) {
            basemap.load();
        }
    }

    property QtObject internal: QtObject {
        // This is a workaround for undesirable behaviour.
        // `basemap.item` will be deleted after access unless we store it in a temp.
        // We don't want it to delete itself as we also lose the thumbnail when it does.
        property ArcGISItem item: basemap ? basemap.item : null;
    }
}
