
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

namespace Esri::ArcGISRuntime::Toolkit {

PopupViewController2::PopupViewController2(QObject* parent)
    : QObject{parent}
{}

Popup* PopupViewController2::popup() const
{
    return m_popup;
}

void PopupViewController2::setPopup(Popup* popup)
{
    if (m_popup == popup)
        return;

    if (m_popup)
        disconnect(m_popup.data(), nullptr, this, nullptr);

    m_popup = popup;

    if (m_popup)
        connect(m_popup.data(), &QObject::destroyed, this, &PopupViewController2::popupChanged);

    qDebug() << "(B)m_popup->evaluatedElements().length()" << m_popup->evaluatedElements().length();
    // what if this gets reset before it completes?
    m_popup->evaluateExpressionsAsync(this)
        .then([this](const QList<PopupExpressionEvaluation*>& res) {
            qDebug() << "res.length()" << res.length();
            qDebug() << "(A)m_popup->evaluatedElements().length()" << m_popup->evaluatedElements().length();
            for ( auto element : m_popup->evaluatedElements())
            {
//                qDebug() << element->popupElementType();
                auto elementType = element->popupElementType();
                switch (elementType)
                {
                    case PopupElementType::TextPopupElement:
                        qDebug() << "TextPopupElement";
                        qDebug() << dynamic_cast<TextPopupElement*>(element)->text();
                        break;
                    case PopupElementType::FieldsPopupElement:
                        qDebug() << "FieldsPopupElement";
                        qDebug() << dynamic_cast<FieldsPopupElement*>(element)->title();
                        break;
                    case PopupElementType::AttachmentsPopupElement:
                        qDebug() << "AttachmentsPopupElement";
                        qDebug() << dynamic_cast<AttachmentsPopupElement*>(element)->title();
                        break;
                    case PopupElementType::MediaPopupElement:
                        qDebug() << "MediaPopupElement";
                        qDebug() << dynamic_cast<MediaPopupElement*>(element)->title();
                        break;
                    default:
                        qDebug() << "Nothing";
                        break;
                }
            }
        })
        .onFailed(this, [](const ErrorException& error) { qDebug() << error.error().message(); });

    emit popupChanged();
}

QString PopupViewController2::title() const
{
    // This is re-exposed from PopupManager as PopupManager does not have
    // NOTIFY/CONSTANT modifiers on its title property, so the Controller
    // re-exposes title to suppress warnings about this.
    return m_popup ? m_popup->title() : nullptr;
}

} // namespace Esri::ArcGISRuntime::Toolkit
