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

// Toolkit headers
#include "CoordinateConversionResult.h"
#include "CoordinateConversionController.h"
#include "Internal/GenericListModel.h"
#include "Internal/GenericTableProxyModel.h"

//Qt headers
#include <QLineEdit>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
   \internal
   \class Esri::ArcGISRuntime::Toolkit::CoordinateEditDelegate
   \inmodule EsriArcGISRuntimeToolkit
   \brief This is an implementation detail of table-view editing for the
   \c CoordinateConversion tool.

   When a coordinate is edited by the user, this tool triggers an update of the
   current point on the controller, as opposed to just editing the value in
   the cell as would normally happen.
 */

 /*!
    \brief Constructor
    \list
    \li \a parent Owning parent object.
    \endlist
  */
CoordinateEditDelegate::CoordinateEditDelegate(QObject* parent):
  QItemDelegate(parent)
{
}

/*!
  \brief Destructor
 */
CoordinateEditDelegate::~CoordinateEditDelegate()
{
}

/*!
  \brief Set the Controller object that will be updated when a user commits
  an edit.

  \list
  \li \a c Controller to update on an edit completing.
  \endlist
 */
void CoordinateEditDelegate::setController(CoordinateConversionController* c)
{
  m_controller = c;
}

/*!
  \brief Returns the controller object that will be updated when the user
  commits an edit.
 */
CoordinateConversionController* CoordinateEditDelegate::controller() const
{
  return m_controller;
}

/*!
  \brief Called when the user commits data to the model.
  \list
    \li \a editor Editing widget
    \li \a model Model to update.
    \li \a index Index in model to update.
  \endlist
 */
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

} // Esri::ArcGISRuntime::Toolkit
