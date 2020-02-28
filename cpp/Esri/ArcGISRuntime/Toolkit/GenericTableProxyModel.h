#ifndef TOOLKIT_GENERICTABLEPROXYMODEL_H
#define TOOLKIT_GENERICTABLEPROXYMODEL_H

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
  GenericTableProxyModel(QObject* parent = nullptr);

  ~GenericTableProxyModel() override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  Q_INVOKABLE bool append(QList<QObject*> object);

  Q_INVOKABLE bool append(QObject* object);

  GenericListModel* sourceModel() const;
  void setSourceModel(GenericListModel* listModel);

private:
  GenericListModel* m_sourceModel;
};

}
}
}

#endif
