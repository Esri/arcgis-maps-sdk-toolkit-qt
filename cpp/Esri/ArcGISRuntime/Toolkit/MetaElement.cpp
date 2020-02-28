#include "MetaElement.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

MetaElement::MetaElement(
  QModelIndex index,
  int customRole,
  QObject* trackedObject,
  QAbstractItemModel* parent):
  QObject(parent),
  m_index(std::move(index)),
  m_customRole(customRole),
  m_trackedObject(trackedObject),
  m_parentModel(parent)
{
  connect(this, &MetaElement::propertyChanged, this, &MetaElement::emitDataChanged);
  connect(m_trackedObject.data(), &QObject::destroyed, this, &QObject::deleteLater);
}

void MetaElement::emitDataChanged()
{
  if (!m_trackedObject || !m_index.isValid())
    return;
  else
    emit m_parentModel->dataChanged(m_index, m_index, { Qt::UserRole, m_customRole });
}

} // Esri
} // ArcGISRuntime
} // Toolkit
