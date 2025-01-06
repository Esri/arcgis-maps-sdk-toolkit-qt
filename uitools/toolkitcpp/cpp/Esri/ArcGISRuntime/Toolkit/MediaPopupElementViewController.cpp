/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
#include "MediaPopupElementViewController.h"

// Qt headers
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// Maps SDK headers
#include <MediaPopupElement.h>
#include <PopupMedia.h>
#include <PopupMediaListModel.h>
#include <PopupTypes.h>

// Toolkit headers
#include <PopupMediaItem.h>
#include <ImagePopupMediaItem.h>
#include <BarChartPopupMediaItem.h>
#include <PieChartPopupMediaItem.h>
#include <LineChartPopupMediaItem.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController
  \inmodule ArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief In MVC architecture, this is the controller for the corresponding
  \c MediaPopupElementView.
 */

/*!
  \brief Constructor
  \list
    \li \a mediaPopupElement - The \l {Esri::ArcGISRuntime::MediaPopupElement} {MediaPopupElement} used to populate the view.
    \li \a parent - The optional parent QObject.
  \endlist
 */
MediaPopupElementViewController::MediaPopupElementViewController(
    MediaPopupElement* mediaPopupElement, QObject* parent)
  : PopupElementViewItem{mediaPopupElement, parent},
    m_popupMediaItems{new GenericListModel(&PopupMediaItem::staticMetaObject, this)}
{
  auto* media = mediaPopupElement->media();
  for (int i = 0; i < media->size(); i++)
  {
    const QJsonDocument mediaPopupElementJson = QJsonDocument::fromJson(mediaPopupElement->toJson().toUtf8());
    auto* popupMedia = media->at(i);

    // Get color information for each popup media. If no color is selected when defining a popup, the default
    // color will be used from the appropriate QtGraphs Series component.
    // s.a. Web Map Spec: https://developers.arcgis.com/web-map-specification/objects/popupElement_media
    const auto colors = mediaPopupElementJson.object()["mediaInfos"][i]["value"]["colors"].toArray();

    switch (popupMedia->popupMediaType())
    {
      case Esri::ArcGISRuntime::PopupMediaType::Image:
        m_popupMediaItems->append(new ImagePopupMediaItem(popupMedia, media));
        break;
      case Esri::ArcGISRuntime::PopupMediaType::BarChart:
        m_popupMediaItems->append(new BarChartPopupMediaItem(popupMedia, colors, media));
        break;
      case Esri::ArcGISRuntime::PopupMediaType::ColumnChart:
        m_popupMediaItems->append(new BarChartPopupMediaItem(popupMedia, colors, media));
        break;
      case Esri::ArcGISRuntime::PopupMediaType::PieChart:
        m_popupMediaItems->append(new PieChartPopupMediaItem(popupMedia, colors, media));
        break;
      case Esri::ArcGISRuntime::PopupMediaType::LineChart:
        m_popupMediaItems->append(new LineChartPopupMediaItem(popupMedia, colors, media));
        break;
      case Esri::ArcGISRuntime::PopupMediaType::Unknown:
        Q_UNIMPLEMENTED();
        break;
    }
  }
}

/*!
  \brief Destructor.
  */
MediaPopupElementViewController::~MediaPopupElementViewController() = default;

/*!
  \brief Returns the description of the \c MediaPopupElement.
 */
QString MediaPopupElementViewController::description() const
{
  return static_cast<MediaPopupElement*>(popupElement())->description();
}

/*!
  \brief Returns the title of the \c MediaPopupElement.
 */
QString MediaPopupElementViewController::title() const
{
  return static_cast<MediaPopupElement*>(popupElement())->title();
}

/*!
  \brief Returns the known list of available PopupMedia.
  Internally, this is a \c GenericListModel with an \c elementType of
  \c PopupMediaItem.
 */
GenericListModel* MediaPopupElementViewController::popupMediaItems() const
{
  return m_popupMediaItems;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController::mediaPopupElementChanged()
  \brief Signal emitted when the underlying \c MediaPopupElement changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController::title
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::MediaPopupElementViewController::description
 */
