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
#include "SearchSourceInterface.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::SearchSourceInterface
    \brief Defines the contract for a search result provider.
   */

  /*!
    \brief Constructs a new SearchSourceInterface object with a given \a parent.
   */
  SearchSourceInterface::SearchSourceInterface(QObject* parent) :
    QObject(parent)
  {
  }

  /*!
    \brief Destructor.
   */
  SearchSourceInterface::~SearchSourceInterface()
  {
  }

  /*!
    \brief Returns the name to show when presenting this source in the UI.
   */
  QString SearchSourceInterface::displayName() const
  {
    return m_displayName;
  }

  /*!
    \brief Sets the name to show when presenting this source in the UI
    to \a displayName.
   */
  void SearchSourceInterface::setDisplayName(QString displayName)
  {
    if (displayName == m_displayName)
      return;

    m_displayName = std::move(displayName);
    emit displayNameChanged();
  }

  // Interface methods.

  /*!
    \fn int Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::maximumResults() const
    \brief Returns the maximum results to return when performing a search.

    Most sources default to 6.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::setMaximumResults(int maximumResults)
    \brief Sets the maximum results to return when performing a search to \a maximumResults.
   */

  /*!
    \fn int Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::maximumSuggestions() const
    \brief Returns the maximum suggestions to return when performing a search.

    Most sources default to 6.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::setMaximumSuggestions(int maximumSuggestions)
    \brief Sets the maximum suggestions to return when performing a search to \a maximumSuggestions.
   */

  /*!
    \fn Esri::ArcGISRuntime::Point Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::preferredSearchLocation() const
    \brief Returns the point to be used as an input to searches and suggestions.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::setPreferredSearchLocation(Esri::ArcGISRuntime::Point preferredSearchLocation)
    \brief Sets the point to be used as an input to searches and suggestions to \a preferredSearchLocation.
   */

  /*!
    \fn Esri::ArcGISRuntime::SuggestListModel* Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::suggestions() const
    \brief Returns suggestions.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::search(const Esri::ArcGISRuntime::SuggestResult& suggestion, Esri::ArcGISRuntime::Geometry area)
    \brief Gets search results using \a suggestion as input.

    If \a area is not default constructed, search is restricted to that area.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::search(const QString& query, Esri::ArcGISRuntime::Geometry area)
    \brief Gets search results using \a query as input.

    If \a area is not default constructed, search is restricted to that area.
   */

  // Signals

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::displayNameChanged()
    \brief Signal emitted when the \l displayName changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::searchCompleted(QList<Esri::ArcGISRuntime::Toolkit::SearchResult*> searchResults)
    \brief Signal emitted when the search completes, giving the found search results as \a searchResults.
   */

  // Properties

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchSourceInterface::displayName
   */

} // Esri::ArcGISRuntime::Toolkit
