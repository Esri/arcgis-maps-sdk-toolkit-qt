
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
#include "PopupViewController2.h"

#include <QDebug>
#include <QFuture>

#include "ErrorException.h"
#include "Popup.h"
#include "PopupExpressionEvaluation.h"

#include "PopupElement.h"
#include "PopupTypes.h"

#include "TextPopupElement.h"
#include "FieldsPopupElement.h"
#include "AttachmentsPopupElement.h"
#include "MediaPopupElement.h"

#include "TextPopupElementViewController.h"

namespace Esri::ArcGISRuntime::Toolkit {

PopupViewController2::PopupViewController2(QObject* parent)
    : QObject{parent},
    m_popupElementsModel(new GenericListModel(&TextPopupElementViewController::staticMetaObject, this))
{
}

Popup* PopupViewController2::popup() const
{
    return m_popup;
}

GenericListModel* PopupViewController2::popupElements() const
{
    return m_popupElementsModel;
}

void PopupViewController2::setPopup(Popup* popup)
{
    if (m_popup == popup)
        return;

    if (m_popup)
    {
        disconnect(m_popup.data(), nullptr, this, nullptr);
        m_popupElementsModel->removeRows(0, m_popupElementsModel->rowCount());
    }

    m_popup = popup;

    if (m_popup)
        connect(m_popup.data(), &QObject::destroyed, this, &PopupViewController2::popupChanged);

    m_popup->evaluateExpressionsAsync(this)
        .then([this](const QList<PopupExpressionEvaluation*>&)
              {
            for ( auto element : m_popup->evaluatedElements())
            {
                auto elementType = element->popupElementType();
                switch (elementType)
                {
                    case PopupElementType::TextPopupElement:       
                        m_popupElement = element;
                        m_popupElementsModel->append(new TextPopupElementViewController(element, this));
                        emit popupElementChanged();
                        break;
                    case PopupElementType::FieldsPopupElement:
                        break;
                    case PopupElementType::AttachmentsPopupElement:
                        break;
                    case PopupElementType::MediaPopupElement:
                        break;
                    default:
                        break;
                }
            }
        })
        .onFailed(this, [](const ErrorException& error) { qDebug() << "ErrorException: " << error.error().message(); });

    emit popupChanged();
}

QString PopupViewController2::title() const
{
    return m_popup ? m_popup->title() : nullptr;
}

QPointer<PopupElement> PopupViewController2::popupElement() const
{
    return m_popupElement;
}

} // namespace Esri::ArcGISRuntime::Toolkit
