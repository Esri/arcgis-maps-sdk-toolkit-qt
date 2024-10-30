
/*******************************************************************************
 *  Copyright 2012-2024 Esri
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

#include <QDebug>
#include <QUrl>
#include <QList>

// Maps SDK headers
#include <MediaPopupElement.h>
#include <PopupElement.h>
#include <PopupMedia.h>
#include <PopupMediaItem.h>
#include <PopupMediaValue.h>
#include <PopupMediaListModel.h>

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
   \li \a parent Parent owning \c QObject.
 \endlist
 */
MediaPopupElementViewController::MediaPopupElementViewController(QObject* parent)
  : PopupElementViewItem{parent}
{
}

/*!
  \brief Destructor.
  */
MediaPopupElementViewController::~MediaPopupElementViewController() = default;

/*!
\brief Constructor. Takes a \a mediaPopupElement and \a parent object.
  */
MediaPopupElementViewController::MediaPopupElementViewController(
    MediaPopupElement* mediaPopupElement, QObject* parent)
  : PopupElementViewItem{std::move(mediaPopupElement), parent},
    m_popupMediaItems{new GenericListModel(&PopupMediaItem::staticMetaObject, this)}
{
  // qDebug() << static_cast<MediaPopupElement*>(popupElement())->media()->size();
  // for ( auto media : *static_cast<MediaPopupElement*>(popupElement())->media() )
  // {
  //   qDebug() << this;
  //   if ( media->popupMediaType() == PopupMediaType::Image ) {
  //     qDebug() << "C++: " << media->value()->sourceUrl();
  //   }
  // }

  int i = 0;
  for (auto media: *static_cast<MediaPopupElement*>(popupElement())->media())
  {
    qDebug() << ++i;
    m_popupMediaItems->append(new PopupMediaItem(media, this));
  }
  qDebug() << "RowCount: " << m_popupMediaItems->rowCount();
}

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

PopupMediaListModel* MediaPopupElementViewController::media() const
{
  // qDebug() << "size: " << static_cast<MediaPopupElement*>(popupElement())->media()->size();
  return static_cast<MediaPopupElement*>(popupElement())->media();
}

QStringListModel* MediaPopupElementViewController::sourceUrls()
{
  if (m_sourceUrls)
  {
    return m_sourceUrls;
  }

  QStringList sourceUrls;
  for ( auto media: *static_cast<MediaPopupElement*>(popupElement())->media() )
  {
    // qDebug() << media->value()->sourceUrl().toString();
    sourceUrls.append(media->value()->sourceUrl().toString());
  }

  m_sourceUrls = new QStringListModel(sourceUrls, popupElement());
  // qDebug() << m_sourceUrls->roleNames();
  return m_sourceUrls;
}

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
