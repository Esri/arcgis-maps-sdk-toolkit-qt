
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
#include "SmartLocatorSearchSource.h"

#include <QUuid>

#include "Internal/SingleShotConnection.h"

#include "GeocodeResult.h"
#include "SuggestListModel.h"
#include "SuggestResult.h"

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {
    constexpr int DEFAULT_REPEAT_SEARCH_RESULT_THRESHOLD = 1;
    constexpr int DEFAULT_REPEAT_SEARCH_SUGGEST_THRESHOLD = 6;
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::SmartLocatorSearchSource
    \brief Extends LocatorSearchSource with intelligent search behaviors; adds support for repeated search.
    \note Advanced functionality requires knowledge of the underlying locator to be used well; this class implements behaviors that
    make assumptions about the locator being the world geocode service.
   */

  /*!
    \brief Constructs a new SmartLocatorSearchSource object with a \a locatorTask and a given \a parent.

    \list
      \li \a locatorTask Task to use for performing searches. This should be the world geocode service, or a similarly-configured service.
    \endlist
   */
  SmartLocatorSearchSource::SmartLocatorSearchSource(LocatorTask* locatorTask, QObject* parent) :
    LocatorSearchSource(locatorTask, parent),
    m_repeatSearchResultThreshold{DEFAULT_REPEAT_SEARCH_RESULT_THRESHOLD},
    m_repeatSearchSuggestThreshold{DEFAULT_REPEAT_SEARCH_SUGGEST_THRESHOLD}
  {
    // If suggestion count is below are below our criteria, redo the suggestion search
    // with area removed.
    connect(suggestions(),
            &SuggestListModel::suggestCompleted,
            this,
            [this]
            {
              if (suggestions()->suggestParameters().searchArea().isEmpty())
              {
                return;
              }

              if (suggestions()->suggestResults().length() < repeatSuggestResultThreshold())
              {
                auto params = suggestions()->suggestParameters();
                params.setSearchArea(Geometry{});
                suggestions()->setSuggestParameters(params);
              }
            });
  }

  /*!
     \brief Destructor.
   */
  SmartLocatorSearchSource::~SmartLocatorSearchSource()
  {
  }

  /*!
    \brief Returns the minimum number of results to attempt to retrieve. If there are too few results, the search is repeated with
    loosened parameters until enough results are accumulated.

    \note If no search is successful, it is still possible to have a total number of results less than this threshold.
    Set to zero to disable search repeat behavior. Defaults to 1.
   */
  int SmartLocatorSearchSource::repeatSearchResultThreshold() const
  {
    return m_repeatSearchResultThreshold;
  }

  /*!
    \brief Sets the minimum number of results to attempt to retrieve
    to \a repeatSearchResultThreshold.
   */
  void SmartLocatorSearchSource::setRepeatSearchResultThreshold(int repeatSearchResultThreshold)
  {
    if (repeatSearchResultThreshold == m_repeatSearchResultThreshold)
      return;

    m_repeatSearchResultThreshold = repeatSearchResultThreshold;
    emit repeatSearchResultThresholdChanged();
  }

  /*!
    \brief Returns the minimum number of suggestions to attempt to retrieve. If there are too few results, the search is repeated with
    loosened parameters until enough suggestions are accumulated.

    \note If no search is successful, it is still possible to have a total number of suggestions less than this threshold.
    Set to zero to disable search repeat behavior. Defaults to 6.
   */
  int SmartLocatorSearchSource::repeatSuggestResultThreshold() const
  {
    return m_repeatSearchSuggestThreshold;
  }

  /*!
    \brief Sets the minimum number of suggestions to attempt to retrieve
    to \a repeatSearchSuggestThreshold.
   */
  void SmartLocatorSearchSource::setRepeatSuggestResultThreshold(int repeatSearchSuggestThreshold)
  {
    if (repeatSearchSuggestThreshold == m_repeatSearchSuggestThreshold)
      return;

    m_repeatSearchSuggestThreshold = repeatSearchSuggestThreshold;
    emit repeatSuggestResultThresholdChanged();
  }

  /*!
    \reimp
   */
  void SmartLocatorSearchSource::search(const SuggestResult& suggestion, const Geometry area)
  {
    // If area is specified, we listen in to the returned results and check to see if the number of
    // results meets our threshold. If the number of results do not meet our threshold we re-do
    // the search with the area constraint removed.
    if (!area.isEmpty() && repeatSearchResultThreshold() > 0)
    {
      singleShotConnection(locator(), &LocatorTask::geocodeCompleted, this, [this, suggestion, area](QUuid taskId, const QList<Esri::ArcGISRuntime::GeocodeResult>& geocodeResults)
                           {
                             // Warning: hack here. We can not detect in an async-friendly manner when a task is cancelled.
                             // Which means we have to disconnect on "any" callback (handled by the singleShotConnection), otherwise we will build up space leaks with repeated
                             // search-and-cancel behaviour.
                             // This is obviously not ideal if the locator is used for multiple concurrent searches, but this is
                             // an unlikely scenario.

                             if (m_searchTask.taskId() != taskId)
                               return;

                             if (geocodeResults.length() < repeatSearchResultThreshold())
                             {
                               auto params = geocodeParameters();
                               params.setMaxResults(params.maxResults() - geocodeResults.length());
                               m_searchTask = locator()->geocodeWithSuggestResultAndParameters(suggestion, params);
                             }
                           });
    }
    LocatorSearchSource::search(suggestion, area);
  }

  /*!
    \reimp
   */
  void SmartLocatorSearchSource::search(const QString& searchString, const Geometry area)
  {
    // If area is specified, we listen in to the returned results and check to see if the number of
    // results meets our threshold. If the number of results do not meet our threshold we re-do
    // the search with the area constraint removed.
    if (!area.isEmpty() && repeatSearchResultThreshold() > 0)
    {
      singleShotConnection(locator(), &LocatorTask::geocodeCompleted, this, [this, searchString, area](QUuid taskId, const QList<Esri::ArcGISRuntime::GeocodeResult>& geocodeResults)
                           {
                             // Warning: hack here. We can not detect in an async-friendly manner when a task is cancelled.
                             // Which means we have to disconnect on "any" callback, otherwise we will build up space leaks with repeated
                             // search-and-cancel behaviour.
                             // This is obviously not ideal if the locator is used for multiple concurrent searches, but this is
                             // an unlikely scenario.

                             if (m_searchTask.taskId() != taskId)
                               return;

                             if (geocodeResults.length() < repeatSearchResultThreshold())
                             {
                               auto params = geocodeParameters();
                               params.setMaxResults(params.maxResults() - geocodeResults.length());
                               m_searchTask = locator()->geocodeWithParameters(searchString, params);
                             }
                           });
    }
    LocatorSearchSource::search(searchString, area);
  }

} // Esri::ArcGISRuntime::Toolkit
