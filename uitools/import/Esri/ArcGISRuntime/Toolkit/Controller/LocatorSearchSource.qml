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
import Esri.ArcGISRuntime 100.15
import QtQml.Models 2.15

/*!
   \qmltype LocatorSearchSource
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.13
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief Uses a Locator to provide search and suggest results.

    \note Most configuration should be done on the GeocodeParameters directly.
 */
QtObject {
    id: locatorSearchSource

    /*!
      \qmlproperty string displayName
      \brief The name to show when presenting this source in the UI.
     */
    property string displayName: ""

    /*!
      \qmlproperty LocatorTask locatorTask
      \brief The locator used by this search source.
     */
    property LocatorTask locatorTask: null

    /*!
      \qmlproperty GeocodeParameters geocodeParameters
      \brief The parameters used for geocoding.
      Some properties on parameters will be updated automatically based on searches.
     */
    readonly property alias geocodeParameters: internal.geocodeParameters

    /*!
      \qmlproperty SuggestParameters suggestParameters
      \brief The parameters used for suggestions.
      Some properties on parameters will be updated automatically based on searches.
     */
    readonly property SuggestParameters suggestParameters:
        locatorTask ? locatorTask.suggestions ? locatorTask.suggestions.suggestParameters
                                              : null : null

    /*!
      \qmlproperty int maximumResults
      \brief The maximum results to return when performing a search.
      \note Defaults to 6.
     */
    property int maximumResults: 6

    /*!
      \qmlproperty int maximumSuggestions
      \brief The maximum suggestions to return when performing a search.
      \note Defaults to 6.
     */
    property int maximumSuggestions: 6

    /*!
      \qmlproperty Point preferredSearchLocation
      \brief The point to be used as an input to searches and suggestions.
     */
    property Point preferredSearchLocation: null

    /*!
      \qmlproperty ListModel suggestions
      \brief The collection of suggestion results.

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
    readonly property ListModel suggestions: ListModel {
        property string searchText: ""
        onSearchTextChanged: {
            if (locatorSearchSource.locatorTask && locatorSearchSource.locatorTask.suggestions) {
                locatorSearchSource.locatorTask.suggestions.searchText = searchText;
            }
        }

        signal suggestionsAdded(var source, int first, int last)
        onRowsInserted: suggestionsAdded(this, first, last)

        signal suggestionsRemoved(var source, int first, int last)
        onRowsAboutToBeRemoved: suggestionsRemoved(this, first, last)
    }

    /*!
      \qmlsignal LocatorSearchSource::searchCompleted(list searchResults)
      Signal emitted when the search completes, giving the found search results as \a searchResults.

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
    signal searchCompleted(var searchResults)

    /*!
      \qmlmethod void LocatorSearchSource::search(var query, Geometry area)
      \brief Starts a search using \a query as input.

      \a query can be a \c string or a \c SuggestResult.

      If \a area is non-null, search is restricted to that area.
     */
    function search(query, area) {
        if (!locatorTask) {
            searchCompleted([]);
            return;
        }
        if (typeof query === "string") {
            internal.search_with_method(locatorTask.geocodeWithParameters, query, area);
        } else if (typeof query === "object" && query instanceof SuggestResult) {
            internal.search_with_method(locatorTask.geocodeWithSuggestResultAndParameters, query, area);
        } else {
            throw `Bad query: ${query}`;
        }
    }

    /*!
        \internal
     */
    property QtObject internal: QtObject {
        id: internal

        property string currentSearchId: ""

        // We're battling the garbage collector here - it's buggy.
        // This object keeps references to the generated results and will be replaced
        // on each new subsequent search. This allows the Graphic and Extent of the results
        // to exist for the appropriate amount of time.
        property var tempParent: null

        property GeocodeParameters geocodeParameters: GeocodeParameters {
            maxResults: locatorSearchSource.maximumResults
            preferredSearchLocation: locatorSearchSource.preferredSearchLocation
        }

        // Can't find a cleaner way to copy. We could loop through all the keys
        // in the object - but due to the way QML is laid out we will also pick up all
        // inherited keys with no way to distinguish what class they came from.
        function copy_geocode_params() {
            const g = internal.geocodeParameters;
            return ArcGISRuntimeEnvironment.createObject("GeocodeParameters", {
                                                             categories: g.categories,
                                                             countryCode: g.countryCode,
                                                             forStorage: g.forStorage,
                                                             maxResults: g.maxResults,
                                                             minScore: g.minScore,
                                                             outputLanguageCode: g.outputLanguageCode,
                                                             outputSpatialReference: g.outputSpatialReference,
                                                             preferredSearchLocation: g.preferredSearchLocation,
                                                             resultAttributeNames: g.resultAttributeNames,
                                                             searchArea: g.searchArea
                                                         });
        }

        function search_with_method(search_method, querySuggest, area, maxResults) {
            // Reset the tempParent allowing for old geoElements and extents to be freed.
            internal.tempParent = { children: [] };

            const task = locatorSearchSource.locatorTask;
            if (internal.currentSearchId) {
                task.cancelTask(internal.currentSearchId);
            }

            const params = internal.copy_geocode_params();
            if (area) {
                params.preferredSearchLocation = null;
                params.searchArea = area;
            }
            if (maxResults) {
                params.maxResults = maxResults;
            }
            internal.currentSearchId = search_method(querySuggest, params);
        }

        property Component resultGeoElement: Component {
            Graphic {
                PictureMarkerSymbol {
                    url: 'qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin-tear.svg'
                }
            }
        }

        property Connections locatorTaskConnections: Connections {
            target: locatorSearchSource.locatorTask
            function onLoadStatusChanged() {
                const task = locatorSearchSource.locatorTask;
                if (task.loadStatus === Enums.LoadStatusLoaded) {
                    internal.initLocator();
                }
            }
            function onGeocodeStatusChanged() {
                const task = locatorSearchSource.locatorTask;

                if (task.geocodeStatus === Enums.TaskStatusErrored) {
                    locatorSearchSource.searchCompleted([]);
                    return;
                }

                if (task.geocodeStatus === Enums.TaskStatusCompleted) {
                    const results = [];
                    for (let i = 0; i < task.geocodeResults.length; i++) {
                        const g = task.geocodeResults[i];
                        // Be aware that every call to `g.extent` returns an underlying new object each time.
                        // Only refer to `extent` if you don't want garbage collection issues.
                        const extent = g.extent;
                        internal.tempParent.children.push(extent);

                        const geoElement = internal.resultGeoElement.createObject(null, {
                                                                                      geometry: g.displayLocation
                                                                                  });
                        internal.tempParent.children.push(geoElement);

                        Object.keys(g.attributes).forEach(k => {
                                                              geoElement.attributes.insertAttribute(k, g.attributes[k]);
                                                          });

                        results.push({
                                         displayTitle: g.label,
                                         displaySubtitle: g.attributes['LongLabel'] ?? `Match percent: ${g.score}`,
                                         markerImageUrl: 'qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin-tear.svg',
                                         owningSource: locatorSearchSource,
                                         geoElement: geoElement,
                                         selectionViewpoint: extent
                                     });
                    }
                    locatorSearchSource.searchCompleted(results);
                }
            }
        }

        property Connections suggestionsConnections: Connections {
            target: locatorSearchSource.locatorTask ? locatorSearchSource.locatorTask.suggestions : null
            function onRowsInserted(parent, first, last) {
                for (let i = first; i <= last; i++) {
                    const suggestion = locatorSearchSource.locatorTask.suggestions.get(i);
                    if (suggestion) {
                        const markerUrl = suggestion.isCollection ? 'qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/search.svg'
                                                                  : 'qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin.svg';
                        locatorSearchSource.suggestions.insert(i, {
                                                                   displayTitle: suggestion.label,
                                                                   displaySubtitle: "",
                                                                   collection: suggestion.isCollection,
                                                                   markerImageUrl: markerUrl,
                                                                   owningSource: locatorSearchSource,
                                                                   suggestion: suggestion
                                                               });
                    }
                }
            }
            function onRowsAboutToBeRemoved(parent, first, last) {
                for (let i = last; i >= first; i--) {
                    locatorSearchSource.suggestions.remove(i);
                }
            }
        }

        function initLocator() {
            const locator = locatorSearchSource.locatorTask;
            if (locator) {
                const info = locator.locatorInfo;
                if (info) {
                    // Update displayname to locator content.
                    if (info.name !== "") {
                        locatorSearchSource.displayName = info.name;
                    } else {
                        locatorSearchSource.displayName = info.description;
                    }

                    // Include the `Type` and `LongLabel` attributes in our results
                    // if applicable.
                    const resultAttrs = info.resultAttributes;
                    const attrs = ['Type', 'LongLabel'];
                    let foundOne = false;
                    for (let i = 0; i < resultAttrs.length; i++) {
                        const name = resultAttrs[i];
                        if (attrs.includes(name)) {
                            internal.geocodeParameters.resultAttributeNames.push(name);
                            foundOne = true;
                        }
                    }

                    // Otherwise return all attributes if we can't discover sensible defaults.
                    if (!foundOne) {
                        internal.geocodeParameters.resultAttributeNames.push("*");
                    }
                }
            }

            const params = locatorSearchSource.suggestParameters;
            if (params)  {
                params.preferredSearchLocation = locatorSearchSource.preferredSearchLocation;
                params.maxResults = locatorSearchSource.maximumSuggestions;
            }
        }
    }

    onPreferredSearchLocationChanged: {
        const params = locatorSearchSource.suggestParameters;
        if (params)  {
            params.preferredSearchLocation = locatorSearchSource.preferredSearchLocation;
        }
    }

    onMaximumSuggestionsChanged: {
        const params = locatorSearchSource.suggestParameters;
        if (params)  {
            params.maxResults = locatorSearchSource.maximumSuggestions;
        }
    }

    onLocatorTaskChanged: internal.initLocator()
}

