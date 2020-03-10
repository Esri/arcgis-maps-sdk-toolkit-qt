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
#include "CoordinateEditDelegate.h"

#include "Esri/ArcGISRuntime/Toolkit/CoordinateConversionResult.h"
#include "Esri/ArcGISRuntime/Toolkit/CoordinateConversionController.h"
#include "Esri/ArcGISRuntime/Toolkit/GenericListModel.h"
#include "Esri/ArcGISRuntime/Toolkit/GenericTableProxyModel.h"

//Qt headers
#include <QLineEdit>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateEditDelegate::CoordinateEditDelegate(QObject* parent):
  QItemDelegate(parent) 
{
}

CoordinateEditDelegate::~CoordinateEditDelegate()
{
}

void CoordinateEditDelegate::setController(CoordinateConversionController* c)
{
  m_controller = c;
}

CoordinateConversionController* CoordinateEditDelegate::controller() const
{
  return m_controller;
}

void CoordinateEditDelegate::setModelData(
  QWidget* editor,
  QAbstractItemModel* model,
  const QModelIndex& index) const
{
  auto lineEdit = qobject_cast<QLineEdit*>(editor);

  if (!lineEdit || !m_controller)
    return QItemDelegate::setModelData(editor, model, index);

  auto tableModel = qobject_cast<GenericTableProxyModel*>(model);
  
  if (!tableModel)
    return QItemDelegate::setModelData(editor, model, index);
  
  auto listModel = tableModel->sourceModel();
  auto result = listModel->element<CoordinateConversionResult>(index);
  if (result)
  {
    // Only commit if text differs - otherwise what looks like a
    // non-change will cause a change due to differences in format accuracies.
    if (result->notation() != lineEdit->text())
      m_controller->setCurrentPoint(lineEdit->text(), result->type());
  }
  else
  {
    QItemDelegate::setModelData(editor, model, index);
  }
}

} // Toolkit
} // ArcGISRuntime
} // Esri
