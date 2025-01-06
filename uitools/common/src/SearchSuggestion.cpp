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
#include "SearchSuggestion.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \inmodule Esri.ArcGISRuntime.Toolkit
    \class Esri::ArcGISRuntime::Toolkit::SearchSuggestion
    \brief Wraps a Esri::ArcGISRuntime::SuggestResult result for display.
   */

  /*!
    \brief Constructs a new SearchSuggestion object with a given \a parent.
   */
  SearchSuggestion::SearchSuggestion(QObject* parent) :
    QObject(parent)
  {
  }

  /*!
    \brief Destructor.
   */
  SearchSuggestion::~SearchSuggestion()
  {
  }

  /*!
    \brief Returns the title that should be shown whenever a suggestion is displayed.
   */
  QString SearchSuggestion::displayTitle() const
  {
    return m_suggestResult.label();
  }

  /*!
    \brief Returns the optional subtitle that should be shown whenever a suggestion is displayed.
   */
  QString SearchSuggestion::displaySubtitle() const
  {
    return m_displaySubtitle;
  }

  /*!
    \brief Sets the optional subtitle that should be shown whenever a suggestion is displayed
    to \a displaySubtitle.
   */
  void SearchSuggestion::setDisplaySubtitle(QString displaySubtitle)
  {
    if (displaySubtitle == m_displaySubtitle)
      return;

    m_displaySubtitle = std::move(displaySubtitle);
    emit displaySubtitleChanged();
  }

  /*!
    \brief Returns the marker that would be shown in the UI.
   */
  QUrl SearchSuggestion::markerImageUrl() const
  {
    if (m_suggestResult.isEmpty())
    {
      return {};
    }
    else if (m_suggestResult.isCollection())
    {
      return QUrl{QStringLiteral("qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/search.svg")};
    }
    else
    {
      return QUrl{QStringLiteral("qrc:///esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin.svg")};
    }
  }

  /*!
    \brief Returns pointer to the search source that created this result.
   */
  SearchSourceInterface* SearchSuggestion::owningSource()
  {
    return m_owningSource;
  }

  /*!
    \brief Sets the pointer to the search source that created this result
    to \a owningSource.
   */
  void SearchSuggestion::setOwningSource(SearchSourceInterface* owningSource)
  {
    if (owningSource == m_owningSource)
      return;

    m_owningSource = owningSource;
    emit owningSourceChanged();
  }

  /*!
    \brief Returns the underling Esri::ArcGISRuntime::SuggestResult.
   */
  const SuggestResult& SearchSuggestion::suggestResult() const
  {
    return m_suggestResult;
  }

  /*!
    \brief Sets the underling Esri::ArcGISRuntime::SuggestResult to \a suggestResult.

    This will also update the \l isCollection and \l displayTitle properties.
   */
  void SearchSuggestion::setSuggestResult(SuggestResult suggestResult)
  {
    if (suggestResult.isCollection() == m_suggestResult.isCollection() && suggestResult.label() == m_suggestResult.label())
      return;

    m_suggestResult = std::move(suggestResult);
    emit suggestResultChanged();
  }

  /*!
    \brief Returns \c true if the search from this suggestion should be treated like a collection search,
    \c false if the search would return a single result.

    This property should be used to display a different icon in the UI depending on if this is a category search
    (like 'Coffee', 'Pizza', or 'Starbucks') and \c false if it is a search for a specific result
    (e.g. '380 New York St. Redlands CA').
   */
  bool SearchSuggestion::isCollection() const
  {
    return m_suggestResult.isCollection();
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSuggestion::displaySubtitleChanged()
    \brief Signal emitted when the \l displaySubtitle changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSuggestion::markerImageUrlChanged()
    \brief Signal emitted when the \l markerImageUrlChanged changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSuggestion::owningSourceChanged()
    \brief Signal emitted when the \l owningSource changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSuggestion::suggestResultChanged()
    \brief Signal emitted when the \l suggestResult changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSuggestion::displayTitle
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSuggestion::displaySubtitle
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSuggestion::markerImageUrl
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSuggestion::owningSource
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSuggestion::collection
   */
} // Esri::ArcGISRuntime::Toolkit
