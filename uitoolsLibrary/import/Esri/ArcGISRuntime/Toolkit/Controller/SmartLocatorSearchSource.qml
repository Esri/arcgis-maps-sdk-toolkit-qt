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

import QtQml
import Esri.ArcGISRuntime
import QtQml.Models

/*!
    \qmltype SmartLocatorSearchSource
    \inqmlmodule Esri.ArcGISRuntime.Toolkit
    \since Esri.ArcGISRuntime 100.13
    \ingroup ArcGISQtToolkitUiQmlControllers
    \brief Extends LocatorSearchSource with intelligent search behaviors; adds support for repeated search.

    \note Advanced functionality requires knowledge of the underlying locator to be used well; this class implements
    behaviors that make assumptions about the locator being the \l{https://developers.arcgis.com/rest/geocode/api-reference/overview-world-geocoding-service.htm}{ArcGIS World Geocoding Service}.
 */
LocatorSearchSource {
    id: smartLocatorSearchSource

    /*!
      \qmlproperty int repeatSearchResultThreshold
      \brief The minimum number of results to attempt to retrieve. If there are too few results, the search is repeated with
      loosened parameters until enough results are accumulated.

      \note If no search is successful, it is still possible to have a total number of results less than this threshold.
      Set to zero to disable search repeat behavior. Defaults to 1.
     */
    property int repeatSearchResultThreshold: 1

    /*!
      \qmlproperty int repeatSuggestResultThreshold
      \brief The minimum number of suggestions to attempt to retrieve. If there are too few results, the search is repeated with
      loosened parameters until enough suggestions are accumulated.

      \note If no search is successful, it is still possible to have a total number of suggestions less than this threshold.
      Set to zero to disable search repeat behavior. Defaults to 6.
     */
    property int repeatSuggestResultThreshold: 6

    /*!
        \internal
     */
    property QtObject internal2: QtObject {
        id: internal2

        property var cachedQuery: null
        property var requeryAttempted: false;

        property Connections suggestionsConnections : Connections {
            target: smartLocatorSearchSource.suggestions
            function suggestionsAdded(source, first, last) {
                if (last - first + 1 < repeatSuggestResultThreshold) {
                    // If we lack enough suggestions,
                    // trigger a new search without a searchArea.
                    suggestParameters.searchArea = null;
                }
            }
        }
    }

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

        internal2.cachedQuery = query;
        internal2.requeryAttempted = false;

        if (typeof query === "string") {
            internal.search_with_method(locatorTask.geocodeWithParameters, query, area);
        } else if (typeof query === "object" && query instanceof SuggestResult) {
            internal.search_with_method(locatorTask.geocodeWithSuggestResultAndParameters, query, area);
        } else {
            throw `Bad query: ${query}`;
        }
    }

    onSearchCompleted: {
        // If we lack enough search results, trigger a new search without a searchArea.
        const count = searchResults.length;
        if (count >= smartLocatorSearchSource.repeatSearchResultThreshold) {
            return;
        }

        if (!locatorTask) {
            return;
        }

        if (internal2.requeryAttempted) {
            return;
        }

        // Only do a search for the remaining results.
        const remaining = smartLocatorSearchSource.maximumResults - count;
        if (remaining <= 0) {
            return;
        }

        internal2.requeryAttempted = true;
        const query = internal2.cachedQuery;

        if (typeof query === "string") {
            internal.search_with_method(locatorTask.geocodeWithParameters, query, null, remaining);
        } else if (typeof query === "object" && query instanceof SuggestResult) {
            internal.search_with_method(locatorTask.geocodeWithSuggestResultAndParameters, query, null, remaining);
        } else {
            throw `Bad query: ${query}`;
        }
    }
}
