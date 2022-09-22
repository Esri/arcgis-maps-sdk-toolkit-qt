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
#include "LocatorSearchSource.h"

// Qt headers
#include <QVariant>
#include <QUuid>

// ArcGISRuntime headers
#include "Envelope.h"
#include "Error.h"
#include "GeocodeResult.h"
#include "Graphic.h"
#include "LocatorAttribute.h"
#include "LocatorInfo.h"
#include "PictureMarkerSymbol.h"
#include "SuggestListModel.h"
#include "SuggestResult.h"

// Toolkit headers
#include "Internal/DoOnLoad.h"
#include "SearchResult.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
  namespace {
    const char* MAP_PIN = "qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin-tear.svg";

    constexpr const int DEFAULT_MAXIMUM_RESULTS = 6;
    constexpr const int DEFAULT_MAXIMUM_SUGGESTIONS = 6;

    // Helper method that takes a GeocodeResult and the source that generated the result, and produces our UI-friendly
    // SearchResult.
    SearchResult* resultFromGeocodeResult(const GeocodeResult& g, SearchSourceInterface* owningSource)
    {
      auto searchResult = new SearchResult(owningSource);
      searchResult->setDisplayTitle(g.label());

      const auto attributes = g.attributes();
      searchResult->setDisplaySubtitle(QString("Match percent: %1").arg(g.score()));
      {
        auto f = attributes.find("LongLabel");
        if (f != attributes.end())
        {
          searchResult->setDisplaySubtitle(f.value().toString());
        }
      }

      searchResult->setOwningSource(owningSource);
      searchResult->setSelectionViewpoint(g.extent());
      searchResult->setMarkerImageUrl(QUrl{MAP_PIN});

      auto graphic = new Graphic(g.displayLocation(), g.attributes(), nullptr, searchResult);
      auto symbol = new PictureMarkerSymbol(QUrl{MAP_PIN}, graphic);
      QObject::connect(symbol, &PictureMarkerSymbol::doneLoading, symbol, [symbol](Error loadError)
                       {
                         if (!loadError.isEmpty())
                         {
                           qDebug() << loadError.message() << loadError.additionalMessage();
                           return;
                         }
                         symbol->setLeaderOffsetY(symbol->height() / 2);
                       });
      symbol->load();
      graphic->setSymbol(symbol);
      searchResult->setGeoElement(graphic);
      return searchResult;
    }

    // Takes a GeocodeParameters or a SuggestParameters \a params and returns a copy of the same with
    // \c{preferredSearchLocation} and \c{searchArea} set to area iff \a area is valid.
    template <typename Params>
    auto normalizeGeometryParams(Params params, Geometry area)
    {
      static_assert(std::is_same<Params, GeocodeParameters>::value || std::is_same<Params, SuggestParameters>::value,
                    "Must be a Geocodeparameters or SuggestParameters");

      if (!area.isEmpty())
      {
        params.setPreferredSearchLocation(Point{});
        params.setSearchArea(area);
      }
      return params;
    }
  }

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::LocatorSearchSource
    \brief Uses a Locator to provide search and suggest results.

    \note Most configuration should be done on the GeocodeParameters directly.
   */

  /*!
    \brief Constructs a new LocatorSearchSource object with a \a locatorTask and a given \a parent.
   */
  LocatorSearchSource::LocatorSearchSource(LocatorTask* locatorTask, QObject* parent) :
    SearchSourceInterface(parent),
    m_locatorTask(locatorTask)
  {
    Q_ASSERT(m_locatorTask != nullptr);
    LocatorSearchSource::setMaximumResults(DEFAULT_MAXIMUM_RESULTS);
    LocatorSearchSource::setMaximumSuggestions(DEFAULT_MAXIMUM_SUGGESTIONS);

    connect(m_locatorTask, &LocatorTask::geocodeCompleted, this, [this](QUuid taskId, const QList<Esri::ArcGISRuntime::GeocodeResult>& geocodeResults)
            {
              if (m_searchTask.taskId() != taskId)
                return;

              QList<SearchResult*> results;

              for (const auto& g : geocodeResults)
              {
                results << resultFromGeocodeResult(g, this);
              }

              emit searchCompleted(std::move(results));
            });

    doOnLoaded(locatorTask, this, [this]
             {
               const auto& info = m_locatorTask->locatorInfo();
               if (!info.name().isEmpty())
               {
                 setDisplayName(info.name());
               }
               else if (!info.description().isEmpty())
               {
                 setDisplayName(info.description());
               }

               QStringList attrs{"Type", "LongLabel"};
               auto resultAttrs = info.resultAttributes();
               auto geocodeAttrs = m_geocodeParameters.resultAttributeNames();
               bool foundOne{false};
               for (const auto& attr : attrs)
               {
                 auto it = std::find_if(std::cbegin(resultAttrs), std::cend(resultAttrs), [attr](const LocatorAttribute& la)
                                        {
                                          return la.name() == attr;
                                        });
                 if (it != std::cend(resultAttrs))
                 {
                   foundOne = true;
                   geocodeAttrs << it->name();
                 }
               }

               if (!foundOne)
               {
                 geocodeAttrs << "*";
               }

               m_geocodeParameters.setResultAttributeNames(geocodeAttrs);
             });
  }

  /*!
    \brief Destructor.
   */
  LocatorSearchSource::~LocatorSearchSource()
  {
  }

  /*!
      \brief Returns the locator used by this search source.
   */
  LocatorTask* LocatorSearchSource::locator()
  {
    return m_locatorTask;
  }

  /*!
      \brief Returns the parameters used for geocoding.
      Some properties on parameters will be updated automatically based on searches.
   */
  GeocodeParameters LocatorSearchSource::geocodeParameters() const
  {
    return m_geocodeParameters;
  }

  /*!
      \brief Returns the parameters used for suggestions.
      Some properties on parameters will be updated automatically based on searches.
   */
  SuggestParameters LocatorSearchSource::suggestParameters() const
  {
    return m_locatorTask->suggestions()->suggestParameters();
  }

  /*!
      \reimp
   */
  int LocatorSearchSource::maximumResults() const
  {
    return m_geocodeParameters.maxResults();
  }

  /*!
      \reimp
   */
  void LocatorSearchSource::setMaximumResults(int maximumResults)
  {
    m_geocodeParameters.setMaxResults(maximumResults);
  }

  /*!
      \reimp
   */
  int LocatorSearchSource::maximumSuggestions() const
  {
    return suggestParameters().maxResults();
  }

  /*!
      \reimp
   */
  void LocatorSearchSource::setMaximumSuggestions(int maximumSuggestions)
  {
    auto sg = suggestParameters();
    sg.setMaxResults(maximumSuggestions);
    m_locatorTask->suggestions()->setSuggestParameters(sg);
  }

  /*!
      \reimp
   */
  Point LocatorSearchSource::preferredSearchLocation() const
  {
    return m_geocodeParameters.preferredSearchLocation();
  }

  /*!
      \reimp
   */
  void LocatorSearchSource::setPreferredSearchLocation(Point preferredSearchLocation)
  {
    m_geocodeParameters.setPreferredSearchLocation(std::move(preferredSearchLocation));

    auto sg = suggestParameters();
    sg.setPreferredSearchLocation(m_geocodeParameters.preferredSearchLocation());
    m_locatorTask->suggestions()->setSuggestParameters(sg);
  }

  SuggestListModel* LocatorSearchSource::suggestions() const
  {
    return m_locatorTask->suggestions();
  }

  /*!
      \reimp
   */
  void LocatorSearchSource::search(const SuggestResult& suggestion, Geometry area)
  {
    m_searchTask.cancel();

    auto params = normalizeGeometryParams(m_geocodeParameters, area);
    m_searchTask = m_locatorTask->geocodeWithSuggestResultAndParameters(suggestion, params);
  }

  void LocatorSearchSource::search(const QString& searchString, Geometry area)
  {
    m_searchTask.cancel();

    auto params = normalizeGeometryParams(m_geocodeParameters, area);
    m_searchTask = m_locatorTask->geocodeWithParameters(searchString, params);
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
