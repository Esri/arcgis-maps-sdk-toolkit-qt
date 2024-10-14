
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
#include "FieldsPopupElementViewController.h"
#include "PopupField.h"

namespace Esri::ArcGISRuntime::Toolkit {

FieldsPopupElementViewController::FieldsPopupElementViewController(QObject *parent)
  : PopupElementViewItem{parent}
{
}

FieldsPopupElementViewController::~FieldsPopupElementViewController() = default;

/*!
\brief Constructor. Takes a \a PopupElement and \a parent object.
  */
FieldsPopupElementViewController::FieldsPopupElementViewController(
    QPointer<FieldsPopupElement> fieldsPopupElement, QObject *parent)
  : PopupElementViewItem{QPointer<FieldsPopupElement>(fieldsPopupElement), parent}
{
  // qDebug() << fieldsPopupElement->fields()->size();
  // for ( auto field : *fieldsPopupElement->fields())
  // {
    // qDebug() << field->label();
  // }
}

QString FieldsPopupElementViewController::title() const
{
  return static_cast<FieldsPopupElement*>(popupElement().get())->title();
}

QStringList FieldsPopupElementViewController::fields() const
{
  return static_cast<FieldsPopupElement*>(popupElement().get())->labels();
}

QVariantList FieldsPopupElementViewController::values()
{
  auto list1 = static_cast<FieldsPopupElement*>(popupElement().get())->labels();
  auto list2 = static_cast<FieldsPopupElement*>(popupElement().get())->formattedValues();
  QVariantList combinedData;
  int size = qMin(list1.size(), list2.size());

  std::transform(list1.begin(), list1.begin() + size, list2.begin(),
                 std::back_inserter(combinedData),
                 [](const QString &item1, const QString &item2) {
                     QVariantMap item;
                     item["labels"] = item1;
                     item["formattedValues"] = item2;
                     return item;
                 });
  emit fieldsPopupElementChanged();
  return combinedData;

  // QVariantList combinedList;
  // combinedList.append(QVariant::fromValue(list1));
  // combinedList.append(QVariant::fromValue(list2));

  // need to comebine these some way to be used as the model for the ListView of labels.
  // QVariantList combinedData = QVariantList::fromList(
  //     list1.toVector().zip(list2.toVector()).toList().transform([](const auto &pair) {
  //         QVariantMap item;
  //         item["column1"] = pair.first;
  //         item["column2"] = pair.second;
  //         return item;
  //     })
  // );
  // emit fieldsPopupElementChanged();
  // return static_cast<FieldsPopupElement*>(popupElement().get())->formattedValues();
}

} // namespace Esri::ArcGISRuntime::Toolkit
