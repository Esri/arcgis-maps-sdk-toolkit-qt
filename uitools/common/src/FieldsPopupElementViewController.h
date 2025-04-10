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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_FIELDSPOPUPELEMENTVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_FIELDSPOPUPELEMENTVIEWCONTROLLER_H

// Qt headers
#include <QObject>
#include <QVariantList>

// Other headers
#include "PopupElementViewItem.h"

namespace Esri::ArcGISRuntime {

class FieldsPopupElement;
class PopupElement;

namespace Toolkit {

class PopupViewController;

class FieldsPopupElementViewController : public PopupElementViewItem
{
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY fieldsPopupElementChanged)
  Q_PROPERTY(QVariantList labelsAndValues READ labelsAndValues NOTIFY fieldsPopupElementChanged)

public:
  explicit FieldsPopupElementViewController(FieldsPopupElement* fieldsPopupElement,
                                            PopupViewController* popupViewController,
                                            QObject* parent = nullptr);
  ~FieldsPopupElementViewController() override;

  QString title() const;
  QVariantList labelsAndValues() const;

signals:
  void fieldsPopupElementChanged();
  void clickedUrl(const QUrl& url);
};

} // namespace Toolkit
} // namespace Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FIELDSPOPUPELEMENTVIEWCONTROLLER_H
