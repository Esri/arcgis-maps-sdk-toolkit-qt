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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H

// Toolkit headers
#include "PopupElementViewItem.h"

// Qt headers
#include <QObject>

namespace Esri::ArcGISRuntime {

class TextPopupElement;
class PopupElement;

namespace Toolkit {

class TextPopupElementViewController : public PopupElementViewItem
{
  Q_OBJECT
  Q_PROPERTY(QString text READ text NOTIFY textPopupElementChanged)

public:
  explicit TextPopupElementViewController(QObject* parent = nullptr);
  explicit TextPopupElementViewController(TextPopupElement* textPopupElement,
                                          QObject* parent = nullptr);
  ~TextPopupElementViewController() override;

  QString text() const;

signals:
  void textPopupElementChanged();
};

} // namespace Toolkit
} // namespace Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_TEXTPOPUPELEMENTVIEWCONTROLLER_H
