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

#include "UtilityNetworkFunctionTraceResultsModel.h"

#include "UtilityNetworkFunctionTraceResult.h"

using namespace Esri::ArcGISRuntime::Toolkit;

UtilityNetworkFunctionTraceResultsModel::UtilityNetworkFunctionTraceResultsModel(QObject* parent)
  : QAbstractListModel(parent)
{
  setupRoles();
}

Qt::ItemFlags UtilityNetworkFunctionTraceResultsModel::flags(const QModelIndex& index) const
{
  return QAbstractListModel::flags(index);
}

int UtilityNetworkFunctionTraceResultsModel::rowCount(const QModelIndex& /*parent*/) const
{
  return static_cast<int>(m_data.size());
}

QVariant UtilityNetworkFunctionTraceResultsModel::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() >= rowCount())
    return QVariant();

  const auto& functionResult = m_data[static_cast<size_t>(index.row())];

  switch (role)
  {
    case NameRole:
      return functionResult->name();
    case TypeRole:
      return functionResult->type();
    case ValueRole:
      return functionResult->value();
    default:
      qDebug() << "Incorrect UtilityNetworkFunctionTraceResultsModel data.";
  }

  return {};
}

void UtilityNetworkFunctionTraceResultsModel::addFunctionResult(UtilityNetworkFunctionTraceResult* functionResult)
{
  const int count = static_cast<int>(m_data.size());
  beginInsertRows(QModelIndex(), count, count);

  m_data.push_back(functionResult);

  endInsertRows();
}

void UtilityNetworkFunctionTraceResultsModel::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}

int UtilityNetworkFunctionTraceResultsModel::size() const
{
  return m_data.size();
}

QHash<int, QByteArray> UtilityNetworkFunctionTraceResultsModel::roleNames() const
{
  return m_roles;
}

void Esri::ArcGISRuntime::Toolkit::UtilityNetworkFunctionTraceResultsModel::setupRoles()
{
  m_roles[NameRole] = "name";
  m_roles[TypeRole] = "type";
  m_roles[ValueRole] = "value";
}
