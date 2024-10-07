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

// Qt headers
#include <QAbstractListModel>
#include <QObject>
#include <QPointer>

// STL headers
#include <Popup.h>
#include <PopupElement.h>
#include <PopupManager.h>

// Other headers
#include "Internal/GenericListModel.h"

Q_MOC_INCLUDE("PopupAttachmentListModel.h")

namespace Esri::ArcGISRuntime {

class PopupAttachmentListModel;
class PopupManager;

namespace Toolkit
{

class PopupViewController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Popup* popup READ popup WRITE setPopup NOTIFY popupChanged)
  Q_PROPERTY(QAbstractListModel* popupElementControllers READ popupElementControllers NOTIFY popupChanged)
  Q_PROPERTY(PopupManager* popupManager READ popupManager WRITE setPopupManager NOTIFY popupManagerChanged)
  Q_PROPERTY(QString title READ title NOTIFY titleChanged)
  Q_PROPERTY(QAbstractListModel* displayFields READ displayFields NOTIFY popupManagerChanged)
  Q_PROPERTY(int fieldCount READ fieldCount NOTIFY fieldCountChanged)
  Q_PROPERTY(QAbstractListModel* attachments READ attachments NOTIFY popupManagerChanged)
  Q_PROPERTY(int attachmentCount READ attachmentCount NOTIFY attachmentCountChanged)
  Q_PROPERTY(bool showAttachments READ isShowAttachments NOTIFY popupManagerChanged)
  Q_PROPERTY(int attachmentThumbnailWidth READ attachmentThumbnailWidth WRITE setAttachmentThumbnailWidth NOTIFY attachmentThumbnailWidthChanged)
  Q_PROPERTY(int attachmentThumbnailHeight READ attachmentThumbnailHeight WRITE setAttachmentThumbnailHeight NOTIFY attachmentThumbnailHeightChanged)

public:
  Q_INVOKABLE explicit PopupViewController(QObject* parent = nullptr);

  ~PopupViewController();

  Popup* popup() const;

  void setPopup(Popup* popup);

  GenericListModel* popupElementControllers() const;

  PopupManager* popupManager() const;

  void setPopupManager(PopupManager* popupManager);

  QAbstractListModel* displayFields() const;

  PopupAttachmentListModel* attachments() const;

  QString title() const;

  bool isShowAttachments() const;

  int attachmentThumbnailWidth() const;
  void setAttachmentThumbnailWidth(int width);

  int attachmentThumbnailHeight() const;
  void setAttachmentThumbnailHeight(int height);

signals:

  void popupChanged();

  void popupManagerChanged();

  void titleChanged();

  void fieldCountChanged();

  void attachmentCountChanged();

  void attachmentThumbnailWidthChanged();

  void attachmentThumbnailHeightChanged();

private:
  int fieldCount() const;

  int attachmentCount() const;

private:
  QPointer<PopupManager> m_popupManager;
  QPointer<Popup> m_popup;
  GenericListModel* m_popupElementControllerModel = nullptr;
};

} // Toolkit
} // Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
