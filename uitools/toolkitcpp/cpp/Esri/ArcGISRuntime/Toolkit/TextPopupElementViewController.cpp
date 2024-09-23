
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
#include "TextPopupElementViewController.h"

//#include <QDebug>

namespace Esri::ArcGISRuntime::Toolkit {

TextPopupElementViewController::TextPopupElementViewController(QObject *parent)
    : QObject{parent}
{
}

QString TextPopupElementViewController::text() const
{
    return m_textPopupElement ? static_cast<TextPopupElement*>(m_textPopupElement.get())->text() : nullptr;
}

QPointer<PopupElement> TextPopupElementViewController::popupElement() const
{
    return m_textPopupElement;
}

void TextPopupElementViewController::setPopupElement(QPointer<PopupElement> popupElement)
{

    if (m_textPopupElement == popupElement)
        return;

    if (m_textPopupElement)
        disconnect(m_textPopupElement.data(), nullptr, this, nullptr);

    m_textPopupElement = popupElement;

    if (m_textPopupElement)
        connect(m_textPopupElement.data(), &QObject::destroyed, this, &TextPopupElementViewController::popupElementChanged);

    emit popupElementChanged();
}

} // namespace Esri::ArcGISRuntime::Toolkit
