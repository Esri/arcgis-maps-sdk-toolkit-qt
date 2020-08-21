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
#include "GenericTableProxyModel.h"

#include "GenericListModel.h"

#include <QMetaProperty>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

namespace
{

/*
  \internal
  \brief Takes a string and transforms it into title-case.
  
  This is for property headers.
  
  \list
  \li \a string String to turn into title case.
  \endlist

  Returns a title-case string.
 */
QString toTitleCase(const QString& string)
{
  QStringList parts = string.split(' ', QString::SkipEmptyParts);
  for (auto& part: parts)
  {
    part.replace(0, 1, part.at(0).toUpper());
  }
  return parts.join(" ");
}

}

/*!
 \internal 
 \inmodule EsriArcGISRuntimeToolkit
 \class Esri::ArcGISRuntime::Toolkit::GenericTableProxyModel

  \brief Consumes a GenericListModel and transposes each element property into
  a cell in a table.

  \sa Esri::ArcGISRuntime::Toolkit::GenericListModel

  This is a convenience type primarily for widgets where custom delegates
  aren't always the most natural fit for column-aligning data.
  
  This table takes a list of \c QObjects. In the table each row represents the
  \c QObject, and each property in the \c QObject maps to a column in the row.
  
  This table takes a GenericListModel and consumes it in the following way:
  
  \list
    \li Each property is mapped to a column.
    \li There are only 3 valid roles for each cell in the table,
        \c Qt::DisplayRole and \c Qt::EditRole which map to the displayed 
        property itself, and \c Qt::UserRole which maps to the underlying 
        object.
  \endlist
 */

/*!
  \brief Constructor

  \list
  \li \a parent Owning parent QObject.
  \endlist
 */
GenericTableProxyModel::GenericTableProxyModel(QObject* parent) :
  QAbstractTableModel(parent),
  m_sourceModel(nullptr)
{
}

/*!
  \brief Destructor.
 */
GenericTableProxyModel::~GenericTableProxyModel()
{
}

/*!
  \brief Returns the underlying GenericListModel used to populate this table.
 */
GenericListModel* GenericTableProxyModel::sourceModel() const
{
  return m_sourceModel;
}

/*!
  \brief Sets the underlying \c GenericListModel \a listModel used to populate 
  this table.
  \note Setting this will reset the entire table.
 */
void GenericTableProxyModel::setSourceModel(GenericListModel* listModel)
{
  beginResetModel();

  if (m_sourceModel)
    disconnect(m_sourceModel, nullptr, this, nullptr);

  m_sourceModel = listModel;

  if(m_sourceModel)
  {
    connect(m_sourceModel, &GenericListModel::modelAboutToBeReset, this, &GenericTableProxyModel::modelAboutToBeReset);
    connect(m_sourceModel, &GenericListModel::modelReset, this, &GenericTableProxyModel::modelReset);

    connect(m_sourceModel, &GenericListModel::dataChanged, this,
            [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& /*roles*/)
    {
      emit dataChanged(index(topLeft.row(), 0),
                       index(bottomRight.row(), columnCount() - 1),
                       { Qt::DisplayRole, Qt::EditRole});
    });

    connect(m_sourceModel, &GenericListModel::rowsAboutToBeInserted, this, &GenericTableProxyModel::rowsAboutToBeInserted);
    connect(m_sourceModel, &GenericListModel::rowsAboutToBeMoved, this, &GenericTableProxyModel::rowsAboutToBeMoved);
    connect(m_sourceModel, &GenericListModel::rowsAboutToBeRemoved, this, &GenericTableProxyModel::rowsAboutToBeRemoved);
    connect(m_sourceModel, &GenericListModel::rowsInserted, this, &GenericTableProxyModel::rowsInserted);
    connect(m_sourceModel, &GenericListModel::rowsMoved, this, &GenericTableProxyModel::rowsMoved);
    connect(m_sourceModel, &GenericListModel::rowsRemoved, this, &GenericTableProxyModel::rowsRemoved);
  }
  endResetModel();
}

/*!
  \brief Returns the number of rows in the table, and number of objects in the 
  list.
  
  \note \a parent is not used for tables.
  
  Returns number of rows. 
 */
int GenericTableProxyModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;
  return m_sourceModel ? m_sourceModel->rowCount() : 0;
}

/*!
  \brief Returns the number of columns in the table.
  
  The number of columns is dictated by the number of QMetaProperty objects
  held in the QMetaObject.
  
  \note \a parent is not used for tables.
 
  Returns number of columns in the table.
   
  \sa Esri::ArcGISRuntime::Toolkit::GenericListModel::elementType
 */
int GenericTableProxyModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return 0;
  return m_sourceModel ? m_sourceModel->roleNames().size() - 1: 0;
}

/*!
  \brief For a given index in the model, returns the data read from that
  object's property and returns the data as a QVariant.
  
  If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property 
  associated with the cell is read from.
  
  If the role is \c Qt::UserRole, then the QVariant return is the pointer
  to the underlying QObject of the row.
  
  \list
  \li \a index The index of the object in the model to query
  \li \a role The role to query.
  \endlist
 
  Returns the data as read from the property cast as a QVariant.
 */
QVariant GenericTableProxyModel::data(const QModelIndex& index, int role) const
{
  if (!m_sourceModel)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  if (role == Qt::UserRole)
    return m_sourceModel->data(m_sourceModel->index(index.row()), Qt::UserRole);

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  const auto c = index.column() + (Qt::UserRole + 1);
  return m_sourceModel->data(m_sourceModel->index(index.row()), c);
}

/*!
  \brief For a given index in the model, sets the data on the property of the
  object associated wit the current cell in the table.
   
  If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property 
  associated with the cell is written to.
  
  If the role is \c Qt::UserRole, then we attempt to overwrite the object at
  the current cell.
 
  \list 
    \li \a index The index of the object in the model to write to.
    \li \a role The role to write to
    \li \a value Value written to the property in the QObject.
  \endlist
  
  Returns \c true if the QMetaProperty::write call succeeded.
 */
bool GenericTableProxyModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid())
    return false;

  if (role == Qt::UserRole)
    return m_sourceModel->setData(m_sourceModel->index(index.row()), value, Qt::UserRole);

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return false;

  const auto c = index.column() + (Qt::UserRole + 1);
  return m_sourceModel->setData(m_sourceModel->index(index.row()), value, c);
}

/*!
  \brief Returns the flags for the cell.
 
  The table will dictate whether a cell is editable or not based on whether
  the underlying \c QMetaProperty has an \c isWritable flag.
  
  \list
  \li \a index Index of cell to query.
  \endlist

  Returns the collection of flags for the given cell.
 */
Qt::ItemFlags GenericTableProxyModel::flags(const QModelIndex &index) const
{
  if (!m_sourceModel)
    return Qt::NoItemFlags;

  auto metaObject = m_sourceModel->elementType();
  if (!metaObject)
    return Qt::NoItemFlags;

  auto flags = QAbstractTableModel::flags(index);
  auto property = metaObject->property(index.column() + metaObject->propertyOffset());
  flags.setFlag(Qt::ItemIsEditable, property.isWritable());
  return flags;
}

/*!
  \brief Returns header data for the table.
  
  The horizontal header is the collection of property names in title-case.
  
  Vertical header is the section numbering.
  
  \list
  \li \a section Section to query.
  \li \a orientation Horizontal or vertical header.
  \li \a role Role to query.
  \endlist

  Returns header data.
 */
QVariant GenericTableProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

if (Qt::Orientation::Vertical == orientation)
  return section + 1;

  const auto roles = m_sourceModel ? m_sourceModel->roleNames() : QHash<int, QByteArray>();
  auto it = roles.find(section + Qt::UserRole + 1);
  if (it != roles.end())
    return toTitleCase(QString(it.value()));
  else
    return "";
}

/*!
  \brief Calls \c insertRows on underlying \c GenericListModel.
  \sa Esri::ArcGISRuntime::Toolkit::GenericListModel::insertRows 
 
  \list
  \li \a row Index of row to insert at
  \li \a count Number of rows to insert
  \li \a parent Not applicable in tables.
  \endlist
  
  Returns \c true if insert was a success.
 */
bool GenericTableProxyModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid())
    return false;

  if (!m_sourceModel)
    return false;

  return m_sourceModel->insertRows(row, count);
}

/*!
 \brief Calls \c removeRows on underlying \c GenericListModel.
 \sa Esri::ArcGISRuntime::Toolkit::GenericListModel::removeRows

 \list 
 \li \a row Index of row to remove at.
 \li \a count Number of rows to remove.
 \li \a parent Not applicable in tables.
 \endlist

 Returns \c true if remove was a success.
 */
bool GenericTableProxyModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid())
    return false;

  if (!m_sourceModel)
    return false;

  return m_sourceModel->removeRows(row, count);
}

/*!
  \brief Calls \c append on underlying \c GenericListModel
  \sa Esri::ArcGISRuntime::Toolkit::GenericListModel::append 

  \list
  \li \a object Object to append.
  \endlist

  Returns \c true on success.
 */
bool GenericTableProxyModel::append(QList<QObject*> object)
{
  return m_sourceModel ? m_sourceModel->append(object) : false;
}

/*!
 \brief Calls \c append on underlying \c GenericListModel
 \sa Esri::ArcGISRuntime::Toolkit::GenericListModel::append 
 
 \list
 \li \a object Objects to append.
 \endlist

 Returns \c true on success.
 */
bool GenericTableProxyModel::append(QObject* object)
{
  return m_sourceModel ? m_sourceModel->append(object) : false;
}

}
}
}
