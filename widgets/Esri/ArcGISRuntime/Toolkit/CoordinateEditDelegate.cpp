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
    m_controller->setCurrentPoint(lineEdit->text(), result->type());
  else
    QItemDelegate::setModelData(editor, model, index);
}

} // Toolkit
} // ArcGISRuntime
} // Esri
