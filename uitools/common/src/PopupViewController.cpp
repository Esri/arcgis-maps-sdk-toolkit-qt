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
#include <QDebug>
#include <QFuture>

// Maps SDK headers
#include "AttachmentsPopupElement.h"
#include "AttributeListModel.h"
#include "FieldsPopupElement.h"
#include "GeoElement.h"
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
#include "PopupElementViewItem.h"
#include "TextPopupElementViewController.h"

namespace Esri::ArcGISRuntime::Toolkit
{

  /*!
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  PopupViewController::PopupViewController(QObject* parent) :
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

  void PopupViewController::disconnectAttributeModelSignal_()
  {
    if (m_attributeModelConnection)
    {
      disconnect(m_attributeModelConnection);
      m_attributeModelConnection = {};
    }
  }

  void PopupViewController::refreshPopupContent_()
  {
    if (!m_popup)
    {
      return;
    }

    m_popupElementControllerModel->removeRows(0, m_popupElementControllerModel->rowCount());

    m_popup->evaluateExpressionsAsync(this).then(this, [this](const QList<PopupExpressionEvaluation*>&)
    {
      if (!m_popup)
      {
        return;
      }

      for (auto element : m_popup->evaluatedElements())
      {
        switch (element->popupElementType())
        {
          case Esri::ArcGISRuntime::PopupElementType::TextPopupElement:
            m_popupElementControllerModel->append(new TextPopupElementViewController(static_cast<TextPopupElement*>(element), this, m_popup));
            break;
          case Esri::ArcGISRuntime::PopupElementType::FieldsPopupElement:
            m_popupElementControllerModel->append(new FieldsPopupElementViewController(static_cast<FieldsPopupElement*>(element), this, m_popup));
            break;
          case Esri::ArcGISRuntime::PopupElementType::AttachmentsPopupElement:
            m_popupElementControllerModel->append(
              new AttachmentsPopupElementViewController(static_cast<AttachmentsPopupElement*>(element), this, m_popup));
            break;
          case Esri::ArcGISRuntime::PopupElementType::MediaPopupElement:
            m_popupElementControllerModel->append(new MediaPopupElementViewController(static_cast<MediaPopupElement*>(element), this, m_popup));
            break;
          default:
            Q_UNIMPLEMENTED();
            break;
        }
      }

      emit popupChanged();
      emit titleChanged();
      emit editSummaryChanged();
    });
  }

  void PopupViewController::setPopup(Popup* popup)
  {
    if (m_popup == popup)
    {
      return;
    }

    if (m_popup)
    {
      disconnect(m_popup.data(), nullptr, this, nullptr);
      disconnectAttributeModelSignal_();
      m_popupElementControllerModel->removeRows(0, m_popupElementControllerModel->rowCount());
    }

    m_popup = popup;

    if (m_popup)
    {
      connect(m_popup.data(), &QObject::destroyed, this, &PopupViewController::popupChanged);

      if (auto* geoElement = m_popup->geoElement())
      {
        if (auto* attributes = geoElement->attributes())
        {
          // AttributeListModel currently forwards streamed attribute updates as begin/endResetModel, so we listen for model reset only.
          // If the attributes model starts emitting more fine grained signals for attribute updates in the future, this can be updated to listen for those.
          m_attributeModelConnection = connect(attributes, &QAbstractItemModel::modelReset, this, [this]()
          {
            refreshPopupContent_();
          });
        }
      }
    }

    refreshPopupContent_();

    emit popupChanged();
    emit titleChanged();
    emit editSummaryChanged();
  }

  QString PopupViewController::title() const
  {
    return m_popup ? m_popup->title() : QString();
  }

  QString PopupViewController::editSummary() const
  {
    return m_popup ? m_popup->editSummary() : QString{};
  }

} // namespace Esri::ArcGISRuntime::Toolkit
