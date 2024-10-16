/*******************************************************************************
 *  Copyright 2012-2024 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPELEMENTVIEWITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPELEMENTVIEWITEM_H

// Qt headers
#include <QObject>
#include <QPointer>

// Maps SDK headers
#include <PopupElement.h>
#include <PopupTypes.h>

// Other headers
#include "Internal/QmlEnums.h"

namespace Esri::ArcGISRuntime {

class TextPopupElement;
class PopupElement;

namespace Toolkit
{

class PopupElementViewItem : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QmlEnums::PopupElementType popupElementType READ popupElementType NOTIFY popupElementChanged)
public:
  explicit PopupElementViewItem(QObject *parent = nullptr);
  explicit PopupElementViewItem(QPointer<PopupElement> popupElement, QObject *parent = nullptr);
  ~PopupElementViewItem();

  QmlEnums::PopupElementType popupElementType() const;
  PopupElement* popupElement() const;
  void setPopupElement(PopupElement* popupElement);

signals:
  void popupElementChanged();

private:
  QPointer<PopupElement> m_popupElement;

};

} // Toolkit
} // Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPELEMENTVIEWITEM_H
