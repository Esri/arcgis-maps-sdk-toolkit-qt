#ifndef UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H
#define UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H

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
#include <QAbstractListModel>

// ArcGISRuntime headers
#include <Point.h>

namespace Esri::ArcGISRuntime {

class UtilityElement;

namespace Toolkit {
class UtilityNetworkTraceStartingPoint;

class UtilityNetworkTraceStartingPointsModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit UtilityNetworkTraceStartingPointsModel(QObject* parent = nullptr);

  enum StartingPointRoles
  {
    SourceNameRole = Qt::UserRole + 1,
    GroupNameRole = Qt::UserRole + 2,
    HasFractionAlongEdgeRole = Qt::UserRole + 3,
    FractionAlongEdgeRole = Qt::UserRole + 4,
    HasMultipleTerminalsRole = Qt::UserRole + 5,
    MultipleTerminalNamesRole = Qt::UserRole + 6,
    SelectedTerminalIndexRole = Qt::UserRole + 7,
  };

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  void addStartingPoint(UtilityNetworkTraceStartingPoint* startingPoint);

  QList<UtilityElement*> utilityElements() const;

  void clear();

  bool doesItemAlreadyExist(UtilityElement* utilityElement) const;

  void removeAt(int index);

  Point pointAt(int index);

  int size() const;

  private:
    QHash<int, QByteArray> roleNames() const override;

  void setupRoles();

  QHash<int, QByteArray> m_roles;
  QList<UtilityNetworkTraceStartingPoint*> m_data;
};
} // namespace Toolkit
}// namespace Esri::ArcGISRuntime

#endif // UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H
