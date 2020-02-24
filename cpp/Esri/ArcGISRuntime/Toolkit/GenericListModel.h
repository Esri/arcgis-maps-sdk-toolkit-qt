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
#ifndef GENERICLISTMODEL_H
#define GENERICLISTMODEL_H

#include <QAbstractListModel>
#include <QMetaObject>

#include <type_traits>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class GenericListModel : public QAbstractListModel
{
  Q_OBJECT

public:
  Q_INVOKABLE GenericListModel(QObject* parent = nullptr);
  GenericListModel(const QMetaObject* elementType, QObject* parent = nullptr);

  ~GenericListModel() override;

  void setElementType(const QMetaObject* elementType);

  const QMetaObject* elementType() const;

  void setDisplayPropertyName(QString propertyName);

  QString displayPropertyName();

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  QHash<int, QByteArray> roleNames() const override;
  
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  Q_INVOKABLE bool append(QList<QObject*> object);

  Q_INVOKABLE bool append(QObject* object);

  template <typename T>  
  T* element(const QModelIndex& index) const
  {
    static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
    return qvariant_cast<T*>(data(index, Qt::UserRole));
  }

private:
  void connectElement(QModelIndex object);

private:
  QString m_displayPropName;
  const QMetaObject* m_elementType;
  QList<QObject*> m_objects;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // GenericListModel
