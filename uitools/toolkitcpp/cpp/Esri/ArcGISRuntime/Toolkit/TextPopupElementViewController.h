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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H

// Qt headers
#include <QObject>
#include <QPointer>

// STL headers
#include <PopupElement.h>
#include <TextPopupElement.h>

namespace Esri::ArcGISRuntime {

class TextPopupElement;
class PopupElement;

namespace Toolkit
{

class TextPopupElementViewController : public QObject
{
    Q_PROPERTY(QPointer<PopupElement> popupElement READ popupElement WRITE setPopupElement NOTIFY popupElementChanged)
    Q_PROPERTY(QString text READ text NOTIFY popupElementChanged)

    Q_OBJECT
public:
    Q_INVOKABLE explicit TextPopupElementViewController(QObject *parent = nullptr);
    ~TextPopupElementViewController() = default;

    QString text() const;
    QPointer<PopupElement> popupElement() const;
    void setPopupElement(QPointer<PopupElement> popupElement);

signals:
    void popupElementChanged();

private:
    QPointer<PopupElement> m_textPopupElement;
};

} // Toolkit
} // Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H
