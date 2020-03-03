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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_GENERICTABLEPROXYMODEL_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_GENERICTABLEPROXYMODEL_H

// Qt headers
#include <QAbstractProxyModel>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class GenericListModel;

/*!
 * \internal
 * \brief Consumes a GenericListModel and transposes each element property into
 * a cell in a table.
 * \sa GenericListModel.
 * 
 * This is a convenience type primarily for widgets where custom delegates
 * aren't always the most natural fit for column-aligning data.
 * 
 * This table takes a list of QObjects. In the table each row represents the
 * QObject, and each property in the QObject maps to a column in the row.
 * 
 * This table takes a GenericListModel and consumes it in the following way:
 *  - Each property is mapped to a column.
 *  - There are only 3 valid roles for each cell in the table,
 *    Qt::DisplayRole and Qt::EditRole which map to the displayed property
 *    itself, and Qt::UserRole which maps to the underlying object.
 */
class GenericTableProxyModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  /*!
   * \brief Constructor
   * \param parent Owning parent QObject.
   */
  Q_INVOKABLE GenericTableProxyModel(QObject* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~GenericTableProxyModel() override;

  /*!
   * \brief Returns the number of rows in the table, and number of objects in
   * the list.
   * \param parent Not used for tables.
   * \return Number of rows. 
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /*!
   * \brief Returns the number of columns in the table.
   * 
   * The number of columns is dictated by the number of QMetaProperty objects
   * held in the QMetaObject.
   * 
   * \param parent Not used for tables.
   * \return Number of columns in the table.
   * 
   * \sa GenericListModel::elementType
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  /*!
   * \brief For a given index in the model, returns the data read from that
   * object's property and returns the data as a QVariant.
   * 
   * If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property 
   * associated with the cell is read from.
   * 
   * If the role is \c Qt::UserRole, then the QVariant return is the pointer
   * to the underlying QObject of the row.
   * 
   * \param index The index of the object in the model to query
   * \param role The role to query.
   * \return QVariant The data as read from the property cast as a QVariant.
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /*!
   * \brief For a given index in the model, sets the data on the property of the
   * object associated wit the current cell in the table.
   * 
   * If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property 
   * associated with the cell is written to.
   * 
   * If the role is \c Qt::UserRole, then we attempt to overwrite the object at
   * the current cell.
   * 
   * \param index The index of the object in the model to write to.
   * \param role The role to write to
   * \param value Value written to the property in the QObject.
   * \return true The QMetaProperty::write call succeeded.
   * \return false The QMetaProperty::write call failed.
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  /*!
   * \brief Returns the flags for the cell.
   *
   * The Table will dictate whether a cell is editable or not based on whether
   * the underlying QMetaProperty has an isWritable flag.
   * 
   * \param index Index of cell to query.
   * \return Collection of Flags for the given cell.
   */
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  /*!
   * \brief Returns header data for the table.
   * 
   * The horizontal header is the collection of property names in title-case.
   * 
   * Vertical header is the section numbering.
   * 
   * \param section Section to query.
   * \param orientation Horizontal or vertical header.
   * \param role Role to query.
   * \return Header data.
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /*!
   * \brief Calls insertRows on underlying GenericListModel.
   * \sa GenericListModel::insertRows 
   * \param row Index of row to insert at
   * \param count Number of rows to insert
   * \param parent Not applicable in tables.
   * \return true True if insert was a success.
   * \return false False if insert was a failure.
   */
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  /*!
   * \brief Calls removeRows on underlying GenericListModel.
   * \sa GenericListModel::removeRows 
   * \param row Index of row to remove at
   * \param count Number of rows to remove
   * \param parent Not applicable in tables.
   * \return true True if remove was a success.
   * \return false False if remove was a failure.
   */
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  /*!
   * \brief Calls append on underlying GenericListModel
   * \sa GenericListModel::append 
   * \param object Object to append.
   * \return Returns true on success, false on failure.
   */
  Q_INVOKABLE bool append(QList<QObject*> object);

  /*!
   * \brief Calls append on underlying GenericListModel
   * \sa GenericListModel::append 
   * \param object Objects to append.
   * \return Returns true on success, false on failure.
   */
  Q_INVOKABLE bool append(QObject* object);

  /*!
   * \brief Returns the underlying GenericListModel used to populate this table.
   */
  GenericListModel* sourceModel() const;

  /*!
   * \brief Sets the underlying GenericListModel used to populate this table.
   * \note Setting this will reset the entire table.
   */
  void setSourceModel(GenericListModel* listModel);

private:
  GenericListModel* m_sourceModel;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_GENERICTABLEPROXYMODEL_H
