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
  Q_INVOKABLE explicit PopupViewController(QObject* parent = nullptr);

  ~PopupViewController();

  PopupManager* popupManager() const;

  void setPopupManager(PopupManager* popupManager);

  QAbstractListModel* displayFields() const;
  
  QAbstractListModel* attachments() const;

  QString title() const;

signals:

  void popupManagerChanged();

  void fieldCountChanged();

  void attachmentCountChanged();

private:
  int fieldCount() const;

  int attachmentCount() const;

private:
  QPointer<PopupManager> m_popupManager;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
