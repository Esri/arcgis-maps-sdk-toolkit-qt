


#include "UtilityNetworkTraceStartingPointsModel.h"

#include "UtilityElement.h"
#include "UtilityNetworkTraceStartingPoint.h"

using namespace Esri::ArcGISRuntime::Toolkit;

UtilityNetworkTraceStartingPointsModel::UtilityNetworkTraceStartingPointsModel(QObject* parent)
  : QAbstractListModel(parent)
{
  setupRoles();
}

Qt::ItemFlags UtilityNetworkTraceStartingPointsModel::flags(const QModelIndex& index) const
{
  return QAbstractListModel::flags(index);
}

int UtilityNetworkTraceStartingPointsModel::rowCount(const QModelIndex& /*parent*/) const
{
  return static_cast<int>(m_data.size());
}

QVariant UtilityNetworkTraceStartingPointsModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= rowCount())
    return QVariant();

  const auto& startingPoint = m_data[static_cast<size_t>(index.row())];

  switch (role)
  {
    case SourceNameRole:
      return startingPoint->sourceName();
    case GroupNameRole:
      return startingPoint->groupName();
    case HasFractionAlongEdgeRole:
      return startingPoint->hasFractionAlongEdge();
    case FractionAlongEdgeRole:
      return startingPoint->fractionAlongEdge();
  }

  return {};
}

bool UtilityNetworkTraceStartingPointsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (index.row() < 0 || index.row() >= rowCount())
    return false;

  auto& startingPoint = m_data[static_cast<size_t>(index.row())];

  switch (role)
  {
    case SourceNameRole:
      break;
    case GroupNameRole:
      break;
    case HasFractionAlongEdgeRole:
      break;
    case FractionAlongEdgeRole:
    {
      bool success = false;
      double fraction = value.toDouble(&success);
      if (success)
        startingPoint->setFractionAlongEdge(fraction);
      return success;
    }
  }

  return false;
}

void UtilityNetworkTraceStartingPointsModel::addStartingPoint(UtilityNetworkTraceStartingPoint* startingPoint)
{
  const int count = static_cast<int>(m_data.size());
  beginInsertRows(QModelIndex(), count, count);

  m_data.push_back(startingPoint);

  endInsertRows();
}

QList<Esri::ArcGISRuntime::UtilityElement*> UtilityNetworkTraceStartingPointsModel::utilityElements() const
{
  QList<UtilityElement*> utilityElementsForStartingPoints;
  for (const auto& sp : m_data)
  {
    utilityElementsForStartingPoints.append(sp->utilityElement());
  }

  return utilityElementsForStartingPoints;
}

void UtilityNetworkTraceStartingPointsModel::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}

bool UtilityNetworkTraceStartingPointsModel::doesItemAlreadyExist(UtilityElement* utilityElement) const
{
  auto utilityElementId = utilityElement->objectId();
  for (const auto& startingPoint : m_data)
  {
    if (startingPoint->utilityElement()->objectId() == utilityElementId)
    {
      return true;
    }
  }
  return false;
}

void UtilityNetworkTraceStartingPointsModel::removeAt(int index)
{
  beginRemoveRows(QModelIndex(),index,index);
  m_data.remove(index);
  endRemoveRows();
}

Esri::ArcGISRuntime::Point UtilityNetworkTraceStartingPointsModel::pointAt(int index)
{
  return m_data.at(index)->extent().center();
}

QHash<int, QByteArray> UtilityNetworkTraceStartingPointsModel::roleNames() const
{
  return m_roles;
}

void UtilityNetworkTraceStartingPointsModel::setupRoles()
{
  m_roles[SourceNameRole] = "sourceName";
  m_roles[GroupNameRole] = "groupName";
  m_roles[HasFractionAlongEdgeRole] = "hasFractionAlongEdge";
  m_roles[FractionAlongEdgeRole] = "fractionAlongEdge";
}
