
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

#include <QAbstractListModel>
#include <QFuture>

#include "Popup.h"
#include "PopupAttachmentListModel.h"
#include "PopupAttachmentManager.h"
#include "PopupAttributeListModel.h"
#include "PopupElement.h"
#include "PopupExpressionEvaluation.h"
#include "PopupManager.h"
#include "PopupTypes.h"

#include "TextPopupElementViewController.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::PopupViewController
  \inmodule ArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief In MVC architecture, this is the controller for the corresponding
  \c PopupView. This class handles the
    management of the Different PopupElementViewController objects.

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
  QObject(parent),
  m_popupElementsModel(new GenericListModel(&TextPopupElementViewController::staticMetaObject, this))
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
  \brief Returns the \c Popup that populates this controller with data, this takes precedence over PopupManager.
 */
Popup* PopupViewController::popup() const
{
  return m_popup;
}

/*!
    \brief Returns the known list of available PopupElementViewControllers.
    Internally, this is a \c GenericListModel with an \c elementType of
    \c TextPopupElementViewController.
 */
GenericListModel* PopupViewController::popupElements() const
{
  return m_popupElementsModel;
}

/*!
  \brief Sets the \c Popup. Setting this will trigger evaluate expressions on the popup and notify the
  popup and title have chagned.
  \list
  \li \a popup To deliver data from.
  \endlist
 */
void PopupViewController::setPopup(Popup* popup)
{
  if (m_popup == popup)
      return;

  if (m_popup)
  {
      disconnect(m_popup.data(), nullptr, this, nullptr);
      m_popupElementsModel->removeRows(0, m_popupElementsModel->rowCount());
  }

  m_popup = popup;

  if (m_popup)
      connect(m_popup.data(), &QObject::destroyed, this, &PopupViewController::popupChanged);

  m_popup->evaluateExpressionsAsync(this)
      .then([this](const QList<PopupExpressionEvaluation*>&)
            {
                for (auto element : m_popup->evaluatedElements())
                {
                    auto elementType = element->popupElementType();
                    switch (elementType)
                    {
                    case PopupElementType::TextPopupElement:
                        m_popupElementsModel->append(new TextPopupElementViewController(element, m_popup));
                        break;
                    case PopupElementType::FieldsPopupElement:
                        break;
                    case PopupElementType::AttachmentsPopupElement:
                        break;
                    case PopupElementType::MediaPopupElement:
                        break;
                    default:
                        break;
                    }
                }
            });

  emit popupChanged();
  emit titleChanged();
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
    connect(attachments, &PopupAttachmentListModel::thumbnailWidthChanged , this, &PopupViewController::attachmentThumbnailWidthChanged);
    connect(attachments, &PopupAttachmentListModel::thumbnailHeightChanged , this, &PopupViewController::attachmentThumbnailHeightChanged);
  }

  if (auto displayFields = this->displayFields())
  {
    connect(displayFields, &QAbstractListModel::rowsInserted , this, &PopupViewController::fieldCountChanged);
    connect(displayFields, &QAbstractListModel::rowsRemoved , this, &PopupViewController::fieldCountChanged);
  }

  emit popupManagerChanged();
  emit titleChanged();
  emit fieldCountChanged();
  emit attachmentCountChanged();
  emit attachmentThumbnailHeightChanged();
  emit attachmentThumbnailWidthChanged();
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
PopupAttachmentListModel* PopupViewController::attachments() const
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
  \brief Returns whether attachments should be displayed or not
  according to the PopupManager.
 */
bool PopupViewController::isShowAttachments() const
{
  // This is re-exposed from PopupManager as PopupManager does not have
  // NOTIFY/CONSTANT modifiers on its showAttachments property, so the Controller
  // re-exposes showAttachments to suppress warnings about this.
  return m_popupManager ? m_popupManager->isShowAttachments() : false;
}

/*!
  \brief Returns the title of the \c PopupManager.
 */
QString PopupViewController::title() const
{
  // This is re-exposed from PopupManager as PopupManager does not have
  // NOTIFY/CONSTANT modifiers on its title property, so the Controller
  // re-exposes title to suppress warnings about this.
  return m_popup ? m_popup->title() : m_popupManager ? m_popupManager->title() : nullptr;
}

/*!
  \brief Returns the minimum attachment thumbnail width.
 */
int PopupViewController::attachmentThumbnailWidth() const
{
  auto attachmentModel = attachments();
  if (!attachmentModel)
    return 0;

  return attachmentModel->thumbnailWidth();
}

/*!
  \brief Sets the minimum attachment thumbnail width to \a width.
 */
void PopupViewController::setAttachmentThumbnailWidth(int width)
{
  auto attachmentModel = attachments();
  if (!attachmentModel)
    return;

  attachmentModel->setThumbnailWidth(width);
}

/*!
  \brief Returns the minimum attachment thumbnail width.
 */
int PopupViewController::attachmentThumbnailHeight() const
{
  auto attachmentModel = attachments();
  if (!attachmentModel)
    return 0;

  return attachmentModel->thumbnailHeight();
}

/*!
  \brief Sets the minimum attachment thumbnail height to \a height.
 */
void PopupViewController::setAttachmentThumbnailHeight(int height)
{
  auto attachmentModel = attachments();
  if (!attachmentModel)
    return;

  attachmentModel->setThumbnailHeight(height);
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::popupChanged()
  \brief Signal emitted when the \c Popup changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::popupManagerChanged()
  \brief Signal emitted when the \c PopupManager changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::titleChanged()
  \brief Signal emitted when the title changes.
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
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentThumbnailWidthChanged()
  \brief Signal emitted when the attachment minimum width changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentThumbnailHeightChanged()
  \brief Signal emitted when the attachment minimum height changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::showAttachments
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentThumbnailWidth
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::PopupViewController::attachmentThumbnailHeight
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

} // Esri::ArcGISRuntime::Toolkit
