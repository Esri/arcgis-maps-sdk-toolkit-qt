#include "GenericTableProxyModel.h"

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

QString toTitleCase(const QString& string)
{
  QStringList parts = string.split(' ', QString::SkipEmptyParts);
  for (auto& part: parts)
  {
    part.replace(0, 1, part.at(0).toUpper());
  }
  return parts.join(" ");
}

}

GenericTableProxyModel::GenericTableProxyModel(QObject* parent) :
  QAbstractTableModel(parent),
  m_sourceModel(nullptr)
{
}

GenericTableProxyModel::~GenericTableProxyModel()
{
}

GenericListModel* GenericTableProxyModel::sourceModel() const
{
  return m_sourceModel;
}

void GenericTableProxyModel::setSourceModel(GenericListModel* listModel)
{
  beginResetModel();

  if (m_sourceModel)
    disconnect(m_sourceModel, nullptr, this, nullptr);

  m_sourceModel = listModel;

  if(m_sourceModel)
  {
    connect(m_sourceModel, &GenericListModel::modelAboutToBeReset, this, &GenericTableProxyModel::modelAboutToBeReset);
    connect(m_sourceModel, &GenericListModel::modelReset, this, &GenericTableProxyModel::modelReset);

    connect(m_sourceModel, &GenericListModel::dataChanged, this,
            [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& /*roles*/)
    {
      emit dataChanged(index(topLeft.row(), 0),
                       index(bottomRight.row(), columnCount() - 1),
                       { Qt::DisplayRole, Qt::EditRole});
    });

    connect(m_sourceModel, &GenericListModel::rowsAboutToBeInserted, this, &GenericTableProxyModel::rowsAboutToBeInserted);
    connect(m_sourceModel, &GenericListModel::rowsAboutToBeMoved, this, &GenericTableProxyModel::rowsAboutToBeMoved);
    connect(m_sourceModel, &GenericListModel::rowsAboutToBeRemoved, this, &GenericTableProxyModel::rowsAboutToBeRemoved);
    connect(m_sourceModel, &GenericListModel::rowsInserted, this, &GenericTableProxyModel::rowsInserted);
    connect(m_sourceModel, &GenericListModel::rowsMoved, this, &GenericTableProxyModel::rowsMoved);
    connect(m_sourceModel, &GenericListModel::rowsRemoved, this, &GenericTableProxyModel::rowsRemoved);
  }
  endResetModel();
}

int GenericTableProxyModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;
  return m_sourceModel ? m_sourceModel->rowCount() : 0;
}

int GenericTableProxyModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;
  return m_sourceModel ? m_sourceModel->roleNames().size() - 1: 0;
}

QVariant GenericTableProxyModel::data(const QModelIndex& index, int role) const
{
  if (!m_sourceModel)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  if (role == Qt::UserRole)
    return m_sourceModel->data(m_sourceModel->index(index.row()), Qt::UserRole);

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  const auto c = index.column() + (Qt::UserRole + 1);
  return m_sourceModel->data(m_sourceModel->index(index.row()), c);
}

bool GenericTableProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid())
    return false;

  if (role == Qt::UserRole)
    return m_sourceModel->setData(m_sourceModel->index(index.row()), value, Qt::UserRole);

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return false;

  const auto c = index.column() + (Qt::UserRole + 1);
  return m_sourceModel->setData(m_sourceModel->index(index.row()), value, c);
}

Qt::ItemFlags GenericTableProxyModel::flags(const QModelIndex &index) const
{
  if (!m_sourceModel)
    return Qt::NoItemFlags;

  auto metaObject = m_sourceModel->elementType();
  if (!metaObject)
    return Qt::NoItemFlags;

  auto flags = QAbstractTableModel::flags(index);
  auto property = metaObject->property(index.column() + metaObject->propertyOffset());
  flags.setFlag(Qt::ItemIsEditable, property.isWritable());
  return flags;
}

QVariant GenericTableProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

if (Qt::Orientation::Vertical == orientation)
  return section + 1;

  const auto roles = m_sourceModel ? m_sourceModel->roleNames() : QHash<int, QByteArray>();
  auto it = roles.find(section + Qt::UserRole + 1);
  if (it != roles.end())
    return toTitleCase(QString(it.value()));
  else
    return "";
}

bool GenericTableProxyModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid())
    return false;

  if (!m_sourceModel)
    return false;

  return m_sourceModel->insertRows(row, count);
}

bool GenericTableProxyModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid())
    return false;

  if (!m_sourceModel)
    return false;

  return m_sourceModel->removeRows(row, count);
}

bool GenericTableProxyModel::append(QList<QObject*> object)
{
  return m_sourceModel ? m_sourceModel->append(object) : false;
}

bool GenericTableProxyModel::append(QObject* object)
{
  return m_sourceModel ? m_sourceModel->append(object) : false;
}

}
}
}
