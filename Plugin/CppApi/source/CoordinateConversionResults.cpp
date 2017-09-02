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

#include "CoordinateConversionResults.h"
#include "CoordinateConversionOptions.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversionResults::CoordinateConversionResults(QObject* parent) :
  QAbstractListModel(parent)
{
  setupRoles();
}

CoordinateConversionResults::~CoordinateConversionResults()
{
}

void CoordinateConversionResults::setResults(QList<Result>&& results)
{
  // clear the old results
  if (!m_results.isEmpty())
  {
    beginRemoveRows(QModelIndex(), 0, m_results.size() - 1);
    m_results.clear();
    endRemoveRows();
  }

  QList<Result> tempResults = std::move(results);

  // set the new results
  beginInsertRows(QModelIndex(), 0, tempResults.size() - 1);
  m_results = std::move(tempResults);
  endInsertRows();

  emit resultsChanged();
}

/*!
  \internal
 */
void CoordinateConversionResults::setupRoles()
{
  m_roles[CoordinateConversionResultsNameRole] = "name";
  m_roles[CoordinateConversionResultsNotationRole] = "notation";
  m_roles[CoordinateConversionResultsCoordinateTypeRole] = "coordinateType";
}

QHash<int, QByteArray> CoordinateConversionResults::roleNames() const
{
  return m_roles;
}

Qt::ItemFlags CoordinateConversionResults::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractListModel::flags(index);
}

int CoordinateConversionResults::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;

  return m_results.size();
}

QVariant CoordinateConversionResults::data(const QModelIndex& index, int role) const
{
  const int row = index.row();
  if (row < 0 || row >= rowCount())
    return QVariant();

  const Result result = m_results.at(row);

  switch (role)
  {
  case CoordinateConversionResultsNameRole:
    return QVariant(result.m_name);
  case CoordinateConversionResultsNotationRole:
    return QVariant(result.m_notation);
  case CoordinateConversionResultsCoordinateTypeRole:
    return QVariant::fromValue<CoordinateConversionOptions::CoordinateType>(
          static_cast<CoordinateConversionOptions::CoordinateType>(result.m_type));
  default:
    break;
  }

  return QVariant();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
