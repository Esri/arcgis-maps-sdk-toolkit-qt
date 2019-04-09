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
struct Result
{
  Result(const QString& name, const QString& notation, int type);
  Result(const Result& other);
  ~Result();
  QString name;
  QString notation;
  int type = 0; // CoordinateConversionOptions::CoordinateType as int
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
  ~CoordinateConversionResults() override;

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
