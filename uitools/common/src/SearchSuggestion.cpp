
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
    \class Esri::ArcGISRuntime::Toolkit::SearchSuggestion
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  SearchSuggestion::SearchSuggestion(QObject* parent) :
    QObject(parent)
  {
  }

  SearchSuggestion::~SearchSuggestion()
  {
  }

  QString SearchSuggestion::displayTitle() const
  {
    return m_suggestResult.label();
  }

  QString SearchSuggestion::displaySubtitle() const
  {
    return m_displaySubtitle;
  }

  void SearchSuggestion::setDisplaySubtitle(QString displaySubtitle)
  {
    if (displaySubtitle == m_displaySubtitle)
      return;

    m_displaySubtitle = std::move(displaySubtitle);
    emit displaySubtitleChanged();
  }

  QUrl SearchSuggestion::markerImageUrl() const
  {
    if (m_suggestResult.isEmpty())
    {
      return {};
    }
    else if (m_suggestResult.isCollection())
    {
      return QUrl{QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/search.svg")};
    }
    else
    {
      return QUrl{QStringLiteral("qrc:/Esri/ArcGISRuntime/Toolkit/pin.svg")};
    }
  }

  SearchSourceInterface* SearchSuggestion::owningSource()
  {
    return m_owningSource;
  }

  void SearchSuggestion::setOwningSource(SearchSourceInterface* owningSource)
  {
    if (owningSource == m_owningSource)
      return;

    m_owningSource = owningSource;
    emit owningSourceChanged();
  }

  const SuggestResult& SearchSuggestion::suggestResult() const
  {
    return m_suggestResult;
  }

  void SearchSuggestion::setSuggestResult(SuggestResult suggestResult)
  {
    if (suggestResult.isCollection() == m_suggestResult.isCollection() && suggestResult.label() == m_suggestResult.label())
      return;

    m_suggestResult = std::move(suggestResult);
    emit suggestResultChanged();
  }

  bool SearchSuggestion::isCollection() const
  {
    return m_suggestResult.isCollection();
  }

} // Esri::ArcGISRuntime::Toolkit
