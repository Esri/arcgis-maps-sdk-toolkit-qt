
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
#include "GeoViews.h"
#include "SingleShotConnection.h"
#include "SmartLocatorSearchSource.h"

#include <QFuture>

// ArcGISRuntime headers
#include <CalloutData.h>
#include <Envelope.h>
#include <EnvelopeBuilder.h>
#include <GeometryEngine.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <MapTypes.h>
#include <PictureMarkerSymbol.h>
#include <SuggestListModel.h>
#include <SymbolStyle.h>

namespace Esri::ArcGISRuntime::Toolkit {

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
    \class Esri::ArcGISRuntime::Toolkit::SearchViewController
    \internal
    This class is an internal implementation detail and is subject to change.
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

  SearchViewController::~SearchViewController()
  {
  }

  QObject* SearchViewController::geoView() const
  {
    return m_geoView;
  }

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

  QString SearchViewController::defaultPlaceholder() const
  {
    return m_defaultPlaceholder;
  }

  void SearchViewController::setDefaultPlaceholder(QString defaultPlaceholder)
  {
    if (defaultPlaceholder == m_defaultPlaceholder)
      return;

    m_defaultPlaceholder = std::move(defaultPlaceholder);
    emit defaultPlaceholderChanged();
  }

  SearchSourceInterface* SearchViewController::activeSource() const
  {
    return m_activeSource;
  }

  void SearchViewController::setActiveSource(SearchSourceInterface* activeSource)
  {
    if (m_activeSource == activeSource)
      return;

    m_activeSource = activeSource;
    emit activeSourceChanged();
  }

  QString SearchViewController::currentQuery() const
  {
    return m_currentQuery;
  }

  void SearchViewController::setCurrentQuery(QString currentQuery)
  {
    if (m_currentQuery == currentQuery)
      return;

    m_currentQuery = std::move(currentQuery);
    emit currentQueryChanged();
  }

  Geometry SearchViewController::queryArea() const
  {
    return m_queryArea;
  }

  void SearchViewController::setQueryArea(Geometry queryArea)
  {
    if (queryArea == m_queryArea)
      return;

    m_queryArea = std::move(queryArea);
    emit queryAreaChanged();
  }

  Point SearchViewController::queryCenter() const
  {
    return m_queryCenter;
  }

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

  SearchViewController::SearchResultMode SearchViewController::resultMode() const
  {
    return m_resultMode;
  }

  void SearchViewController::setResultMode(SearchResultMode resultMode)
  {
    if (resultMode == m_resultMode)
      return;

    m_resultMode = resultMode;
    emit resultModeChanged();
  }

  GenericListModel* SearchViewController::results() const
  {
    return m_results;
  }

  SearchResult* SearchViewController::selectedResult() const
  {
    return m_selectedResult;
  }

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

      if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
      {
        // When the geoview changes, update the lastsearcharea
        singleShotConnection(sceneView, &SceneViewToolkit::viewpointChanged, this, [sceneView, this]()
                             {
                               auto extent = sceneView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                               m_lastSearchArea = extent;
                             });
        // Set sceneView viewpoint to where graphic is.
        auto future = sceneView->setViewpointAsync(m_selectedResult->selectionViewpoint(), 0);
        Q_UNUSED(future)
      }
      else if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
      {
        // When the geoview changes, update the lastsearcharea
        singleShotConnection(mapView, &MapViewToolkit::viewpointChanged, this, [mapView, this]()
                             {
                               auto extent = mapView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                               m_lastSearchArea = extent;
                             });
        // Set mapView callout and zoom to where graphic + callout are (if applicable.)
        mapView->calloutData()->setTitle(m_selectedResult->displayTitle());
        mapView->calloutData()->setDetail(m_selectedResult->displaySubtitle());
        mapView->calloutData()->setGeoElement(m_selectedResult->geoElement());
        mapView->calloutData()->setVisible(true);

        auto future = mapView->setViewpointAsync(m_selectedResult->selectionViewpoint(), 0);
        Q_UNUSED(future)
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

  GenericListModel* SearchViewController::sources() const
  {
    return m_sources;
  }

  GenericListModel* SearchViewController::suggestions() const
  {
    return m_suggestions;
  }

  bool SearchViewController::isEligableForRequery() const
  {
    return m_isEligableForRequery;
  }

  void SearchViewController::setIsEligableForRequery(bool isEligableForRequery)
  {
    if (isEligableForRequery == m_isEligableForRequery)
      return;

    m_isEligableForRequery = isEligableForRequery;
    emit isEligableForRequeryChanged();
  }

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
        //set the lastSearchArea after the geoView viewpoint has changed and only once.
        source->search(currentQuery(), queryRestrictionArea);
      }
    }
  }

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

  void SearchViewController::clearSearch()
  {
    setSelectedResult(nullptr);
    m_suggestions->clear();
    m_results->clear();
    setCurrentQuery({});
    setIsEligableForRequery(false);
  }

  bool SearchViewController::isAutomaticConfigurationEnabled() const
  {
    return m_isAutomaticConfigurationEnabled;
  }

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
                  //connect either to a scene or a map event changedviewpoint
                  if (auto mapView = qobject_cast<MapViewToolkit*>(m_geoView))
                  {
                    singleShotConnection(mapView, &MapViewToolkit::viewpointChanged, this, [mapView, this]()
                                         {
                                           auto extent = mapView->currentViewpoint(ViewpointType::BoundingGeometry).targetGeometry().extent();
                                           m_lastSearchArea = extent;
                                         });
                  }
                  else if (auto sceneView = qobject_cast<SceneViewToolkit*>(m_geoView))
                  {
                    singleShotConnection(sceneView, &SceneViewToolkit::viewpointChanged, this, [sceneView, this]()
                                         {
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
                    for (auto r : std::as_const(results))
                    {
                      r->setParent(m_results);
                      m_results->append(r);
                    }

                    // Render all possible results on the geoView.
                    if (m_graphicsOverlay)
                    {
                      m_graphicsOverlay->graphics()->clear();

                      for (auto r : std::as_const(results))
                      {
                        addGeoElementToOverlay(m_graphicsOverlay, r->geoElement());
                      }

                      if (auto geoView = qobject_cast<GeoView*>(m_geoView))
                      {
                        const auto extent = m_graphicsOverlay->extent();
                        EnvelopeBuilder b{extent};
                        b.expandByFactor(1.2); // Give some margins to the view.
                        auto future = geoView->setViewpointAsync(b.toEnvelope(), 0);
                        Q_UNUSED(future)
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

  bool SearchViewController::checkPanningDifferenceLastSearch(const Geometry& geom) const
  {
    // Check center difference.
    double centerDiff = ArcGISRuntime::GeometryEngine::distance(m_lastSearchArea.extent().center(), geom.extent().center());
    double currentExtentAvg = (m_lastSearchArea.extent().width() + m_lastSearchArea.extent().height()) / 2;
    double threshold = currentExtentAvg * m_thresholdRatioRepeatSearch;
    return centerDiff > threshold;
  }

  bool SearchViewController::checkZoomingDifferenceLastSearch(const Geometry& geom) const
  {
    // Check extent difference.
    double widthDiff = abs(geom.extent().width() - m_lastSearchArea.extent().width());
    double heightDiff = abs(geom.extent().height() - m_lastSearchArea.extent().height());

    double widthThreshold = m_lastSearchArea.extent().width() * m_thresholdRatioRepeatSearch;
    double heightThreshold = m_lastSearchArea.extent().height() * m_thresholdRatioRepeatSearch;
    return widthDiff > widthThreshold || heightDiff > heightThreshold;
  }

} // Esri::ArcGISRuntime::Toolkit
