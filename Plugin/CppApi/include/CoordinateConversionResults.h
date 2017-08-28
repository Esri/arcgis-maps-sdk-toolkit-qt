// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef COORDINATECONVERSIONRESULTS_H
#define COORDINATECONVERSIONRESULTS_H

#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include "SolutionsCommon.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Solutions
{

struct SOLUTIONS_EXPORT Result
{
  Result(const QString& name, const QString& notation, int type) :
    m_name(name), m_notation(notation), m_type(type)
  {
  }

  QString m_name;
  QString m_notation;
  int m_type; // actually CoordinateConversionOptions::CoordinateType
};

class SOLUTIONS_EXPORT CoordinateConversionResults : public QAbstractListModel
{
  Q_OBJECT

public:
  enum CoordinateConversionResultsRoles
  {
    CoordinateConversionResultsNameRole = Qt::UserRole + 1,
    CoordinateConversionResultsNotationRole = Qt::UserRole + 2,
    CoordinateConversionResultsCoordinateTypeRole = Qt::UserRole + 3
  };

public:
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  explicit CoordinateConversionResults(QObject* parent = nullptr);
  ~CoordinateConversionResults();

  void setResults(QList<Result>&& results);

signals:
  void resultsChanged();

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  void setupRoles();
  QHash<int, QByteArray> m_roles;
  QList<Result> m_results;
};

} // Solutions
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONRESULTS_H
