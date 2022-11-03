#ifndef UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H
#define UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H

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
    HasFractionAlongEdgeRole = Qt::UserRole +3,
    FractionAlongEdgeRole = Qt::UserRole +4,
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

  private:
    QHash<int, QByteArray> roleNames() const override;

  void setupRoles();

  QHash<int, QByteArray> m_roles;
  QList<UtilityNetworkTraceStartingPoint*> m_data;
};
} // namespace Toolkit
}// namespace Esri::ArcGISRuntime

#endif // UTILITYNETWORKTRACESTARTINGPOINTSMODEL_H
