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
#include "GenericListModel.h"
#include "MetaElement.h"

#include <QMetaProperty>
#include <QPointer>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{


GenericListModel::GenericListModel(QObject* parent) :
  GenericListModel(nullptr, parent)
{
}

GenericListModel::GenericListModel(const QMetaObject* elementType, QObject* parent) :
  QAbstractListModel(parent),
  m_displayPropIndex(-1),
  m_elementType(std::move(elementType))
{
  connect(this, &GenericListModel::rowsInserted, this, &GenericListModel::countChanged);
  connect(this, &GenericListModel::rowsRemoved, this, &GenericListModel::countChanged);

  // We connect to our own `dataChanged` signal. We test to see if the properties
  // that are updating are also the same properties associated with our
  // Qt::DisplayRole and Qt::EditRole. If so then we need to emit dataChanged
  // for these roles as well if they are not already emitting.
  connect(this, &GenericListModel::dataChanged, this,
    [this](const QModelIndex& topLeft, const QModelIndex& bottomRight,
       const QVector<int>& roles)
  {
    if (m_displayPropIndex < 0)
      return;

    if (!m_elementType)
      return;

    if (roles.contains(Qt::DisplayRole))
      return;

    const auto offset = m_elementType->propertyOffset();
    if (roles.contains(m_displayPropIndex - offset + Qt::UserRole +1))
      emit dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
  });
}

GenericListModel::~GenericListModel()
{
}

int GenericListModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;
  else
    return m_objects.size();
}

QVariant GenericListModel::data(const QModelIndex& index, int role) const
{
  if (!m_elementType)
    return QVariant();
  else if (!index.isValid())
    return QVariant();

  auto o = m_objects.at(index.row());
  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    const auto property = m_elementType->property(m_displayPropIndex);
    return property.read(o);
  }
  else if (role == Qt::UserRole)
  {
    return QVariant::fromValue<QObject*>(o);
  }
  else if (role >= Qt::UserRole)
  {
    const auto propIndex = role - (Qt::UserRole + 1) + m_elementType->propertyOffset();
    const auto property = m_elementType->property(propIndex);
    return property.read(o);
  }
  else
  {
    return QVariant();
  }
}

bool GenericListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!m_elementType)
    return false;
  else if (!index.isValid())
    return false;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    auto o = m_objects.at(index.row());
    const auto property = m_elementType->property(m_displayPropIndex);
    return property.write(o, value);
  }
  else if (role == Qt::UserRole)
  {
    auto newObject = qvariant_cast<QObject*>(value);
    if (newObject && m_elementType == newObject->metaObject())
    {
      m_objects[index.row()] = newObject;
      emit dataChanged(index, index);
      return true;
    }
    else
    {
      return false;
    }
  }
  else if (role > Qt::UserRole)
  {
    auto o = m_objects.at(index.row());
    const auto propIndex = role - (Qt::UserRole + 1) + m_elementType->propertyOffset();
    const auto property = m_elementType->property(propIndex);
    return property.write(o, value);
  }
  else
  {
    return false;
  }
}

QHash<int, QByteArray> GenericListModel::roleNames() const
{
  if (!m_elementType)
    return { };

  QHash<int, QByteArray> output;
  output.insert( Qt::UserRole, "modelData");

  const int offset = m_elementType->propertyOffset();
  for (int i = offset; i < m_elementType->propertyCount(); ++i)
  {
    auto property = m_elementType->property(i);
    output.insert(i - offset + Qt::UserRole + 1, property.name());
  }
  return output;
}

bool GenericListModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (!m_elementType)
    return false;
  if (parent.isValid())
    return false;
  else if (count < 1)
    return false;
  else if (row < 0)
    return false;
  else if (row > m_objects.size())
    return false;

  beginInsertRows(parent, row, row + count - 1);
  for (int i = 0; i < count; ++i)
  {
    m_objects.insert(row, m_elementType->newInstance(Q_ARG(QObject*, this)));
  }
  endInsertRows();

  for (int i = row; i < count; ++i)
  {
    connectElement(index(row));
  }
  return true;
}

bool GenericListModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid())
    return false;
  else if (count < 0)
    return false;
  else if (row < 0)
    return false;
  else if (row >= rowCount(parent))
    return false;
  else if (row + count > rowCount(parent))
    return false;
  else if (count == 0)
    return true; // A valid no-op.

  beginRemoveRows(parent, row, row + count - 1);
  for (int i = count - 1; i >= row; --i)
  {
    auto o = m_objects.at(i);

    // Ensure additional removal signals are not triggered.
    if (o)
      disconnect(o, &QObject::destroyed, this, nullptr);

    delete o;

    m_objects.removeAt(i);
  }
  endRemoveRows();
  return true;
}

void GenericListModel::setElementType(const QMetaObject* metaObject)
{
  beginResetModel();
  for (auto o : m_objects)
  {
    // Ensure additional removal signals are not triggered.
    if (o)
      disconnect(o, &QObject::destroyed, this, nullptr);

    delete o;
  }
  m_objects.clear();
  m_elementType = metaObject;
  m_displayPropIndex = -1;
  endResetModel();
}

const QMetaObject* GenericListModel::elementType() const
{
  return m_elementType;
}

void GenericListModel::setDisplayPropertyName(QString propertyName)
{
  m_displayPropIndex = m_elementType->indexOfProperty(propertyName.toLatin1());
}

QString GenericListModel::displayPropertyName()
{
  if (m_displayPropIndex < 0)
    return "";
  else
    return m_elementType->property(m_displayPropIndex).name();
}

bool GenericListModel::append(QObject* object)
{
  if (!m_elementType)
    return false;
  
  if (!object)
    return false;

  if (object->metaObject() != m_elementType)
    return false;

  auto i = rowCount();
  beginInsertRows(QModelIndex(), i, i);
  m_objects << object;
  endInsertRows();
  connectElement(index(i));
  return true;
}

bool GenericListModel::append(QList<QObject*> objects)
{
  const auto size = objects.size();

  if (!m_elementType)
    return false;
  if (size < 1)
    return true;

  for (auto o: objects)
  {
    if (!o)
      return false;
    else if (o->metaObject() != m_elementType)
      return false;
  }

  auto i = rowCount();

  beginInsertRows(QModelIndex(), i, i + size - 1);
  m_objects << objects;
  endInsertRows();

  for (; i < size; ++i)
    connectElement(index(i));

  return true;
}

void GenericListModel::connectElement(QModelIndex index)
{
  if (!m_elementType)
    return;
  else if (!index.isValid())
    return;

  QObject* object = m_objects.at(index.row());

  if (!object || object->metaObject() != m_elementType)
    return;

  // If object is deleted externally we remove from the model.
  connect(object, &QObject::destroyed, this,
    [this, pIndex = QPersistentModelIndex(index)]
    {
      if (!pIndex.isValid())
        return;

      const auto row = pIndex.row();
      
      if (row < 0 || row >= m_objects.size())
        return;

      m_objects[row] = nullptr; // Prevents double delete.
      removeRow(row, pIndex.parent());
    }
  );

  // Connect to each property notifySignal and hook up to our dataChanged signal
  // using MetaElement objects as drop-in replacements for lambdas.
  const auto offset = m_elementType->propertyOffset();
  for (int i = offset; i < m_elementType->propertyCount(); ++i)
  {
    auto property = m_elementType->property(i);
    auto notifySignal = property.notifySignal();
    if (notifySignal.isValid())
    {
      auto element = new MetaElement(
        index,
        i - offset + Qt::UserRole +1,
        object,
        this);

      // Signal to signal connection.
      connect(object, notifySignal,
              element, QMetaMethod::fromSignal(&MetaElement::propertyChanged));
    }
  }
}

int GenericListModel::count() const
{
  return m_objects.size();
}

QVariant GenericListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (!m_elementType)
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  if (Qt::Orientation::Vertical == orientation)
    return section + 1;
  else if (Qt::Orientation::Horizontal && section == 0)
    return QString(m_elementType->className());
  else
    return "";
}

} // Toolkit
} // ArcGISRuntime
} // Esri
