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

#include "CoordinateConversionResults.h"
#include "CoordinateConversionOptions.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionResults
  \since Esri::ArcGISRuntime 100.2
  \brief This class stores results for the CoordinateConversionController.

  This class is primarily used by the CoordinateConversionController. You should
  not need to interact with this class directly.

  The following roles are available
  \table
    \header
        \li Role
        \li Type
        \li Description
        \li AttributeRoles
    \row
        \li name
        \li QString
        \li The name of the option.
        \li \l {Esri::ArcGISRuntime::Toolkit::CoordinateConversionResults::CoordinateConversionResultsNameRole}{CoordinateConversionResultsNameRole}
    \row
        \li notation
        \li QString
        \li The resulting notation from any conversion operations.
        \li \l {Esri::ArcGISRuntime::Toolkit::CoordinateConversionResults::CoordinateConversionResultsNotationRole}{CoordinateConversionResultsNotationRole}
    \row
        \li type
        \li int (as CoordinateConversionOptions::CoordinateType)
        \li The format used for the conversion.
        \li \l {Esri::ArcGISRuntime::Toolkit::CoordinateConversionResults::CoordinateConversionResultsCoordinateTypeRole}{CoordinateConversionResultsCoordinateTypeRole}
  \endtable
 */

/*!
  \internal
 */
CoordinateConversionResults::CoordinateConversionResults(QObject* parent) :
  QAbstractListModel(parent)
{
  setupRoles();
}

/*!
  \internal
 */
CoordinateConversionResults::~CoordinateConversionResults()
{
}

/*!
  \internal
 */
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
void CoordinateConversionResults::clearResults()
{
  if (m_results.isEmpty())
    return;

  for (auto& result : m_results)
    result.m_notation.clear();

  emit dataChanged(index(0), index(m_results.size()-1), QVector<int>() << CoordinateConversionResultsNotationRole);
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

/*!
  \internal
 */
QHash<int, QByteArray> CoordinateConversionResults::roleNames() const
{
  return m_roles;
}

/*!
  \internal
 */
Qt::ItemFlags CoordinateConversionResults::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractListModel::flags(index);
}

/*!
  \internal
 */
int CoordinateConversionResults::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;

  return m_results.size();
}

/*!
  \internal
 */
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

/*!
  \internal
 */
Result::Result(const QString& name, const QString& notation, int type) :
  m_name(name), m_notation(notation), m_type(type)
{
}

} // Toolkit
} // ArcGISRuntime
} // Esri
