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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GENERICTABLEPROXYMODEL_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GENERICTABLEPROXYMODEL_H

// Qt headers
#include <QAbstractProxyModel>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class GenericListModel;

class GenericTableProxyModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit GenericTableProxyModel(QObject* parent = nullptr);

  ~GenericTableProxyModel() override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  Q_INVOKABLE bool append(QList<QObject*> object);

  Q_INVOKABLE bool append(QObject* object);

  GenericListModel* sourceModel() const;

  void setSourceModel(GenericListModel* listModel);

private:
  GenericListModel* m_sourceModel = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_GENERICTABLEPROXYMODEL_H
