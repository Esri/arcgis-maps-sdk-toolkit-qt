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

// Maps SDK headers
#include <FieldsPopupElement.h>
#include <PopupElement.h>
#include <PopupField.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController
  \inmodule ArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief In MVC architecture, this is the controller for the corresponding
  \c FieldsPopupElementView.
 */

/*!
  \brief Constructor
  \list
    \li \a fieldsPopupElement - The \l {Esri::ArcGISRuntime::FieldsPopupElement} {FieldsPopupElement} used to populate the view.
    \li \a parent - The optional parent QObject.
  \endlist
  */
FieldsPopupElementViewController::FieldsPopupElementViewController(
    FieldsPopupElement* fieldsPopupElement, QObject* parent)
  : PopupElementViewItem{fieldsPopupElement, parent}
{
}

/*!
  \brief Destructor.
  */
FieldsPopupElementViewController::~FieldsPopupElementViewController() = default;

/*!
  \brief Returns the title of the \c FieldsPopupElement.
 */
QString FieldsPopupElementViewController::title() const
{
  return static_cast<FieldsPopupElement*>(popupElement())->title();
}

/*!
  \brief Returns the combination of each element from \l {Esri::ArcGISRuntime::FieldsPopupElement::labels} {labels} and
  \l {Esri::ArcGISRuntime::FieldsPopupElement::formattedValues} {formattedValues} of the
  \c FieldsPopupElement in a QList of QMap elements
  */
QVariantList FieldsPopupElementViewController::labelsAndValues() const
{
  const auto list1 = static_cast<FieldsPopupElement*>(popupElement())->labels();
  const auto list2 = static_cast<FieldsPopupElement*>(popupElement())->formattedValues();
  QVariantList combinedData;

  Q_ASSERT(list1.size() == list2.size());

  std::transform(list1.begin(), list1.end(), list2.begin(),
                 std::back_inserter(combinedData),
                 [](const QString &item1, const QString &item2) {
    QVariantMap item;
    item["label"] = item1;
    item["formattedValue"] = item2;
    return item;
  });

  return combinedData;
}

} // namespace Esri::ArcGISRuntime::Toolkit

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController::fieldsPopupElementChanged()
  \brief Signal emitted when the underlying \c FieldsPopupElement changes.
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController::title
 */

/*!
  \property Esri::ArcGISRuntime::Toolkit::FieldsPopupElementViewController::values
 */