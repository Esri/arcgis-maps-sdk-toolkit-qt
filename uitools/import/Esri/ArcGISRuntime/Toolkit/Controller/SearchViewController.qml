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
import Esri.ArcGISRuntime 100.13
import Esri.ArcGISRuntime.Toolkit 100.13
import QtQml.Models 2.15
import QtQuick 2.15

/*!
   \qmltype SearchViewController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.13
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief Performs searches and manages search state for a SearchView.
 */
QtObject {
    id: controller

    /*!
      \brief The result mode used by the controller to display results.
      \sa SearchViewController.resultMode
      \value Single Always select the best result, even if multiple results are available.
      \value Multiple Always display results as a list, even if only one result is available.
      \value Automatic Show results as a list if there are multiple, otherwise if there is only
      one found result then select that result.
    */
    enum SearchResultMode {
        Single,
        Multiple,
        Automatic
    }

    /*!
      \qmlproperty GeoView geoView
      \brief The geoView the controller may optionally use for setting results, and for
      updating the active search area and preferred search location.
     */
    property var geoView: null;

    /*!
      \qmlproperty string defaultPlaceholder
      \brief The string shown in the search view when no user query is entered.
      Default is "Find a place or address".
     */
    property string defaultPlaceholder: 'Find a place or address'

    /*!
      \qmlproperty var activeSource
      \brief The currently active search source.
      All sources are used if this property is null.
     */
    property var activeSource: null;

    /*!
      \qmlproperty string currentQuery
      \brief The current user-entered query.
      This property drives both suggestions and searches.
     */
    property string currentQuery;

    /*!
      \qmlproperty Geometry queryArea
      \brief  The search area to be used for the current query.

      Ignored in most queries, unless the \c{restrictToArea} flag is set to true when calling
      \l SearchViewController::commitSearch.

      This property should be updated as the user navigates the map/scene, or at minimum before calling
      \l SearchViewController::commitSearch.

      Defaults to null
     */
    property alias queryArea: internal.queryArea

    /*!
      \qmlproperty Point queryCenter
      \brief The center for the search.

      This property is used to weigh results in favour of those results closest to the
      center. This allows results to be local to the active viewpoint.

      This should be updated by the view every time the user navigates the map.

      Defaults to null.
     */
    property alias queryCenter: internal.queryCenter

    /*!
      \qmlproperty SearchViewController.SearchResultMode resultMode
      \brief The mode which dictates how results are displayed.

      With \c{Automatic},
      if only a single result is found, then this is automatically set as the selected result. Otherwise when there is more than
      one possible result then the \l results property is populated with all possible results.

      With \c{Multiple}, the \l results property is
      always populated with all found results (even if there is only 1 result in the list).

      With \c{Single}, even if multiple results are found,
      the best result is automatically set as the "selected" result and the \l results property is never populated.

      The default value is \c{Automatic}.
     */
    property int resultMode: SearchViewController.SearchResultMode.Automatic

    /*!
      \qmlproperty ListElement selectedResult
      \brief The selected result from the \l results collection.

      Defaults to null.

      \sa resultMode
     */
    property var selectedResult

    /*!
      \qmlproperty ListModel sources
      \brief The collection of search sources to be used.

      Each element is a ListElement with a single \c modelData
      field, which contains the search source object.
     */
    readonly property alias sources: internal.sources

    /*!
      \qmlproperty ListModel suggestions
      \brief The collection of suggestion results from a query.

      A suggestion is a ListElement with the following fields.

      \table
      \header
         \li name
         \li type
         \li description
      \row
        \li \c displayTitle
        \li \c string
        \li The suggestion heading.
      \row
        \li \c displaySubtitle
        \li \c string
        \li Optional suggestion subheading.
      \row
        \li \c collection
        \li \c bool
        \li Flag for if the suggestion is a boolean.
      \row
        \li \c markerImageUrl
        \li \c url
        \li URL of the image to display alongside suggestion.
      \row
        \li \c owningSource
        \li \c object
        \li The search-source this suggestion was created from.
      \row
        \li \c suggestion
        \li \c SuggestionResult
        \li The \c SuggestionResult that populated this \c ListElement.
      \endtable
     */
    readonly property alias suggestions: internal.suggestions

    /*!
      \qmlproperty ListModel results
      \brief The collection of search results from a query.

      A result is a ListElement with the following fields.

      \table
      \header
         \li name
         \li type
         \li description
      \row
        \li \c displayTitle
        \li \c string
        \li The result heading.
      \row
        \li \c displaySubtitle
        \li \c string
        \li Optional result subheading.
      \row
        \li \c markerImageUrl
        \li \c url
        \li URL of the image to display alongside result.
      \row
        \li \c owningSource
        \li \c object
        \li The search-source this result was created from.
      \row
        \li \c geoElement
        \li \c GeoElement
        \li The optional map location/graphic of this result.
      \row
        \li \c selectionViewpoint
        \li \c Envelope
        \li An area containing the result.
      \endtable
     */
    readonly property alias results: internal.results

    /*!
      \qmlproperty bool eligableForRequery
      \brief Flag for whether the user can perform a requery.
      A requery will perform the search at the same location, but will only look at results
      that exist within the current extent, ignoring all other results.
     */
    property bool eligableForRequery: false

    /*!
      \qmlproperty bool automaticConfigurationEnabled
      \brief Flag for whether automatic configuration is enabled or not.
       Determines whether the view will update its configuration based on the attached
       geoview's geomodel automatically. This includes such properties as \l queryCenter,
       \l queryArea, and \l eligableForRequery.

       Defaults to true.
     */
    property bool automaticConfigurationEnabled: true

    /*!
      \qmlmethod void SearchViewController::commitSearch(bool restrictToArea)
      \brief Starts a search using the current query. When \a restrictToArea is \c true then the
      query will only look for results within the current \l queryArea.
     */
    function commitSearch(restrictToArea) {
        internal.suggestions.clear();
        internal.results.clear();
        controller.eligableForRequery = false;

        // Go through all sources and start a search.
        const area = restrictToArea ? internal.queryArea : null;
        for (let i = 0; i < internal.sources.count; i++) {
            const source = internal.sources.get(i).modelData;
            if (source) {
                source.search(controller.currentQuery, area);
            }
        }
    }

    /*!
      \qmlmethod void SearchViewController::acceptSuggestion(ListElement searchSuggestion)
      \brief Starts a search using the given suggestion result \a searchSuggestion.

      At minimum the searchSuggestion needs two fields. `owningSource`, which points to the
      search source to perform the search, and `suggestion`, which is a \c{SuggestionResult}.
     */
    function acceptSuggestion(searchSuggestion) {
        const source = searchSuggestion.owningSource;
        const suggestion = searchSuggestion.suggestion;

        internal.suggestions.clear();
        internal.results.clear();
        controller.eligableForRequery = false;
        source.search(suggestion);
    }

    /*!
      \qmlmethod void SearchViewController::clearSearch()
      \brief Clears the search.
      This will clear \l results , \l suggestions, \l selectedResult and \l currentQuery.
     */
    function clearSearch() {
        controller.selectedResult = null;
        internal.suggestions.clear();
        internal.results.clear();
        controller.currentQuery = "";
        controller.eligableForRequery = false;
    }

    /*!
      \internal
     */
    property QtObject internal: QtObject {
        id: internal

        property GraphicsOverlay graphicsOverlay: null

        property Geometry queryArea: null

        property Point queryCenter: null

        property ListModel sources: ListModel {
            onRowsInserted: {
                for (let i = first; i <= last; ++i) {
                    const source = sources.get(i).modelData;
                    if (source) {
                        // Listen to the source changes in results/suggestions.
                        source.suggestions.suggestionsAdded.connect(internal.onSuggestionsAdded);
                        source.suggestions.suggestionsRemoved.connect(internal.onSuggestionsRemoved);
                        source.searchCompleted.connect(internal.onSourceSearchCompleted);
                    }
                }
            }
            onRowsAboutToBeRemoved: {
                for (let i = first; i <= last; i++) {
                    const source = source.get(i).modelData;
                    if (source) {
                        // Disconnect from source.
                        source.suggestions.suggestionsAdded.disconnect(internal.onSuggestionsAdded);
                        source.suggestions.suggestionsRemoved.disconnect(internal.onSuggestionsRemoved);
                        source.searchCompleted.disconnect(internal.onSourceSearchCompleted);
                        // Go through all results and remove those that come from this source.
                        if (internal.results.count > 0) {
                            for (let j = internal.results.count - 1; j >= 0; j--) {
                                const result = internal.results.get(j);
                                if (result && result.owningSource === source) {
                                    internal.results.remove(j);
                                }
                            }
                        }
                        // Go through all suggestions and remove those that come from this source.
                        if (internal.suggestions.count > 0) {
                            for (let k = internal.suggestions.count - 1; k >= 0; k--) {
                                const suggestion = internal.suggestions.get(k);
                                if (suggestion && suggestion.owningSource === source) {
                                    internal.suggestions.remove(k);
                                }
                            }
                        }
                    }
                }
            }
        }

        property ListModel suggestions: ListModel {
            function element(index) {
                return this.get(index.row);
            }
        }

        property ListModel results: ListModel {
            function element(index) {
                return this.get(index.row);
            }
        }

        property Connections geoViewConnections: Connections {
            target: controller.geoView
            function onViewpointChanged() {
                if (!controller.automaticConfigurationEnabled) {
                    return;
                }

                // update queryArea/queryCenter with the viewpoint change.
                if (geoView instanceof SceneView) {
                    const vp = geoView.currentViewpointCenter;
                    if (vp) {
                        internal.queryCenter = vp.center;
                        internal.queryArea = null;
                    }
                } else if (geoView instanceof MapView) {
                    const vp2 = geoView.currentViewpointExtent;
                    if (vp2) {
                        internal.queryCenter = vp2.extent.center;
                        internal.queryArea = vp2.extent;
                    }
                }

                // Show the requery button if the user manually navigated the view.
                if (geoView.navigating) {
                    controller.eligableForRequery = true;
                }
            }
        }

        property Component defaultWorldGeocoderSource: Component {
            LocatorSearchSource {
                locatorTask: LocatorTask {
                    url: 'https://geocode-api.arcgis.com/arcgis/rest/services/World/GeocodeServer'
                }
            }
        }

        function onSuggestionsAdded(source, first, last) {
            for (let i = first; i <= last; i++) {
                const suggestion = source.get(i);
                controller.suggestions.append(suggestion);
            }
        }

        function onSuggestionsRemoved(source, first, last) {
            if (controller.suggestions.count > 0) {
                for (let i = controller.suggestions.count - 1; i >= 0; i--) {
                    for (let j = last;j >= first; j--) {
                        const s1 = source.get(j);
                        const s2 = controller.suggestions.get(i);
                        // Test suggestion equallity. If suggestion matches then we remove it.
                        if (s1.displayTitle === s2.displayTitle &&
                                s1.displaySubtitle === s2.displaySubtitle &&
                                s1.collection === s2.collection &&
                                s1.owningSource === s2.owningSource) {
                            controller.suggestions.remove(i);
                            break;
                        }
                    }
                }
            }
        }

        function onSourceSearchCompleted(results) {
            if (results.length === 0) {
                return;
            }

            if (controller.resultMode === SearchViewController.SearchResultMode.Single
                    || results.length === 1 && controller.resultMode !== SearchViewController.SearchResultMode.Automatic) {
                // Add single result, discard the rest.
                controller.selectedResult = results[0];
            } else {
                // Add all results to results list.
                for (const r of results) {
                    internal.results.append(r);
                }
                // Add graphics to graphicsView and zoom to.
                if (internal.graphicsOverlay) {
                    internal.graphicsOverlay.graphics.clear();
                    for (const q of results) {
                        const geoElement = q.geoElement;
                        if (geoElement instanceof Graphic) {
                            internal.graphicsOverlay.graphics.append(geoElement);
                        }
                    }

                    if (controller.geoView) {
                        const extent = internal.graphicsOverlay.extent;
                        let builder = ArcGISRuntimeEnvironment.createObject('EnvelopeBuilder', {geometry: extent });
                        builder.expandByFactor(1.2);
                        let newViewpoint = ArcGISRuntimeEnvironment.createObject('ViewpointExtent', {extent: builder.geometry });
                        controller.geoView.setViewpointAndSeconds(newViewpoint, 0);
                    }
                }
            }
        }

        onQueryCenterChanged: {
            for (let i = 0; i < internal.sources.count; i++) {
                const source = internal.sources.get(i).modelData;
                if (source) {
                    // Update all sources with changes to queryCenter.
                    source.preferredSearchLocation = internal.queryCenter;
                }
            }
        }
    }

    onGeoViewChanged: {
        // Reset reverrything if geoView changes.
        if (controller.automaticConfigurationEnabled) {
            controller.currentQuery = "";
            controller.activeSource = null;
            controller.defaultPlaceholder =  'Find a place or address';
            internal.graphicsOverlay = null;
            internal.sources.clear();
            internal.sources.append({
                                        "modelData" : internal.defaultWorldGeocoderSource.createObject(this)
                                    });

            if (controller.geoView) {
                internal.graphicsOverlay = ArcGISRuntimeEnvironment.createObject('GraphicsOverlay', {}, internal);
                geoView.graphicsOverlays.append(internal.graphicsOverlay);
            }

        }
    }

    onCurrentQueryChanged: {
        controller.eligableForRequery = false;
        for (let i = 0; i < internal.sources.count; i++) {
            const source = internal.sources.get(i).modelData;
            if (source) {
                source.suggestions.searchText = controller.currentQuery;
            }
        }
    }

    onSelectedResultChanged: {
        if (internal.graphicsOverlay) {
            internal.graphicsOverlay.graphics.clear();
            if (controller.selectedResult) {
                const geoElement = controller.selectedResult.geoElement;
                if (geoElement instanceof Graphic) {
                    internal.graphicsOverlay.graphics.append(geoElement);
                }
            }
        }

        if (controller.geoView) {
            if (controller.geoView.calloutData !== undefined && controller.selectedResult !== null) {
                const g = controller.selectedResult.geoElement;
                controller.geoView.calloutData.title = controller.selectedResult.displayTitle;
                controller.geoView.calloutData.detail = controller.selectedResult.displaySubtitle;
                controller.geoView.calloutData.geoElement = g;
                controller.geoView.calloutData.visible = true;

                let newViewpoint = ArcGISRuntimeEnvironment.createObject(
                        'ViewpointExtent',
                        { extent: controller.selectedResult.selectionViewpoint });
                controller.geoView.setViewpointAndSeconds(
                            newViewpoint,
                            0);
            }
        }
    }
}
