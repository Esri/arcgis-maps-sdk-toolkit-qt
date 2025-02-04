
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
#include "AttachmentsPopupElementViewController.h"

// Qt headers
#include <QFuture>

// Maps SDK headers
#include <Attachment.h>
#include <AttachmentsPopupElement.h>
#include <PopupMedia.h>
#include <PopupMediaListModel.h>
#include <PopupTypes.h>

// Toolkit headers
#include <PopupAttachmentItem.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */
AttachmentsPopupElementViewController::AttachmentsPopupElementViewController(
    AttachmentsPopupElement* attachmentsPopupElement, QObject* parent)
  : PopupElementViewItem{attachmentsPopupElement, parent},
    m_popupAttachmentItems{new GenericListModel(&PopupAttachmentItem::staticMetaObject, this)}
{
  qDebug() << attachmentsPopupElement->displayType();
  qDebug() << "attachments created";
  attachmentsPopupElement->fetchAttachmentsAsync().then([this] ()
  {
    QMetaObject::invokeMethod(this, "popuplateAttachments", Qt::QueuedConnection);
  });

  // attachmentsPopupElement->fetchAttachmentsAsync().then(
  //       [this](const QList<Attachment*>& attachments)
  //       {
  //       });

}

AttachmentsPopupElementViewController::~AttachmentsPopupElementViewController() = default;

QString AttachmentsPopupElementViewController::title() const
{
  return static_cast<AttachmentsPopupElement*>(popupElement())->title();
}

QString AttachmentsPopupElementViewController::description() const
{
  return static_cast<AttachmentsPopupElement*>(popupElement())->description();
}

GenericListModel* AttachmentsPopupElementViewController::popupAttachmentItems() const
{
  return m_popupAttachmentItems;
}

void AttachmentsPopupElementViewController::popuplateAttachments() {
  qDebug() << "attachments fetched";
  qDebug() << static_cast<AttachmentsPopupElement*>(popupElement())->attachments().size();

  qDebug() << "Current Thread: " << QThread::currentThread();
  qDebug() << "Parent Thread: " << this->thread();
  for (auto* popupAttachment : static_cast<AttachmentsPopupElement*>(popupElement())->attachments())
  {
    m_popupAttachmentItems->append(new PopupAttachmentItem(popupAttachment, this));

  }
  emit attachmentPopupElementChanged();
}

} // namespace Esri::ArcGISRuntime::Toolkit
