#include "GenericListModel.h"

#include <QMetaProperty>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{


namespace
{

class MetaElement : public QObject
{
  Q_OBJECT
public:
  MetaElement(QModelIndex _index,
              int _role,
              GenericListModel* _listModel,
              const char* _propertyName,
              QObject* parent) :
    QObject(parent),
    m_trackedObject(_index),
    m_listModel(_listModel),
    m_propertyName(_propertyName),
    m_role(_role)
  {
    connect(this, &MetaElement::propertyChanged,
            this, &MetaElement::emitDataChanged);
    connect(m_listModel.data(), &QObject::destroyed, this, &QObject::deleteLater);
  }

  Q_SIGNAL void propertyChanged();

private:
  Q_SLOT void emitDataChanged()
  {
    if (!m_listModel || !m_trackedObject.isValid())
      return;

    QVector<int> roles { m_role };
    if (m_propertyName == m_listModel->displayPropertyName())
      roles << Qt::DisplayRole << Qt::EditRole;

      emit m_listModel->dataChanged(
            m_trackedObject,
            m_trackedObject,
            roles);
  }

  QPersistentModelIndex m_trackedObject;
  QPointer<GenericListModel> m_listModel;
  const char* m_propertyName;
  int m_role;
};

}

GenericListModel::GenericListModel(QObject* parent) :
  GenericListModel(nullptr, parent)
{
}

GenericListModel::GenericListModel(const QMetaObject* elementType, QObject* parent) :
  QAbstractListModel(parent),
  m_elementType(std::move(elementType))
{
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
  else if (index.parent().isValid())
    return QVariant();
  else if (index.column() != 0)
    return QVariant();
  else if (index.row() < 0)
    return QVariant();
  else if (index.row() >= rowCount())
    return QVariant();

  auto o = m_objects.at(index.row());
  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    const auto propIndex = m_elementType->indexOfProperty(m_displayPropName.toLatin1());
    const auto property = m_elementType->property(propIndex);
    return property.read(o);
  }
  else if (role >= Qt::UserRole)
  {
    const auto propIndex = role - Qt::UserRole + m_elementType->propertyOffset();
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
  else if (index.parent().isValid())
    return false;
  else if (index.column() != 0)
    return false;
  else if (index.row() < 0)
    return false;
  else if (index.row() >= rowCount())
    return false;
  else if (!value.isValid())
    return false;

  auto o = m_objects.at(index.row());
  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    const auto propIndex = m_elementType->indexOfProperty(m_displayPropName.toLatin1());
    const auto property = m_elementType->property(propIndex);
    return property.write(o, value);
  }
  else if (role >= Qt::UserRole)
  {
    const auto propIndex = role - Qt::UserRole + m_elementType->propertyOffset();
    const auto property = m_elementType->property(propIndex);
    return property.write(o, value);
  }
  else
  {
    return false;
  }
}

Qt::ItemFlags GenericListModel::flags(const QModelIndex& index) const
{
  const auto flags = QAbstractListModel::flags(index);
  if (index.isValid())
    return flags | Qt::ItemIsEditable;
  else
    return flags;
}

QHash<int, QByteArray> GenericListModel::roleNames() const
{
  if (!m_elementType)
    return { };

  QHash<int, QByteArray> output;
  const int offset = m_elementType->propertyOffset();
  for (int i = offset; i < m_elementType->propertyCount(); ++i)
  {
    auto property = m_elementType->property(i);
    output.insert(i - offset + Qt::UserRole, property.name());
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
  else if (row > rowCount(parent))
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
  else if (count < 1)
    return false;
  else if (row < 0)
    return false;
  else if (row + count > rowCount(parent))
    return false;

  beginRemoveRows(parent, row, row + count - 1);
  for (int i = 0; i < count; ++i)
  {
    delete m_objects.at(row);
    m_objects.removeAt(row);
  }
  endInsertRows();
  return true;
}

void GenericListModel::setElementType(const QMetaObject* metaObject)
{
  beginRemoveRows(QModelIndex(), 0, rowCount());
  qDeleteAll(m_objects);
  m_objects.clear();
  m_elementType = metaObject;
  endRemoveRows();
}

const QMetaObject* GenericListModel::elementType() const
{
  return m_elementType;
}

void GenericListModel::setDisplayPropertyName(QString propertyName)
{
  m_displayPropName = std::move(propertyName);
}

QString GenericListModel::displayPropertyName()
{
  return m_displayPropName;
}

bool GenericListModel::append(QObject* object)
{
  if (!m_elementType)
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

  auto i = rowCount();
  beginInsertRows(QModelIndex(), i, i + objects.size() - 1);
  m_objects << objects;
  endInsertRows();
  for (; i < size; ++i)
  {
    connectElement(index(i));
  }

  return true;
}

void GenericListModel::connectElement(QModelIndex index)
{
  if (!m_elementType)
    return;
  else if (!index.isValid())
    return;
  else if (index.parent().isValid())
    return;
  else if (index.column() != 0)
    return;
  else if (index.row() < 0)
    return;
  else if (index.row() >= rowCount())
    return;

  QObject* object = m_objects.at(index.row());

  if (!object || object->metaObject() != m_elementType)
    return;

  const auto offset = m_elementType->propertyOffset();
  for (int i = offset; i < m_elementType->propertyCount(); ++i)
  {
    auto property = m_elementType->property(i);
    auto notifySignal = property.notifySignal();
    if (notifySignal.isValid())
    {
      auto element = new MetaElement(
        index,
        i - offset + Qt::UserRole,
        this,
        property.name(),
        object);

      connect(object, notifySignal,
              element, QMetaMethod::fromSignal(&MetaElement::propertyChanged));
    }
  }
}

} // Toolkit
} // ArcGISRuntime
} // Esri

#include "GenericListModel.moc"
