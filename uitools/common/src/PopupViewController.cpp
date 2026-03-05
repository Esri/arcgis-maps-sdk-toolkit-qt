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

  namespace
  {
    bool isReusableElementType(PopupElementType type)
    {
      return type == PopupElementType::TextPopupElement || type == PopupElementType::FieldsPopupElement;
    }

    PopupElementViewItem* popupElementViewItemAt(GenericListModel* model, int index)
    {
      auto* object = model->element(model->index(index, 0));
      return qobject_cast<PopupElementViewItem*>(object);
    }

    bool canReuseControllersForElements(GenericListModel* model, const QList<PopupElement*>& evaluatedElements)
    {
      if (model->rowCount() != evaluatedElements.size())
      {
        return false;
      }

      for (int i = 0; i < evaluatedElements.size(); ++i)
      {
        auto* existingItem = popupElementViewItemAt(model, i);
        const auto newType = evaluatedElements.at(i)->popupElementType();

        if (!existingItem ||
            existingItem->popupElementType() != static_cast<QmlEnums::PopupElementType>(newType) ||
            !isReusableElementType(newType))
        {
          return false;
        }
      }

      return true;
    }

    void notifyControllerChanged(QObject* object, PopupElementType type)
    {
      switch (type)
      {
        case PopupElementType::TextPopupElement:
          QMetaObject::invokeMethod(object, "textPopupElementChanged", Qt::DirectConnection);
          break;
        case PopupElementType::FieldsPopupElement:
          QMetaObject::invokeMethod(object, "fieldsPopupElementChanged", Qt::DirectConnection);
          break;
        default:
          break;
      }
    }

    void appendControllerForElement(GenericListModel* model, PopupElement* element, PopupViewController* controller, Popup* popup)
    {
      switch (element->popupElementType())
      {
        case PopupElementType::TextPopupElement:
          model->append(new TextPopupElementViewController(static_cast<TextPopupElement*>(element), controller, popup));
          break;
        case PopupElementType::FieldsPopupElement:
          model->append(new FieldsPopupElementViewController(static_cast<FieldsPopupElement*>(element), controller, popup));
          break;
        case PopupElementType::AttachmentsPopupElement:
          model->append(new AttachmentsPopupElementViewController(static_cast<AttachmentsPopupElement*>(element), controller, popup));
          break;
        case PopupElementType::MediaPopupElement:
          model->append(new MediaPopupElementViewController(static_cast<MediaPopupElement*>(element), controller, popup));
          break;
        default:
          Q_UNIMPLEMENTED();
          break;
      }
    }
  } // namespace

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

    m_popup->evaluateExpressionsAsync(this).then(this, [this](const QList<PopupExpressionEvaluation*>&)
    {
      if (!m_popup)
      {
        return;
      }

      const auto evaluatedElements = m_popup->evaluatedElements();
      const bool canReuseControllers = canReuseControllersForElements(m_popupElementControllerModel, evaluatedElements);

      if (canReuseControllers)
      {
        for (int i = 0; i < evaluatedElements.size(); ++i)
        {
          auto* existingObject = m_popupElementControllerModel->element(m_popupElementControllerModel->index(i, 0));
          auto* existingItem = popupElementViewItemAt(m_popupElementControllerModel, i);
          auto* newElement = evaluatedElements.at(i);

          existingItem->setPopupElement(newElement);
          notifyControllerChanged(existingObject, newElement->popupElementType());
        }
      }
      else
      {
        m_popupElementControllerModel->removeRows(0, m_popupElementControllerModel->rowCount());

        for (auto* const element : evaluatedElements)
        {
          appendControllerForElement(m_popupElementControllerModel, element, this, m_popup);
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
