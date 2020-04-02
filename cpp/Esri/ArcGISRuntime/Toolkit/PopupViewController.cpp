/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#include "PopupViewController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::PopupViewController
  \inmodule ArcGISRuntimeToolkit
  \brief In MVC architecture, this is the controller for the corresponding
  \c PopupView.
  
  This controller is a thin wrapper around a \c PopupManager. It re-exposes some
  \c PopupManager properties, including the number of total rows to render as a 
  property.
 */

/*!
 \brief Constructor
 \list
   \li \a parent Parent owning \c QObject.
 \endlist
 */
PopupViewController::PopupViewController(QObject* parent):
  QObject(parent)
{
}

/*!
  \brief Destructor.
 */
PopupViewController::~PopupViewController()
{
}

/*!
  \brief Returns the \c PopupManager that populates this controller with data.
 */
PopupManager* PopupViewController::popupManager() const
{
  return m_popupManager;
}

/*!
  \brief Sets the \c PopupManager. Setting this will trigger a notify on all
  remaining properties.
  \list
  \li \a popupManager To deliver data from.
  \endlist
 */
void PopupViewController::setPopupManager(PopupManager* popupManager)
{
  if (popupManager == m_popupManager)
    return;

  if (m_popupManager)
    disconnect(m_popupManager.data(), nullptr, this, nullptr);

  if (auto attachments = this->attachments())
    disconnect(attachments, nullptr, this, nullptr);

  if (auto displayFields = this->displayFields())
    disconnect(displayFields, nullptr, this, nullptr);

  m_popupManager = popupManager;

  if (m_popupManager)
    connect(m_popupManager.data(), &QObject::destroyed, this, &PopupViewController::popupManagerChanged);

  if (auto attachments = this->attachments())
  {
    connect(attachments, &QAbstractListModel::rowsInserted , this, &PopupViewController::attachmentCountChanged);
    connect(attachments, &QAbstractListModel::rowsRemoved , this, &PopupViewController::attachmentCountChanged);
  }

  if (auto displayFields = this->displayFields())
  {
    connect(displayFields, &QAbstractListModel::rowsInserted , this, &PopupViewController::fieldCountChanged);
    connect(displayFields, &QAbstractListModel::rowsRemoved , this, &PopupViewController::fieldCountChanged);
  }

  emit popupManagerChanged();
  emit fieldCountChanged();
  emit attachmentCountChanged();
}

/*!
  \brief Returns a list model containing the key/value fields of the
  \c Popup associated with this PopupManager.
  \note This can be null.
 */
QAbstractListModel* PopupViewController::displayFields() const
{
  return m_popupManager ? m_popupManager->displayedFields() : nullptr;
}

/*!
  \brief Returns a list model containing the attachment images of the
  Popup associated with this PopupManager. 
  \note This can be null.
 */
QAbstractListModel* PopupViewController::attachments() const
{
  if (!m_popupManager)
    return nullptr;

  auto attachmentManager = m_popupManager->attachmentManager();
  if (!attachmentManager)
    return nullptr;

  return attachmentManager->attachmentsModel();
}

/*!
  \internal
  \brief Exposes the number of rows in the list model returned by 
  \c displayFields. This is a property for QML. In C++ code call 
  \c{displayFields()->rowCount()}.
 */
int PopupViewController::fieldCount() const
{
  if (auto displayFields = this->displayFields())
    return displayFields->rowCount();

  return 0;
}

/*!
  \internal
  \brief Exposes the number of rows in the list model returned by 
  \c attachments. This is a property for QML. In C++ code call 
  \c{attachments()->rowCount()}.
 */
int PopupViewController::attachmentCount() const
{
  if (auto attachments = this->attachments())
    return attachments->rowCount();

  return 0;
}

/*!
  \brief Returns the title of the \c PopupManager.
 */
QString PopupViewController::title() const
{
  // This is re-exposed from PopupManager as PopupManager does not have
  // NOTIFY/CONSTANT modifiers on its title property, so the Controller
  // re-exposes title to suppress warnings about this.
  return m_popupManager ? m_popupManager->title() : nullptr;
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::popupManagerChanged()
  \brief Signal emitted when the \c PopupManager changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::fieldCountChanged()
  \brief Signal emitted when the number of rows in the \c displayFields 
  list-model changes.
*/

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentCountChanged()
  \brief Signal emitted when the number of rows in the attachments list-model 
  changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::popupManager
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::displayFields
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::fieldCount
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::attachments
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentCount
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::title
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
