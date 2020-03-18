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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H

// ArcGISRuntime headers
#include <PopupManager.h>

// Qt headers
#include <QAbstractListModel>
#include <QObject>
#include <QPointer>

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

/*!
 * \brief In MVC architecture, this is the controller for the corresponding
 * PopupView.
 * 
 * This controller is a thin wrapper around a PopupManager. It re-exposes some
 * PopupManager properties, including the number of total rows to render as a 
 * property.
 */
class PopupViewController : public QObject
{
  Q_OBJECT

  Q_PROPERTY(PopupManager* popupManager READ popupManager WRITE setPopupManager NOTIFY popupManagerChanged)
  Q_PROPERTY(QAbstractListModel* displayFields READ displayFields NOTIFY popupManagerChanged)
  Q_PROPERTY(int fieldCount READ fieldCount NOTIFY fieldCountChanged)
  Q_PROPERTY(QAbstractListModel* attachments READ attachments NOTIFY popupManagerChanged)
  Q_PROPERTY(int attachmentCount READ attachmentCount NOTIFY attachmentCountChanged)
  Q_PROPERTY(QString title READ title NOTIFY popupManagerChanged)

public:
  /*!
   * \brief Constructor
   * \param parent Parent owning QObject.
   */
  explicit Q_INVOKABLE PopupViewController(QObject* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~PopupViewController();

  /*!
   * \brief Returns the PopupManager that populates this controller with data.
   */
  PopupManager* popupManager() const;

  /*!
   * \brief Sets the PopupManager. Setting this will trigger a notify on all
   * remaining properties. 
   * \param popupManager To deliver data from.
   */
  void setPopupManager(PopupManager* popupManager);

  /*!
   * \brief Returns a list model containing the key/value fields of the
   * Popup associated with this PopupManager.
   * \note This can be null.
   */
  QAbstractListModel* displayFields() const;
  
  /*!
   * \brief Returns a list model containing the attachment images of the
   * Popup associated with this PopupManager. 
   * \note This can be null.
   */
  QAbstractListModel* attachments() const;

  /*!
   * \brief Returns the title of the PopupManager.
   */
  QString title() const;

signals:
  /*!
   * \brief Signal emitted when the PopupManager changes.
   */
  void popupManagerChanged();

  /*!
   * \brief Signal emitted when the number of rows in the displayFields 
   * list-model changes.
   */
  void fieldCountChanged();

  /*!
   * \brief Signal emitted when the number of rows in the attachments 
   * list-model changes.
   */
  void attachmentCountChanged();

private:
  /*!
   * \internal
   * Exposes the number of rows in the list model returned by `displayFields`.
   * This is a property for QML. In C++ code call `displayFields()->rowCount()`.
   */
  int fieldCount() const;

  /*!
   * \internal
   * Exposes the number of rows in the list model returned by `attachments`.
   * This is a property for QML. In C++ code call `attachments()->rowCount()`.
   */
  int attachmentCount() const;

private:
  QPointer<PopupManager> m_popupManager = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
