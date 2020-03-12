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


namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

class PopupViewController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(PopupManager* popupManager READ popupManager WRITE setPopupManager NOTIFY popupManagerChanged)
  Q_PROPERTY(QAbstractListModel* displayFields READ displayFields NOTIFY popupManagerChanged)
  Q_PROPERTY(int fieldCount READ fieldCount_ NOTIFY fieldCountChanged)
  Q_PROPERTY(QAbstractListModel* attachments READ attachments NOTIFY popupManagerChanged)
  Q_PROPERTY(int attachmentCount READ attachmentCount_ NOTIFY attachmentCountChanged)
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

  PopupManager* popupManager() const;

  void setPopupManager(PopupManager* popupManager);

  QAbstractListModel* displayFields() const;

  QAbstractListModel* attachments() const;

signals:
  void popupManagerChanged();
  void fieldCountChanged();
  void attachmentCountChanged();

private:
  int fieldCount_() const;
  int attachmentCount_() const;

private:
  PopupManager* m_popupManager = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
