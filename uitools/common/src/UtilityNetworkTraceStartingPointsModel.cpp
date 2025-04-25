
/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
#include "UtilityNetworkTraceStartingPointsModel.h"

#include <UtilityElement.h>

#include "UtilityNetworkTraceStartingPoint.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

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
    case HasMultipleTerminalsRole:
      return startingPoint->hasMultipleTerminals();
    case MultipleTerminalNamesRole:
      return startingPoint->multipleTerminalNames();
    case SelectedTerminalIndexRole:
      return startingPoint->selectedTerminalIndex();
    default:
      qDebug() << "Incorrect UtilityNetworkTraceStartingPointsModel data.";
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
    case HasMultipleTerminalsRole:
      break;
    case MultipleTerminalNamesRole:
      break;
    case SelectedTerminalIndexRole:
    {
      bool success = false;
      int i = value.toInt(&success);
      if (success)
        startingPoint->setSelectedTerminalNameByIndex(i);
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

int UtilityNetworkTraceStartingPointsModel::size() const
{
  return m_data.size();
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
  m_roles[HasMultipleTerminalsRole] = "hasMultipleTerminals";
  m_roles[MultipleTerminalNamesRole] = "multipleTerminalNames";
  m_roles[SelectedTerminalIndexRole] = "selectedTerminalIndex";
}
} // namespace Esri::ArcGISRuntime::Toolkit
