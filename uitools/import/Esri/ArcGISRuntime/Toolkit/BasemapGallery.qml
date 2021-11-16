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
import Esri.ArcGISRuntime.Toolkit.Controller 100.13

import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/*!
 \qmltype BasemapGallery
 \inqmlmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiQmlViews
 \since Esri.ArcGISRuntime 100.12
 \brief The user interface for the BasemapGallery.

 The BasemapGallery displays a collection of items representing basemaps from either ArcGIS Online, a user-defined portal,
 or an array of Basemaps. When the user selects a basemap from the BasemapGallery, the  basemap rendered in the current
 geoModel is removed from the given map/scene and replaced with the basemap selected in the gallery.
 */

Pane {
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
      the \c GeoModel and the \c Portal where applicable.
    */
    property var controller: BasemapGalleryController { }

    /*!
       \qmlproperty GeoModel geoModel
       \brief The \c GeoModel for this tool. Should be a \c Scene or a \c Map.
     */
    property var geoModel;

    /*!
       \qmlproperty Portal portal
       \brief The \c Portal contains basemaps which will be fetched and displayed in the gallery if applicable.
       When a valid Portal is set then `Portal.fetchBasemaps` is immediately called.

       Note: Changing the current active portal will reset the contents of the gallery.
     */
    property var portal;

    onPortalChanged: {
        if (controller) {
            controller.portal = portal;
        }
    }

    /*!
       \qmlproperty GenericListModel gallery
        The list of basemaps currently visible in the gallery. Items added or removed from this
        list will update the gallery.
    */
    property var gallery;

    /*!
       \qmlproperty Basemap currentBasemap
       \brief Currently applied basemap on the associated \c GeoModel. This may be a basemap
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

    padding: 0

    Binding {
        restoreMode: Binding.RestoreNone
        target: controller
        property: "portal"
        value: portal
    }

    Binding {
        target: controller
        property: "geoModel"
        value: geoModel
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

    /*!
     \qmlmethod void BasemapGallery::setGeoModelFromGeoView(GeoView view)
     \brief Convenience function for QML/C++ users which allows the map/scene to be extracted from a
     SceneView or MapView assigned to \a view in QML code.

    This is only a concern as [Map/Scene]QuickView does not expose a [Map/Scene] property in QML.

     For example, to hook up BasemapGallery with a MapQuickView:

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
    function setGeoModelFromGeoView(view) {
        return controller.setGeoModelFromGeoView(view);
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
        delegate: ItemDelegate {
            id: basemapDelegate
            width: view.cellWidth
            height: view.cellHeight
            indicator: BusyIndicator {
                id: busyIndicator
                anchors.centerIn: parent
                running: false
                z: 1
            }
            enabled: controller.basemapMatchesCurrentSpatialReference(modelData.basemap)
            onClicked: controller.setCurrentBasemap(modelData.basemap)
            down: GridView.isCurrentItem

            Connections {
                target: controller
                function onCurrentBasemapChanged() { busyIndicator.running = false;}
            }
            icon {
                cache: false
                source: modelData.thumbnailUrl
                width: basemapGallery.internal.defaultCellSize
                height: basemapGallery.internal.defaultCellSize
                color: "transparent"
            }

            text: modelData.name === "" ? "Unnamed basemap" : modelData.name
            display: {
                if (basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.List) {
                    return AbstractButton.TextBesideIcon;
                } else if (basemapGallery.internal.calculatedStyle === BasemapGallery.ViewStyle.Grid) {
                    return AbstractButton.TextUnderIcon;
                }
            }
            Connections {
                target: basemapDelegate.ToolTip.toolTip.contentItem
                enabled: basemapDelegate.ToolTip.visible
                function onLinkActivated(link) {
                    Qt.openUrlExternally(link)
                }
            }
            ToolTip.text: modelData.tooltip
            MouseArea {
                id: mouseArea
                z : 2
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if(controller.currentBasemap !== modelData.basemap)
                        busyIndicator.running = true;
                    controller.setCurrentBasemap(modelData.basemap);
                }

                // When mouse enters thumbnail area, use timer to delay showing of tooltip.
                onEntered: {
                    // Create a definition for the showTooltipFn property of timerOnEntered
                    timerOnEntered.showTooltipFn = () => {
                        if (allowTooltips && mouseArea.containsMouse && modelData.tooltip !== "")
                        basemapDelegate.ToolTip.visible = true;
                    }
                    timerOnEntered.start();
                }

                // When mouse exits thumbnail area, use timer to delay hiding of tooltip.
                onExited: {
                    timerOnEntered.stop();
                    // Create a definition for the hideTooltipFn property of timerOnExited
                    timerOnExited.hideTooltipFn = () => {
                        basemapDelegate.ToolTip.visible = false;
                    }
                    timerOnExited.start();
                }
            }
        }
        highlightFollowsCurrentItem: false
    }

    Timer {
        id: timerOnEntered

        property var showTooltipFn: null;

        interval: Qt.styleHints.mousePressAndHoldInterval * 2
        repeat: false
        onTriggered: {
            if (showTooltipFn)
                showTooltipFn();
        }
    }

    Timer {
        id: timerOnExited

        property var hideTooltipFn: null;

        interval: Qt.styleHints.mousePressAndHoldInterval * 1.9
        repeat: false
        onTriggered: {
            if (hideTooltipFn)
                hideTooltipFn();
        }
    }

    property QtObject internal: QtObject {
        property int defaultCellSize: 100;

        property int defaultCellHeightGrid: defaultCellSize + 46;
        property int defaultCellWidthGrid: defaultCellSize + 56;

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
