
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
#include "PopupAttachmentItem.h"

#include <PopupAttachment.h>

namespace Esri::ArcGISRuntime::Toolkit {

PopupAttachmentItem::PopupAttachmentItem(PopupAttachment* popupAttachment, QObject *parent)
  : QObject{parent}, m_popupAttachment{popupAttachment}
{
}

QString PopupAttachmentItem::name() const
{
  return m_popupAttachment->name();
}

QString PopupAttachmentItem::contentType() const
{
  return m_popupAttachment->contentType();
}

}
