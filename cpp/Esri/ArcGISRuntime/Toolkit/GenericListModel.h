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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_GENERICLISTMODEL_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_GENERICLISTMODEL_H

#include <QAbstractListModel>
#include <QMetaObject>

#include <type_traits>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \internal
 * \brief This ListModel automatically exposes a list of QObjects with
 * properties as a list of elements where each property is mapped to a user
 * role.
 * 
 * A GenericListModel addresses the need to take a collection of properties,
 * on a QObject and expose them in a view (Widgets or QML).
 * 
 * A GenericListModel takes a QMetaObject as an `elementType`. Each property in
 * the QMetaObject is scanned and assigned a role in the order in which the
 * property was declared in the class.
 * 
 * For example, given a class Foo:
 * 
 * ```
 * class Foo : public QObject
 * {
 *   Q_OBJECT
 *   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA NOTIFY propertyAChanged)
 *   Q_PROPERTY(QString propertyB READ propertyB NOTIFY propertyBChanged)
 *   Q_PROPERTY(bool propertyC READ propertyC CONTANT)
 * };
 * ```
 * 
 * Then a GenericListModel will expose the following roles:
 * 
 * +-----------+------------------+---------+
 * |    Name   |      Value       | Type    |
 * +-----------+------------------+---------+
 * | modelData | Qt::UserRole     | Foo*    |
 * | propertyA | Qt::UserRole + 1 | int     |
 * | propertyB | Qt::UserRole + 2 | QString |
 * | propertyC | Qt::UserRole + 3 | bool    |
 * +-----------+------------------+---------+
 * 
 * A hard-coded \c modelData role is always exposed as \c Qt::UserRole, followed
 * by each property on \c Foo in order of declaration. 
 *
 * Both data and setData can be called respectively on these roles.
 * Likewise, every time a property updates, the notify signal will be consumed
 * and this model will emit a dataChanged signal.
 * 
 * The class definition for a given QObject must have
 * a constructor of form `Q_INVOKABLE Foo(QObject* parent = nullptr)`.
 * An ElementType that does not have this form of constructor will trigger
 * undefined behaviour when insertRows is called.
 * 
 * \sa GenericTableProxyModel
 */
class GenericListModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
  /*!
   * \brief Constructor
   * \param parent Owning parent QObject.
   */
  explicit Q_INVOKABLE GenericListModel(QObject* parent = nullptr);

  /*!
   * \brief Constructor
   * \param elementType the QMetaObject that defines the roles this object will
   * expose.
   * \param parent Owning parent QObject.
   */
  GenericListModel(const QMetaObject* elementType, QObject* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~GenericListModel() override;

  /*!
   * \brief Set the QMetaObject that defines all the roles this model will
   * expose.
   * 
   * This function will reset the model and delete the current model
   * contents if changed.
   * 
   * \param elementType A QMetaObject with properties to expose.
   */
  void setElementType(const QMetaObject* elementType);

  /*!
   * \brief Return the QMetaObject which dictates all the roles this model
   * exposes.
   * \return A QMetaObject with the desired properties.
   */
  const QMetaObject* elementType() const;

  /*!
   * \brief The name of the property which is to be exposed as both the
   * display role and the edit role.
   * 
   * When data or setData is called with \c Qt::DisplayRole or \c Qt::EditRole
   * as the given role, then the underlying property as defined by this setter
   * will be read from or set to.
   * 
   * This property is reset if `setElementType` is called.
   * 
   * \param propertyName name of property to expose as DisplayRole and EditRole.
   */
  void setDisplayPropertyName(const QString& propertyName);

  /*!
   * \brief Returns the name of the property which has been elevated to be used
   * as the Qt::DisplayRole and Qt::EditRole in this model.
   * \return name of property.
   */
  QString displayPropertyName();

  /*!
   * \param parent Unused for lists.
   * \return int Returns the number of objects in the list if parent is an
   * invalid QModelIndex, otherwise returns 0.
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /*!
   * \brief For a given index in the model, returns the data read from that
   * object's property and returns the data as a QVariant.
   * 
   * If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property read
   * from is the property with the name defined by the displayPropertyName
   * field.
   * 
   * If the role is \c Qt::UserRole, then the QVariant return is the pointer
   * to the underlying QObject in the list.
   * 
   * If the role is is any value greater than Qt::UserRole, then the role is
   * mapped to the property in the object associated with that role. See
   * roleNames to get a list of those properties.
   * 
   * \param index The index of the object in the model to query
   * \param role The role to query data for which maps to properties
   * \return QVariant The data as read from the property cast as a QVariant.
   * \sa roleNames
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /*!
   * \brief For a given index in the model, attempts to set the data in the
   * corresponding property to value and returns whether the write succeeded or
   * not.
   * 
   * If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property written
   * to is the property with the name defined by the displayPropertyName
   * field.
   * 
   * If the role is \c Qt::UserRole, then we attempt to cast the QVariant to
   * the type defined by elementType, and replace the object in the model with
   * this new object.
   * 
   * If the role is is any value greater than Qt::UserRole, then the role is
   * mapped to the property in the object associated with that role. See
   * roleNames to get a list of those properties.
   * 
   * \param index QObject in list to write to.
   * \param role The role to map to a property in the QObject.
   * \param value Value written to the property in the QObject.
   * \return true The QMetaProperty::write call succeeded.
   * \return false The QMetaProperty::write call failed.
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /*!
   * \brief A collection of role names and the corresponding user role enum. 
   * 
   * This will always return the hard-coded (name, role) combination 
   * `(modelData, Qt::UserRole)`.
   * 
   * For each subsequent property it will also expose:
   * `(property_N, Qt::UserRole + N + 1)` where `property_N` is the Nth 
   * property in the QMetaObject.
   * 
   * I.E. for the `Foo` example given in the class documentation we would 
   * return:
   * 
   * +-----------+------------------+
   * |    Name   |      Value       |
   * +-----------+------------------+
   * | modelData | Qt::UserRole     |
   * | propertyA | Qt::UserRole + 1 |
   * | propertyB | Qt::UserRole + 2 |
   * | propertyC | Qt::UserRole + 3 |
   * +-----------+------------------+
   * 
   * \sa elementType
   */
  QHash<int, QByteArray> roleNames() const override;
  
  /*!
   * \brief Default constructs count new objects and appends them to this list.
   * 
   * For this function work, the type as defined by elementType must have the
   * following constructor:
   *
   * ``` 
   * Q_INVOKABLE MyClass(QObject* parent = nullptr);
   * ```
   * 
   * Otherwise behaviour is undefined.
   * 
   * \param row Row to start insertion at.
   * \param count Number of new objects to create and insert.
   * \param parent Not used for lists.
   * \return true If creation was successful.
   * \return false  If creation was not successful.
   */
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  /*!
   * \brief Removes and deletes the range of objects defined by row and count.
   * 
   * \param row Start index for deletion.
   * \param count Number of objects to delete.
   * \param parent Not used for lists.
   * \return true If deletion was successful.
   * \return false If deletion was not successful.
   */
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

  /*!
   * \brief Returns header data for the list.
   * 
   * The horizontal header is the class-name of the elementType. 
   * 
   * Vertical header is the section numbering.
   * 
   * \param section Section to query
   * \param orientation Horizontal or vertical header.
   * \param role Role to query.
   * \return Header data.
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  /*!
   * \brief Helper function append an additional object to this list.
   * 
   * The append will fail if object is null.
   * The append will fail if the object's MetaType does not match elementType.
   * 
   * \param object Object to append to this model.
   * \return true If object was appended.
   * \return false If object was not appended.
   */
  Q_INVOKABLE bool append(QList<QObject*> object);

  /*!
   * \brief Helper function append additional objects to this list.
   * \param objects List of object to append to this model.
   * 
   * The append will fail if any object is null.
   * The append will fail if any objects' MetaType does not match elementType.
   * 
   * \return true If objects were appended.
   * \return false If objects were not appended.
   */
  Q_INVOKABLE bool append(QObject* object);

  /*!
   * \brief Helper function to grab the object at index and cast to type T.
   * \tparam T The type to cast to.
   * \param index Index of element to grab.
   * \return Cast of object in list to T. Null is returned if the cast fails.
   */
  template <typename T>  
  T* element(const QModelIndex& index) const
  {
    static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
    return qvariant_cast<T*>(data(index, Qt::UserRole));
  }

signals:
  /*! Emitted when the size of the size of the list changes. */
  void countChanged();

private:
  /*!
   * \internal 
   * \brief Given a QModelIndex, connect up to the underlying QObject.
   * 
   * We connect up to the destroyed signal on the QObject so that we can
   * automatically remove the object from this list.
   * 
   * We also connect up to each notify signal on each property, so we can
   * emit a dataChanged() signal each time a property informs us of an update.
   * 
   * \param index Index of item in the model.
   */
  void connectElement(QModelIndex index);

  /*!
   * \internal
   * \brief Returns the size of the list for the count property.
   * \note Use rowCount in C++ code. This is a QML convenience property.
   */
  int count() const;

private:
  int m_displayPropIndex = -1;
  const QMetaObject* m_elementType = nullptr;
  QList<QObject*> m_objects;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_GENERICLISTMODEL_H
