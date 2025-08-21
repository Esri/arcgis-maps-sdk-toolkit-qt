// COPYRIGHT 2025 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com
/// \file PopupViewController.cpp

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
#ifndef QRT_DISABLE_DEPRECATED_WARNINGS
#define QRT_DISABLE_DEPRECATED_WARNINGS
#endif
#include "PopupViewController.h"

// Qt headers
#include <QAbstractListModel>
#include <QFuture>
#include <QDebug>

// Maps SDK headers
#include "AttachmentsPopupElement.h"
#include "FieldsPopupElement.h"
#include "MediaPopupElement.h"
#include "Popup.h"
#include "PopupDefinition.h"
#include "PopupElement.h"
#include "PopupExpressionEvaluation.h"
#include "PopupTypes.h"
#include "TextPopupElement.h"

// Toolkit headers
#include "AttachmentsPopupElementViewController.h"
#include "FieldsPopupElementViewController.h"
#include "MediaPopupElementViewController.h"
#include "TextPopupElementViewController.h"
#include "PopupElementViewItem.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

PopupViewController::PopupViewController(QObject* parent):
  QObject(parent),
  m_popupElementControllerModel(new GenericListModel(&PopupElementViewItem::staticMetaObject, this))
{
}

PopupViewController::~PopupViewController()
{
}

Popup* PopupViewController::popup() const
{
  return m_popup;
}

GenericListModel* PopupViewController::popupElementControllers() const
{
  return m_popupElementControllerModel;
}

void PopupViewController::setPopup(Popup* popup)
{
  if (m_popup == popup)
    return;

  if (m_popup)
  {
    disconnect(m_popup.data(), nullptr, this, nullptr);
    m_popupElementControllerModel->removeRows(0, m_popupElementControllerModel->rowCount());
  }

  m_popup = popup;

  if (m_popup)
    connect(m_popup.data(), &QObject::destroyed, this, &PopupViewController::popupChanged);

  m_popup->evaluateExpressionsAsync(this).then(this, [this](const QList<PopupExpressionEvaluation*>&)
  {
    for (auto element : m_popup->evaluatedElements())
    {
      switch (element->popupElementType())
      {
        case Esri::ArcGISRuntime::PopupElementType::TextPopupElement:
          m_popupElementControllerModel->append(
                new TextPopupElementViewController(static_cast<TextPopupElement*>(element), this, m_popup));
          break;
        case Esri::ArcGISRuntime::PopupElementType::FieldsPopupElement:
          m_popupElementControllerModel->append(
                new FieldsPopupElementViewController(static_cast<FieldsPopupElement*>(element), this, m_popup));
          break;
        case Esri::ArcGISRuntime::PopupElementType::AttachmentsPopupElement:
          m_popupElementControllerModel->append(
                new AttachmentsPopupElementViewController(static_cast<AttachmentsPopupElement*>(element), this, m_popup));
          break;
        case Esri::ArcGISRuntime::PopupElementType::MediaPopupElement:
          m_popupElementControllerModel->append(
                new MediaPopupElementViewController(static_cast<MediaPopupElement*>(element), this, m_popup));
          break;
        default:
          Q_UNIMPLEMENTED();
          break;
      }
    }
    emit popupChanged();
  });

  emit popupChanged();
  emit titleChanged();
}

QString PopupViewController::title() const
{
  return m_popup ? m_popup->title() : QString();
}

} // Esri::ArcGISRuntime::Toolkit
