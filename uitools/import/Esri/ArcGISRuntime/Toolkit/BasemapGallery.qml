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
import Esri.ArcGISRuntime.Toolkit.Controller 100.12

import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

/*!
 \qmltype BasemapGallery
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.12
 \brief The user interface for the BasemapGallery.

 The BasemapGallery displays a collection of items representing basemaps from either ArcGIS Online, a user-defined portal,
 or an array of Basemaps. When the user selects a basemap from the BasemapGallery, the  basemap rendered in the current
 geoview is removed from the given map/scene and replaced with the basemap selected in the gallery.
 */

Control {
    id: basemapGallery

    /*!
      \brief Options for the style of BasemapGallery.

      Valid options are:

      \value ViewStyle.Automatic The BasemapGallery will display as a grid.
             If there is not enough room to render a grid, then the gallery will
             render as a list.
      \value ViewStyle.Grid The BasemapGallery will display as a grid.
      \value ViewStyle.List The BasemapGallery will display as a list.

      The default is \c ViewStyle.Automatic.
     */
    enum ViewStyle {
        Automatic,
        Grid,
        List
    }

    /*!
      \qmlproperty BasemapGalleryController controller.
      \brief The controller handles binding logic between the BasemapGallery and
      the \c GeoView and the \c Portal where applicable.
    */
    property var controller: BasemapGalleryController { }

    /*!
       \qmlproperty GeoView geoView
       \brief The \c GeoView for this tool. Should be a \c SceneView or a \c MapView.
     */
    property var geoView;

    /*!
       \qmlproperty Portal portal
       \brief The \c Portal contains basemaps which will be fetched and displayed in the gallery if applicable.
       When a valid Portal is set then `Portal.fetchBasemaps` is immediately called.

       Note: Changing the current active portal will reset the contents of the gallery.
     */
    property var portal;

    /*!
       \qmlproperty GenericListModel gallery
        The list of basemaps currently visible in the gallery. Items added or removed from this
        list will update the gallery.
    */
    property var gallery;

    /*!
       \qmlproperty Basemap currentBasemap
       \brief Currently applied basemap on the associated \c GeoView. This may be a basemap
       which does not exist in the gallery.
     */
    property var currentBasemap;

    /*!
       \qmlproperty ViewStyle style
       \brief The style of the basemap gallery. The gallery can be displayed as a list, grid, or automatically switch
       between the two based on screen real estate.

       Defaults to \c{ViewStyle.Automatic}.
     */
    property int style: BasemapGallery.ViewStyle.Automatic

    /*!
       \qmlproperty bool allowTooltips
       \brief When this property is true, mouse-hover tooltips are enabled for gallery items.
       
       Defaults to \c{true}.
     */
    property bool allowTooltips: true

    implicitWidth: style === BasemapGallery.ViewStyle.List ? internal.defaultCellWidthList
                                                           : internal.defaultCellWidthGrid * 3

    implicitHeight: style === BasemapGallery.ViewStyle.List ? internal.defaultCellHeightList * 2
                                                            : internal.defaultCellHeightGrid * 2

    width: {
        if (style === BasemapGallery.ViewStyle.Automatic && internal.calculatedStyle === BasemapGallery.ViewStyle.List) {
            return internal.defaultCellWidthList;
        } else {
            return implicitWidth;
        }
    }

    background: Rectangle {
        color: basemapGallery.palette.base
    }

    Binding {
        target: controller
        property: "portal"
        value: portal
    }

    Binding {
        target: basemapGallery
        property: "portal"
        value: controller.portal
    }

    Binding {
        target: controller
        property: "geoView"
        value: geoView
    }

    Binding {
        target: basemapGallery
        property: "gallery"
        value: controller.gallery
    }

    Binding {
        target: basemapGallery
        property: "currentBasemap"
        value: controller.currentBasemap
    }

    function setCurrentBasemap(...args) {
        return controller.setCurrentBasemap(...args);
    }

    contentItem: GridView {
        id: view
        model: controller.gallery
        cellWidth: basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.Grid ? basemapGallery.internal.defaultCellWidthGrid
                                                                                             : basemapGallery.internal.defaultCellWidthList
        cellHeight: basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.Grid ? basemapGallery.internal.defaultCellHeightGrid
                                                                                              : basemapGallery.internal.defaultCellHeightList
        clip: true
        snapMode: GridView.SnapToRow
        currentIndex: controller.basemapIndex(controller.currentBasemap)
        ScrollBar.vertical: ScrollBar { }
        delegate: Item {
            width: view.cellWidth
            height: view.cellHeight
            enabled: controller.basemapMatchesCurrentSpatialReference(modelData.basemap)
            ToolTip.visible: allowTooltips && mouseArea.containsMouse && modelData.tooltip !== ""
            ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval * 2
            ToolTip.text: modelData.tooltip
            GridLayout {
                anchors.fill: parent
                anchors.margins: 8
                flow:  {
                    if (basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.List) {
                        return GridLayout.LeftToRight;
                    } else if (basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.Grid) {
                        return GridLayout.TopToBottom;
                    }
                }
                Image {
                    id: thumbnailItem
                    source: modelData.thumbnailUrl
                    cache: false
                    fillMode: Image.PreserveAspectCrop
                    clip: true
                    Layout.maximumWidth: basemapGallery.internal.defaultCellSize
                    Layout.maximumHeight: Layout.maximumWidth
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                }
                Text {
                    id: itemText
                    color: GridView.isCurrentItem ? palette.highlightedText : palette.text
                    text: modelData.name === "" ? "Unnamed basemap" : modelData.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:  (basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.Grid) ? Qt.AlignTop
                                                                                                                    : Qt.AlignVCenter
                    minimumPointSize: 16
                    wrapMode: Text.WordWrap
                    font: basemapGallery.font
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: controller.setCurrentBasemap(modelData.basemap)
            }
        }
        highlightFollowsCurrentItem: false
        highlight: Rectangle {
            x: view.currentItem ? view.currentItem.x : NaN
            y: view.currentItem ? view.currentItem.y : NaN
            visible: view.currentIndex >= 0
            width: view.cellWidth
            height: view.cellHeight
            color: palette.highlight
            radius: 5
        }
    }

    property QtObject internal: QtObject {
        property int defaultCellSize: 100;

        property int defaultCellHeightGrid: defaultCellSize + 86;
        property int defaultCellWidthGrid: defaultCellSize + 16;

        property int defaultCellHeightList: defaultCellSize + 16;
        property int defaultCellWidthList: defaultCellSize + 116;

        property int calculatedStyle: {
            switch(basemapGallery.style) {
            case BasemapGallery.ViewStyle.List:
                return BasemapGallery.ViewStyle.List;
            case BasemapGallery.ViewStyle.Grid:
                return BasemapGallery.ViewStyle.Grid;
            case BasemapGallery.ViewStyle.Automatic:
                if (basemapGallery.parent.width < basemapGallery.implicitWidth) {
                    return BasemapGallery.ViewStyle.List;
                } else {
                    return BasemapGallery.ViewStyle.Grid;
                }

            }
        }
    }
}
