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
#include <QAbstractListModel>
#include <QObject>
#include <QPointer>

// STL headers
#include <FieldsPopupElement.h>
#include <PopupElement.h>
#include <PopupFieldListModel.h>

// Other headers
#include "PopupElementViewItem.h"

namespace Esri::ArcGISRuntime {

class FieldsPopupElement;
class PopupElement;

namespace Toolkit {

class FieldsPopupElementViewController : public PopupElementViewItem
{
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY fieldsPopupElementChanged)
  Q_PROPERTY(QStringList fields READ fields NOTIFY fieldsPopupElementChanged)
  Q_PROPERTY(QVariantList values READ values NOTIFY fieldsPopupElementChanged)

public:
  explicit FieldsPopupElementViewController(QObject *parent = nullptr);
  explicit FieldsPopupElementViewController(QPointer<FieldsPopupElement> fieldsPopupElement,
                                          QObject *parent = nullptr);
  ~FieldsPopupElementViewController();

  QString title() const;
  QStringList fields() const;
  QVariantList values();

signals:
void fieldsPopupElementChanged();

};

} // namespace Toolkit
} // namespace Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_FIELDSPOPUPELEMENTVIEWCONTROLLER_H
