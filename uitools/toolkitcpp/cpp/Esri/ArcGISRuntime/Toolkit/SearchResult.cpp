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
    \inmodule EsriArcGISRuntimeToolkit
    \class Esri::ArcGISRuntime::Toolkit::SearchResult
    \brief Wraps a Esri::ArcGISRuntime::GeocodeResult result for display.
   */

  /*!
    \brief Constructs a new SearchResult object with a given \a parent.
   */
  SearchResult::SearchResult(QObject* parent) :
    QObject(parent)
  {
  }

  /*!
    \brief Destructor.
   */
  SearchResult::~SearchResult()
  {
  }

  /*!
    \brief Returns the title that should be shown whenever a result is displayed.
   */
  QString SearchResult::displayTitle() const
  {
    return m_displayTitle;
  }

  /*!
    \brief Sets the title that should be shown whenever a result is displayed
    to \a displayTitle.
   */
  void SearchResult::setDisplayTitle(QString displayTitle)
  {
    if (displayTitle == m_displayTitle)
      return;

    m_displayTitle = std::move(displayTitle);
    emit displayTitleChanged();
  }

  /*!
    \brief Returns the optional subtitle that should be shown whenever a result is displayed.
   */
  QString SearchResult::displaySubtitle() const
  {
    return m_displaySubtitle;
  }

  /*!
    \brief Sets the optional subtitle that should be shown whenever a result is displayed
    to \a displaySubtitle.
   */
  void SearchResult::setDisplaySubtitle(QString displaySubtitle)
  {
    if (displaySubtitle == m_displaySubtitle)
      return;

    m_displaySubtitle = std::move(displaySubtitle);
    emit displaySubtitleChanged();
  }

  /*!
    \brief Returns the marker that would be shown on the map.
    This property is available as a convenience such that consumers need not worry whether the
    Esri::ArcGISRuntime::GeoElement is a graphic or a feature when displaying the icon in the UI.
   */
  QUrl SearchResult::markerImageUrl() const
  {
    return m_markerImageUrl;
  }

  /*!
    \brief Sets the marker that would be shown on the map to the image located at \a markerImageUrl.
    This property is available as a convenience. Calling this setter after construction
    will \e{not} update the geoElement image automatically.
   */
  void SearchResult::setMarkerImageUrl(QUrl markerImageUrl)
  {
    if (markerImageUrl == m_markerImageUrl)
      return;

    m_markerImageUrl = std::move(markerImageUrl);
    emit markerImageUrlChanged();
  }

  /*!
    \brief Returns pointer to the search source that created this result.
   */
  SearchSourceInterface* SearchResult::owningSource()
  {
    return m_owningSource;
  }

  /*!
    \brief Sets the pointer to the search source \a owningSource that created this result.
   */
  void SearchResult::setOwningSource(SearchSourceInterface* owningSource)
  {
    if (owningSource == m_owningSource)
      return;

    m_owningSource = owningSource;
    emit owningSourceChanged();
  }

  /*!
    \brief Returns the geo-element.

    For locator results, should be the graphic that was used to display the result on the map.
    For feature layer results, should be the result feature.
    Can be null depending on the type of the result, and can have \c{GeoElement}s without a defined geometry.
   */
  GeoElement* SearchResult::geoElement() const
  {
    return m_geoElement;
  }

  /*!
    \brief Sets the geo-element to \a geoElement.
   */
  void SearchResult::setGeoElement(GeoElement* geoElement)
  {
    if (geoElement == m_geoElement)
      return;

    m_geoElement = geoElement;
    emit geoElementChanged();
  }

  /*!
    \brief Returns the viewpoint to be used when the view zooms to a selected result.

    This property can be default-constructed because not all valid results will have a geometry.
    E.g. feature results from non-spatial features.
   */
  Viewpoint SearchResult::selectionViewpoint() const
  {
    return m_selectionViewpoint;
  }

  /*!
    \brief Sets the viewpoint to be used when the view zooms to a selected result
    to \a selectionViewpoint.
   */
  void SearchResult::setSelectionViewpoint(Viewpoint selectionViewpoint)
  {
    if (selectionViewpoint.toJson() == m_selectionViewpoint.toJson())
      return;

    m_selectionViewpoint = std::move(selectionViewpoint);
    emit selectionViewpointChanged();
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::displayTitleChanged()
    \brief Signal emitted when the \l displayTitleChanged changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::displaySubtitleChanged()
    \brief Signal emitted when the \l displaySubtitle changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::markerImageUrlChanged()
    \brief Signal emitted when the \l markerImageUrlChanged changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::owningSourceChanged()
    \brief Signal emitted when the \l owningSource changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::geoElementChanged()
    \brief Signal emitted when the \l geoElement changes.
   */

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::SearchResult::selectionViewpointChanged()
    \brief Signal emitted when the \l selectionViewpoint changes.
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchResult::displayTitle
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchResult::displaySubtitle
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchResult::markerImageUrl
   */

  /*!
    \property Esri::ArcGISRuntime::Toolkit::SearchResult::owningSource
   */

} // Esri::ArcGISRuntime::Toolkit
