
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
#include "SearchResult.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
    \class Esri::ArcGISRuntime::Toolkit::SearchResult
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  SearchResult::SearchResult(QObject* parent) :
    QObject(parent)
  {
  }

  SearchResult::~SearchResult()
  {
  }

  QString SearchResult::displayTitle() const
  {
    return m_displayTitle;
  }

  void SearchResult::setDisplayTitle(QString displayTitle)
  {
    if (displayTitle == m_displayTitle)
      return;

    m_displayTitle = std::move(displayTitle);
    emit displayTitleChanged();
  }

  QString SearchResult::displaySubtitle() const
  {
    return m_displaySubtitle;
  }

  void SearchResult::setDisplaySubtitle(QString displaySubtitle)
  {
    if (displaySubtitle == m_displaySubtitle)
      return;

    m_displaySubtitle = std::move(displaySubtitle);
    emit displaySubtitleChanged();
  }

  QUrl SearchResult::markerImageUrl() const
  {
    return m_markerImageUrl;
  }

  void SearchResult::setMarkerImageUrl(QUrl markerImageUrl)
  {
    if (markerImageUrl == m_markerImageUrl)
      return;

    m_markerImageUrl = std::move(markerImageUrl);
    emit markerImageUrlChanged();
  }

  SearchSourceInterface* SearchResult::owningSource()
  {
    return m_owningSource;
  }

  void SearchResult::setOwningSource(SearchSourceInterface* owningSource)
  {
    if (owningSource == m_owningSource)
      return;

    m_owningSource = owningSource;
    emit owningSourceChanged();
  }

  GeoElement* SearchResult::geoElement() const
  {
    return m_geoElement;
  }

  void SearchResult::setGeoElement(GeoElement* geoElement)
  {
    if (geoElement == m_geoElement)
      return;

    m_geoElement = geoElement;
    emit geoElementChanged();
  }

  Viewpoint SearchResult::selectionViewpoint() const
  {
    return m_selectionViewpoint;
  }

  void SearchResult::setSelectionViewpoint(Viewpoint selectionViewpoint)
  {
    if (selectionViewpoint.toJson() == m_selectionViewpoint.toJson())
      return;

    m_selectionViewpoint = std::move(selectionViewpoint);
    emit selectionViewpointChanged();
  }

} // Esri::ArcGISRuntime::Toolkit
