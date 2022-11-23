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

#ifndef UTILITYNETWORKFUNCTIONTRACERESULTSMODEL_H
#define UTILITYNETWORKFUNCTIONTRACERESULTSMODEL_H

#include <QAbstractListModel>

#include "UtilityNetworkFunctionTraceResult.h"

namespace Esri::ArcGISRuntime {

class UtilityElement;

namespace Toolkit {

class UtilityNetworkFunctionTraceResultsModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit UtilityNetworkFunctionTraceResultsModel(QObject* parent = nullptr);

  enum StartingPointRoles
  {
    NameRole = Qt::UserRole + 1,
    TypeRole = Qt::UserRole + 2,
    ValueRole = Qt::UserRole + 3,
  };

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  void addFunctionResult(const UtilityNetworkFunctionTraceResult& functionResult);

  void clear();

  int size() const;

  private:
    QHash<int, QByteArray> roleNames() const override;

  void setupRoles();

  QHash<int, QByteArray> m_roles;
  QList<UtilityNetworkFunctionTraceResult> m_data;
};
} // namespace Toolkit
}// namespace Esri::ArcGISRuntime

#endif // UTILITYNETWORKFUNCTIONTRACERESULTSMODEL_H
