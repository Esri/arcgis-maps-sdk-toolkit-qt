/*******************************************************************************
 *  Copyright 2012-2018 Esri
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
  \ingroup ToolCoordinateConversion
  \inmodule ArcGISQtToolkit
  \brief Results from the CoordinateConversionController.
  \since Esri::ArcGISRuntime 100.2

  This class is primarily used by the CoordinateConversionController. You should
  not need to interact with this class directly.

  The following roles are available:
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

  \sa {Coordinate Conversion Tool}
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
  // set the new results
  beginResetModel();
  m_results.clear();
  m_results = std::move(results);
  endResetModel();

  emit resultsChanged();
}

void CoordinateConversionResults::removeResult(const QString& name)
{
  for (int i = 0; i < m_results.size(); ++i)
  {
    if (m_results.at(i).m_name.compare(name) == 0)
    {
      beginRemoveRows(QModelIndex(), i, i);
      m_results.removeAt(i);
      endRemoveRows();

      break;
    }
  }
}

/*!
  \internal
 */
void CoordinateConversionResults::clearResults()
{
  if (m_results.isEmpty())
    return;

  emit beginResetModel();
  for (auto& result : m_results)
    result.m_notation.clear();
  endResetModel();

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

  const Result& result = m_results.at(row);

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

/*!
  \fn CoordinateConversionResults::resultsChanged()
  \brief Signal emitted when the results change.
 */

/*!
  \enum CoordinateConversionResults::CoordinateConversionResultsRoles
  \brief Enumeration of roles used to access results in the list model.

  \value CoordinateConversionResultsNameRole
         The Name role.
  \value CoordinateConversionResultsNotationRole
         The Notation role.
  \value CoordinateConversionResultsCoordinateTypeRole
         The Coordinate Type role.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
