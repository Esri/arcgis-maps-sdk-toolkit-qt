// Copyright 2017 ESRI
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
#include "ToolkitCommon.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \internal
*/
class Result
{
public:
 Result(const QString& name, const QString& notation, int type);
 ~Result() = default;
 QString m_name;
 QString m_notation;
 int m_type; // CoordinateConversionOptions::CoordinateType as int
};

class TOOLKIT_EXPORT CoordinateConversionResults : public QAbstractListModel
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
  explicit CoordinateConversionResults(QObject* parent = nullptr);
  ~CoordinateConversionResults();

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

signals:
  void resultsChanged();

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  friend class CoordinateConversionController;

  void setResults(QList<Result>&& results);
  void removeResult(const QString& name);
  void clearResults();
  void setupRoles();

  QHash<int, QByteArray> m_roles;
  QList<Result> m_results;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONRESULTS_H
