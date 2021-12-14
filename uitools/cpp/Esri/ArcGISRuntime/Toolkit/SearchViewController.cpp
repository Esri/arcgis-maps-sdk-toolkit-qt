
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
#include "SearchViewController.h"

// Toolkit headers
#include "Internal/GeoViews.h"
#include "SmartLocatorSearchSource.h"

// ArcGISRuntime headers
#include <EnvelopeBuilder.h>
#include <GeometryEngine.h>
#include <Graphic.h>
#include <PictureMarkerSymbol.h>
#include <SuggestListModel.h>
#include <SymbolStyle.h>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    const char* DEFAULT_DEFAULT_PLACEHOLDER = "Find a place or address";
    const char* DEFAULT_LOCATOR_URL = "https://geocode-api.arcgis.com/arcgis/rest/services/World/GeocodeServer";

    /*
      Helper method to cast a GeoElement to a Graphic.
      We will improve the quality of this method when there becomes a better way to test the type of
      a GeoElement.
     */
    void addGeoElementToOverlay(GraphicsOverlay* overlay, GeoElement* element)
    {
      if (overlay)
      {
        if (auto graphic = dynamic_cast<Graphic*>(element))
        {
          overlay->graphics()->append(graphic);
        }
        else
        {
          qDebug() << "Failure in converting GeoElement to Graphic. Skipping.";
        }
      }
    }
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::SearchViewController
    \brief Performs searches and manages search state for a SearchView.
   */

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \enum Esri::ArcGISRuntime::Toolkit::SearchViewController::SearchResultMode
    \brief The result mode used by the controller to display results.
    \sa Esri::ArcGISRuntime::Toolkit::SearchViewController::resultMode
    \value Single Always select the best result, even if multiple results are available.
    \value Multiple Always display results as a list, even if only one result is available.
    \value Automatic Show results as a list if there are multiple, otherwise if there is only
    one found result then select that result.
  */

  /*!
    \brief Constructs a new SearchViewController object with a given \a parent.
   */
  SearchViewController::SearchViewController(QObject* parent) :
    QObject(parent),
    m_suggestions(new GenericListModel(&SearchSuggestion::staticMetaObject, this)),
    m_sources(new GenericListModel(&SearchSourceInterface::staticMetaObject, this)),
    m_results(new GenericListModel(&SearchResult::staticMetaObject, this)),
    m_defaultPlaceholder(DEFAULT_DEFAULT_PLACEHOLDER)
  {
    // Detect any and all changes to the current Query text, and update all the applicable
    // sources in the controller.
    connect(this, &SearchViewController::currentQueryChanged, this, &SearchViewController::onQueryChanged);

    // Detect the deletion of sources from the controller, and perform the cleanup code.
    connect(m_sources, &QAbstractItemModel::rowsAboutToBeRemoved, this, &SearchViewController::onSourcesRemoved);

    // Whenever a source is added, perform the connection setup.
    connect(m_sources, &QAbstractItemModel::rowsInserted, this, &SearchViewController::onSourcesAdded);
  }

  /*!
    \brief Destructor.
   */
  SearchViewController::~SearchViewController()
  {
  }

  /*!
    \brief Returns the \c GeoView as a \c QObject.
   */
  QObject* SearchViewController::geoView() const
  {
    return m_geoView;
  }

  /*!
    \brief Set the \c GeoView object this Controller uses. This will reset internal
    state related to the old GeoView, such as the active query location and area.

    Internally this is cast to a \c MapView or SceneView using \c qobject_cast,
        which is why the paremeter is of form \c QObject and not \c GeoView.

    \list
      \li \a geoView Object which must inherit from \c GeoView* and \c QObject*.
    \endlist
   */
  void SearchViewController::setGeoView(QObject* geoView)
  {
    if (geoView == m_geoView)
      return;

    if (m_geoView)
      disconnect(this, nullptr, m_geoView, nullptr);

    delete m_graphicsOverlay;
    m_graphicsOverlay = nullptr;

    m_geoView = geoView;

    if (isAutomaticConfigurationEnabled())
    {
      // Hard reset everything when geoView changes.
      setCurrentQuery({});
      setActiveSource(nullptr);
      setDefaultPlaceholder(DEFAULT_DEFAULT_PLACEHOLDER);
      // Reset sources to default.
      {
        sources()->clear();
        auto locatorTask = new LocatorTask(QUrl(DEFAULT_LOCATOR_URL), nullptr, nullptr);
        auto searchSource = new SmartLocatorSearchSource(locatorTask, m_sources);
        locatorTask->setParent(searchSource);
        sources()->append(searchSource);
      }
    }

    if (auto geoView = qobject_cast<GeoView*>(m_geoView))
    {
      m_graphicsOverlay = new GraphicsOverlay(this);
      geoView->graphicsOverlays()->append(m_graphicsOverlay);

      auto setViewpoint = [geoView, this]
      {
        //setting the lastsearcharea with the inital geoViewCast extent
        if (m_lastSearchArea.isEmpty())
          m_lastSearchArea = geoView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry();

        if (isAutomaticConfigurationEnabled())
        {
          auto vp = geoView->currentViewpoint(ViewpointType::BoundingGeometry);
          setQueryCenter(vp.targetGeometry().extent().center());
          setQueryArea(vp.targetGeometry());

          if (geoView->isNavigating())
          {
            // m_queryArea at this point is effectively the currentviewpoint geometry.
            if (checkZoomingDifferenceLastSearch(m_queryArea))
              setIsEligableForRequery(true);
            if (checkPanningDifferenceLastSearch(m_queryArea))
              setIsEligableForRequery(true);
          }
        }
      };

      if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        connect(mapView, &MapViewToolkit::viewpointChanged, this, setViewpoint);
        setViewpoint();
      }
      else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        connect(sceneView, &SceneViewToolkit::viewpointChanged, this, setViewpoint);
        setViewpoint();
      }
    }

    emit geoViewChanged();
  }

  /*!
    \brief Returns the string shown in the search view when no user query is entered.

    Default is "Find a place or address".
   */
  QString SearchViewController::defaultPlaceholder() const
  {
    return m_defaultPlaceholder;
  }

  /*!
    \brief Sets the string shown in the search view when no user query is entered
    to \a defaultPlaceholder.
   */
  void SearchViewController::setDefaultPlaceholder(QString defaultPlaceholder)
  {
    if (defaultPlaceholder == m_defaultPlaceholder)
      return;

    m_defaultPlaceholder = std::move(defaultPlaceholder);
    emit defaultPlaceholderChanged();
  }

  /*!
    \brief Returns the currently active search source.
    All sources are used if this property is null.
   */
  SearchSourceInterface* SearchViewController::activeSource() const
  {
    return m_activeSource;
  }

  /*!
    \brief Sets the currently active search source to \a activeSource.
    All sources are used if this property is null.
   */
  void SearchViewController::setActiveSource(SearchSourceInterface* activeSource)
  {
    if (m_activeSource == activeSource)
      return;

    m_activeSource = activeSource;
    emit activeSourceChanged();
  }

  /*!
    \brief Returns the current user-entered query.

    This property drives both suggestions and searches.
   */
  QString SearchViewController::currentQuery() const
  {
    return m_currentQuery;
  }

  /*!
    \brief Sets the current user-entered query to \a currentQuery.
    This should be updated by the view after every key press.

    This property drives both suggestions and searches.
   */
  void SearchViewController::setCurrentQuery(QString currentQuery)
  {
    if (m_currentQuery == currentQuery)
      return;

    m_currentQuery = std::move(currentQuery);
    emit currentQueryChanged();
  }

  /*!
    \brief Returns the search area to be used for the current query.

    Ignored in most queries, unless the \c{restrictToArea} flag is set to true when calling
    \l SearchViewController::commitSearch.

    This property should be updated as the user navigates the map/scene, or at minimum before calling
    \l SearchViewController::commitSearch.

    Defaults to a default constructed Geometry.
   */
  Geometry SearchViewController::queryArea() const
  {
    return m_queryArea;
  }

  /*!
    \brief Sets the search area to be used for the current query to \a queryArea.

    This property should be updated as the user navigates the map/scene, or at minimum
    before calling \l commitSearch.
   */
  void SearchViewController::setQueryArea(Geometry queryArea)
  {
    if (queryArea == m_queryArea)
      return;

    m_queryArea = std::move(queryArea);
    emit queryAreaChanged();
  }

  /*!
    \brief Returns the center for the search.

    This property is used to weigh results in favour of those results closest to the
    center. This allows results to be local to the active viewpoint.

    This should be updated by the view every time the user navigates the map.

    Defaults to a default constructed Point.
   */
  Point SearchViewController::queryCenter() const
  {
    return m_queryCenter;
  }

  /*!
    \brief Sets the center for the search to \a queryCenter.

    This should be updated by the view every time the user navigates the map.
   */
  void SearchViewController::setQueryCenter(Point queryCenter)
  {
    if (queryCenter == m_queryCenter)
      return;

    m_queryCenter = std::move(queryCenter);

    for (int i = 0; i <= m_sources->rowCount(); ++i)
    {
      auto source = m_sources->element<SearchSourceInterface>(m_sources->index((i)));
      if (source)
      {
        source->setPreferredSearchLocation(m_queryCenter);
      }
    }

    emit queryCenterChanged();
  }

  /*!
    \brief Returns the mode which dictates how results are displayed.

    With \l{Esri::ArcGISRuntime::Toolkit::SearchViewController::SearchResultMode}{SearchResultMode::Automatic},
    if only a single result is found then this is automatically set as the selected result, otherwise when the is more than
    one possible result then the \l results property is populated with all possible results.

    With \l{Esri::ArcGISRuntime::Toolkit::SearchViewController::SearchResultMode}{SearchResultMode::Multiple}, the \l results property is
    always populated with all found results (even if there is only 1 result in the list).

    With \l{Esri::ArcGISRuntime::Toolkit::SearchViewController::SearchResultMode}{SearchResultMode::Single}, even if multiple results are found,
    the best result is automatically set as the "selected" result and the \l results property is never populated.

    The default value is \l{Esri::ArcGISRuntime::Toolkit::SearchViewController::SearchResultMode}{SearchResultMode::Automatic}.
  */
  SearchViewController::SearchResultMode SearchViewController::resultMode() const
  {
    return m_resultMode;
  }

  /*!
    \brief Sets the mode which dictates how results are displayed to \a resultMode.
   */
  void SearchViewController::setResultMode(SearchResultMode resultMode)
  {
    if (resultMode == m_resultMode)
      return;

    m_resultMode = resultMode;
    emit resultModeChanged();
  }

  /*!
   \brief Returns the collection of found results.

    Defaults to an empty list.
   */
  GenericListModel* SearchViewController::results() const
  {
    return m_results;
  }

  /*!
   \brief Returns the selected result from the \l results collection.

    Defaults to null.

    \sa resultMode
   */
  SearchResult* SearchViewController::selectedResult() const
  {
    return m_selectedResult;
  }

  /*!
    \brief Sets the selected result from the \l results collection to \a selectedResult.

    Selecting a result will trigger changing the viewpoint of the geoView to that result
    if applicable.
   */
  void SearchViewController::setSelectedResult(SearchResult* selectedResult)
  {
    if (m_selectedResult == selectedResult)
      return;

    if (m_selectedResult)
      m_selectedResult->deleteLater();

    if (m_graphicsOverlay)
      m_graphicsOverlay->graphics()->clear();

    m_selectedResult = selectedResult;

    if (m_selectedResult)
    {
      // The SearchViewController takes control of the result, and we are going to delete the
      // remaining results from the model as there's no need to keep them hanging about any further.
      m_selectedResult->setParent(this);

      connect(m_selectedResult, &QObject::destroyed, this, &SearchViewController::selectedResultChanged);

      addGeoElementToOverlay(m_graphicsOverlay, m_selectedResult->geoElement());

      auto connection = std::make_shared<QMetaObject::Connection>();
      if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        // When the geoview changes, update the lastsearcharea
        *connection = connect(sceneView, &SceneViewToolkit::viewpointChanged, this, [sceneView, this, connection]()
                              {
                                disconnect(*connection);
                                auto extent = sceneView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                                m_lastSearchArea = extent;
                              });
        // Set sceneView viewpoint to where graphic is.
        sceneView->setViewpoint(m_selectedResult->selectionViewpoint(), 0);
      }
      else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        // When the geoview changes, update the lastsearcharea
        *connection = connect(mapView, &MapViewToolkit::viewpointChanged, this, [mapView, this, connection]()
                              {
                                disconnect(*connection);
                                auto extent = mapView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                                m_lastSearchArea = extent;
                              });
        // Set mapView callout and zoom to where graphic + callout are (if applicable.)
        mapView->calloutData()->setTitle(m_selectedResult->displayTitle());
        mapView->calloutData()->setDetail(m_selectedResult->displaySubtitle());
        mapView->calloutData()->setGeoElement(m_selectedResult->geoElement());
        mapView->calloutData()->setVisible(true);

        mapView->setViewpoint(m_selectedResult->selectionViewpoint(), 0);
      }
    }
    else
    {
      // Hide callout if we are unsetting the selectedResult.
      if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        mapView->calloutData()->setVisible(false);
      }
    }

    // Now that we own the selected result, we can clear the old ones out.
    m_results->clear();

    emit selectedResultChanged();
  }

  /*!
      \brief Returns the collection of search sources to be used.
   */
  GenericListModel* SearchViewController::sources() const
  {
    return m_sources;
  }

  /*!
      \brief Returns the collection of suggestion results from a query.
   */
  GenericListModel* SearchViewController::suggestions() const
  {
    return m_suggestions;
  }

  /*!
      \brief Returns whether the user can perform a requery.
      A requery will perform the search at the same location, but will only look at results
      that exist within the current extent, ignoring all other results.
   */
  bool SearchViewController::isEligableForRequery() const
  {
    return m_isEligableForRequery;
  }

  /*!
      \brief Sets whether the user can perform a requery to \a isEligableForRequery.
      When \l automaticConfigurationEnabled is \c true, and the user changes the viewpoint, this is automatically set to
      \c true. Clearing or changing the current query will set this property to \c false.
   */
  void SearchViewController::setIsEligableForRequery(bool isEligableForRequery)
  {
    if (isEligableForRequery == m_isEligableForRequery)
      return;

    m_isEligableForRequery = isEligableForRequery;
    emit isEligableForRequeryChanged();
  }

  /*!
    \brief Starts a search using the current query. When \a restrictToArea is \c true then the
    query will only look for results within the current \l queryArea.
   */
  void SearchViewController::commitSearch(bool restrictToArea)
  {
    m_suggestions->clear();
    m_results->clear();
    setIsEligableForRequery(false);

    auto queryRestrictionArea = restrictToArea ? queryArea() : Geometry{};
    for (int i = 0; i <= m_sources->rowCount(); ++i)
    {
      auto source = m_sources->element<SearchSourceInterface>(m_sources->index((i)));
      if (source)
      {
        auto connection = std::make_shared<QMetaObject::Connection>();
        //set the lastSearchArea after the geoView viewpoint has changed and only once.
        source->search(currentQuery(), queryRestrictionArea);
      }
    }
  }

  /*!
    \brief Starts a search using the given suggestion result \a searchSuggestion.
   */
  void SearchViewController::acceptSuggestion(SearchSuggestion* searchSuggestion)
  {
    SuggestResult r;
    SearchSourceInterface* source = nullptr;
    QString displayTitle;

    if (searchSuggestion != nullptr)
    {
      r = searchSuggestion->suggestResult();
      source = searchSuggestion->owningSource();
      displayTitle = searchSuggestion->displayTitle();
    }

    // Warning: Don't access the `searchSuggestion` beyond this point as it
    // deleted.
    m_suggestions->clear();
    m_results->clear();
    setIsEligableForRequery(false);

    if (searchSuggestion == nullptr || source == nullptr)
      return;

    source->search(r);
  }

  /*!
      \brief Clears the search.
      This will clear \l results , \l suggestions, \l selectedResult and \l currentQuery.
   */
  void SearchViewController::clearSearch()
  {
    setSelectedResult(nullptr);
    m_suggestions->clear();
    m_results->clear();
    setCurrentQuery({});
    setIsEligableForRequery(false);
  }

  /*!
   \brief Returns whether automatic configuration is enabled or not.
   Determines whether the view will update its configuration based on the attached
   geoview's geomodel automatically. This includes such properties as \l queryCenter,
   \l queryArea, and \l isEligableForRequery.

   Defaults to true.
   */
  bool SearchViewController::isAutomaticConfigurationEnabled() const
  {
    return m_isAutomaticConfigurationEnabled;
  }

  /*!
   \brief Sets whether automatic configuration is enabled to \a isAutomaticConfigurationEnabled.
   */
  void SearchViewController::setIsAutomaticConfigurationEnabled(bool isAutomaticConfigurationEnabled)
  {
    if (isAutomaticConfigurationEnabled == m_isAutomaticConfigurationEnabled)
      return;

    m_isAutomaticConfigurationEnabled = isAutomaticConfigurationEnabled;
    emit isAutomaticConfigurationEnabledChanged();
  }

  double SearchViewController::thresholdRatioRepeatSearch()
  {
    return m_thresholdRatioRepeatSearch;
  }

  void SearchViewController::setThresholdRatioRepeatSearch(double rate)
  {
    if (m_thresholdRatioRepeatSearch == rate)
      return;

    m_thresholdRatioRepeatSearch = rate;
    emit thresholdRatioRepeatSearchChanged();
  }

  /*!
    \internal
    \brief User has updated the query, update the sources.
   */
  void SearchViewController::onQueryChanged()
  {
    setIsEligableForRequery(false);
    const int rowCount = m_sources->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
      auto source = m_sources->element<SearchSourceInterface>(m_sources->index(i));
      if (source)
      {
        source->suggestions()->setSearchText(currentQuery());
      }
    }
  }

  /*!
    \internal
    \brief User has added one or more sources to the sources list, we hook up and listen to this source.
   */
  void SearchViewController::onSourcesAdded(const QModelIndex& parent, int firstSource, int lastSource)
  {
    if (parent.isValid())
      return;

    for (int i = firstSource; i <= lastSource; ++i)
    {
      auto source = m_sources->element<SearchSourceInterface>(m_sources->index(i));
      if (source)
      {
        // Handle search results coming from the source.
        connect(source, &SearchSourceInterface::searchCompleted, this, [this](QList<SearchResult*> results)
                {
                  if (results.empty())
                    return;
                  auto connection = std::make_shared<QMetaObject::Connection>();
                  //connect either to a scene or a map event changedviewpoint
                  if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
                  {
                    *connection = connect(mapView, &MapViewToolkit::viewpointChanged, this, [mapView, connection, this]()
                                          {
                                            disconnect(*connection);
                                            auto extent = mapView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                                            m_lastSearchArea = extent;
                                          });
                  }
                  else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
                  {
                    *connection = connect(sceneView, &SceneViewToolkit::viewpointChanged, this, [sceneView, connection, this]()
                                          {
                                            disconnect(*connection);
                                            auto extent = sceneView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                                            m_lastSearchArea = extent;
                                          });
                  }

                  // If only one result needs be applicable, automatically accept it, otherwise,
                  // add all results to the list model.
                  if (m_resultMode == SearchResultMode::Single ||
                      (results.size() == 1 && m_resultMode != SearchResultMode::Multiple))
                  {
                    auto it = std::begin(results);
                    // Take only first element.
                    auto f = *it++;
                    setSelectedResult(f);

                    // Discard remaining elements.
                    for (; it != std::end(results); ++it)
                    {
                      (*it)->deleteLater();
                    }
                  }
                  else
                  {
                    // Take ownership of all elements.
                    for (auto r : qAsConst(results))
                    {
                      r->setParent(m_results);
                      m_results->append(r);
                    }

                    // Render all possible results on the geoView.
                    if (m_graphicsOverlay)
                    {
                      m_graphicsOverlay->graphics()->clear();

                      for (auto r : qAsConst(results))
                      {
                        addGeoElementToOverlay(m_graphicsOverlay, r->geoElement());
                      }

                      if (auto geoView = qobject_cast<GeoView*>(m_geoView))
                      {
                        const auto extent = m_graphicsOverlay->extent();
                        EnvelopeBuilder b{extent};
                        b.expandByFactor(1.2); // Give some margins to the view.
                        geoView->setViewpoint(b.toEnvelope(), 0);
                      }
                    }
                  }
                });

        // Handle suggestion updates coming the source.
        auto suggestionModel = source->suggestions();
        connect(suggestionModel, &QAbstractItemModel::rowsInserted, this, [source, suggestionModel, this](const QModelIndex& parent, int firstSugggest, int lastSuggest)
                {
                  if (parent.isValid())
                    return;

                  const auto suggestResults = suggestionModel->suggestResults();
                  for (int i = firstSugggest; i <= lastSuggest; ++i)
                  {
                    // Wrap a SuggestResult in our SearchSuggestion type.
                    const auto suggestion = suggestResults.at(i);
                    auto searchSuggestion = new SearchSuggestion(m_suggestions);
                    searchSuggestion->setSuggestResult(suggestion);
                    searchSuggestion->setOwningSource(source);
                    m_suggestions->append(searchSuggestion);
                    // Remove suggestion from our aggregate mode if it removed from the current source's suggestion model.
                    // Note that we keep track of the "true" position in the model via a QPersistentModelIndex.
                    QPersistentModelIndex pIndex = m_suggestions->index(m_suggestions->rowCount() - 1);
                    connect(suggestionModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, [searchSuggestion, pIndex](const QModelIndex& parent, int first, int last)
                            {
                              if (parent.isValid())
                                return;

                              for (int i = first; i <= last; ++i)
                              {
                                if (i == pIndex.row())
                                {
                                  searchSuggestion->deleteLater();
                                }
                              }
                            });
                  }
                });
      }
    }
  }
  /*!
    \internal
    \brief User has removed one or more sources from the sources list, requiring cleanup.
   */
  void SearchViewController::onSourcesRemoved(const QModelIndex& parent, int firstSource, int lastSource)
  {
    if (parent.isValid())
      return;

    // Go through all removed sources.
    for (int i = firstSource; i <= lastSource; ++i)
    {
      auto source = m_sources->element<SearchSourceInterface>(m_sources->index(i));
      if (source)
      {
        // disconnect from sources and their suggestions
        disconnect(source, nullptr, this, nullptr);
        disconnect(source->suggestions(), nullptr, this, nullptr);

        // for any suggesion that comes from a removed source, remove that
        // suggestion from the list model.
        const int rowCount = m_suggestions->rowCount();
        if (rowCount > 0)
        {
          for (int j = rowCount - 1; j >= 0; --j)
          {
            auto index = m_suggestions->index(j);
            auto suggestion = m_suggestions->element<SearchSuggestion>(index);
            if (suggestion->owningSource() == source)
            {
              m_suggestions->removeRow(j);
            }
          }
        }
      }
    }
  }

  /*!
   * \internal
   * \brief Compares the last search viewpoint with the \a geom and checks they are not more different than a specified panning difference.
   * The percentage used for the check is the variable returned from \l thresholdRatioRepeatSearch
   */
  bool SearchViewController::checkPanningDifferenceLastSearch(Geometry& geom)
  {
    // Check center difference.
    double centerDiff = ArcGISRuntime::GeometryEngine::distance(m_lastSearchArea.extent().center(), geom.extent().center());
    double currentExtentAvg = (m_lastSearchArea.extent().width() + m_lastSearchArea.extent().height()) / 2;
    double threshold = currentExtentAvg * m_thresholdRatioRepeatSearch;
    return centerDiff > threshold;
  }

  /*!
   * \internal
   * \brief Compares the last search viewpoint with the \a geom and checks they are not more different than a specified zooming difference.
   * The percentage used for the check the variable returned from \l thresholdRatioRepeatSearch
   */
  bool SearchViewController::checkZoomingDifferenceLastSearch(Geometry& geom)
  {
    // Check extent difference.
    double widthDiff = abs(geom.extent().width() - m_lastSearchArea.extent().width());
    double heightDiff = abs(geom.extent().height() - m_lastSearchArea.extent().height());

    double widthThreshold = m_lastSearchArea.extent().width() * m_thresholdRatioRepeatSearch;
    double heightThreshold = m_lastSearchArea.extent().height() * m_thresholdRatioRepeatSearch;
    return widthDiff > widthThreshold || heightDiff > heightThreshold;
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::geoViewChanged()
    \brief Signal emitted when the \l geoView changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::defaultPlaceholderChanged()
    \brief Signal emitted when the \l defaultPlaceholder changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::activeSourceChanged()
    \brief Signal emitted when the \l activeSource changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::currentQueryChanged()
    \brief Signal emitted when the \l currentQuery changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::queryAreaChanged()
    \brief Signal emitted when the \l queryArea changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::queryCenterChanged()
    \brief Signal emitted when the \l queryCenter changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::resultModeChanged()
    \brief Signal emitted when the \l resultMode changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::selectedResultChanged()
    \brief Signal emitted when the \l selectedResult changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::isEligableForRequeryChanged()
    \brief Signal emitted when the \l isEligableForRequery property changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchViewController::isAutomaticConfigurationEnabledChanged()
    \brief Signal emitted when the \l isAutomaticConfigurationEnabled property changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::geoView
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::defaultPlaceholder
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::activeSource
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::currentQuery
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::resultMode
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::results
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::selectedResult
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::sources
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::suggestions
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::eligableForRequery
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchViewController::automaticConfigurationEnabled
   */
} // Toolkit
} // ArcGISRuntime
} // Esri
