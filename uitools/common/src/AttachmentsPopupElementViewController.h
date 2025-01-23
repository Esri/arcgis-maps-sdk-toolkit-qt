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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_ATTACHMENTSPOPUPELEMENTVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_ATTACHMENTSPOPUPELEMENTVIEWCONTROLLER_H

// Qt headers
#include <QObject>

// Other headers
#include "Internal/GenericListModel.h"
#include "PopupElementViewItem.h"

namespace Esri::ArcGISRuntime {

class PopupElement;
class AttachmentsPopupElement;
class PopupMediaListModel;

namespace Toolkit {

class AttachmentsPopupElementViewController : public PopupElementViewItem
{
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY attachmentPopupElementChanged)
  Q_PROPERTY(QString description READ description NOTIFY attachmentPopupElementChanged)
  Q_PROPERTY(QAbstractListModel* popupAttachmentItems READ popupAttachmentItems NOTIFY attachmentPopupElementChanged)

public:
  explicit AttachmentsPopupElementViewController(AttachmentsPopupElement* attachmentsPopupElement,
                                           QObject* parent = nullptr);
  ~AttachmentsPopupElementViewController() override;

private:
  QString title() const;
  QString description() const;
  GenericListModel* popupAttachmentItems() const;

signals:
  void attachmentPopupElementChanged();

private:
  GenericListModel* m_popupAttachmentItems = nullptr;
};

} // namespace Toolkit
} // namespace Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_ATTACHMENTSPOPUPELEMENTVIEWCONTROLLER_H
