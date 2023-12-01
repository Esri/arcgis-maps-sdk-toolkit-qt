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
#include "GenericListModel.h"

#include "MetaElement.h"

#include <QMetaProperty>
#include <QPointer>
namespace Esri::ArcGISRuntime::Toolkit {

  /*!
 \inmodule EsriArcGISRuntimeToolkit
 \class Esri::ArcGISRuntime::Toolkit::GenericListModel

 \brief This ListModel automatically exposes a list of QObjects with
 properties as a list of elements where each property is mapped to a user
 role.

 A GenericListModel addresses the need to take a collection of properties,
 on a QObject and expose them in a view (Widgets or QML).

 A GenericListModel takes a QMetaObject as an `elementType`. Each property in
 the QMetaObject is scanned and assigned a role in the order in which the
 property was declared in the class.

 For example, given a class Foo:

 \code
 class Foo : public QObject
 {
   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA NOTIFY propertyAChanged)
   Q_PROPERTY(QString propertyB READ propertyB NOTIFY propertyBChanged)
   Q_PROPERTY(bool propertyC READ propertyC CONSTANT)
 };
 \endcode

 Then a GenericListModel will expose the following roles:

 \table
  \header
    \li Name
    \li Value
    \li Type
  \row
    \li listData
    \li \c{Qt::UserRole}
    \li \c{Foo*}
  \row
    \li propertyA
    \li \c{ Qt::UserRole + 1 }
    \li \c {int}
  \row
    \li propertyB
    \li \c{ Qt::UserRole + 2 }
    \li \c {QString}
  \row
    \li propertyC
    \li \c{ Qt::UserRole + 3}
    \li \c {bool}
 \endtable

 A hard-coded \e{listData} role is always exposed as \c Qt::UserRole,
 followed by each property on \c Foo in order of declaration.

 Both \c data and \c setData can be called respectively on these roles.
 Likewise, every time a property updates, the notify signal will be consumed
 and this model will emit a dataChanged signal.

 The class definition for a given \c QObject must have
 a constructor of form \c{Q_INVOKABLE Foo(QObject* parent = nullptr)}.
 An ElementType that does not have this form of constructor will trigger
 undefined behaviour when insertRows is called.
 */

  /*!
  \brief Constructor
  \list
  \li \a parent Owning parent QObject.
  \endlist
 */
  GenericListModel::GenericListModel(QObject* parent) :
    GenericListModel(nullptr, parent)
  {
  }

  /*!
  \brief Constructor
  \list
  \li \a elementType the QMetaObject that defines the roles this object will
  expose.
  \li \a parent Owning parent QObject.
  \endlist
 */
  GenericListModel::GenericListModel(const QMetaObject* elementType, QObject* parent) :
    QAbstractListModel(parent),
    m_elementType(elementType)
  {
    connect(this, &GenericListModel::rowsInserted, this, &GenericListModel::countChanged);
    connect(this, &GenericListModel::rowsRemoved, this, &GenericListModel::countChanged);

    // We connect to our own `dataChanged` signal. We test to see if the properties
    // that are updating are also the same properties associated with our
    // Qt::DisplayRole and Qt::EditRole or Qt::DecorationRole or Qt::ToolTipRole. If so then we need to emit dataChanged
    // for these roles as well if they are not already emitting.
    connect(this, &GenericListModel::dataChanged, this,
            [this](const QModelIndex& topLeft, const QModelIndex& bottomRight,
                   const QVector<int>& roles)
            {
              if (m_displayPropIndex < 0)
                return;

              if (!m_elementType)
                return;

              if (roles.contains(Qt::DisplayRole))
                return;

              const auto offset = m_elementType->propertyOffset();
              if (roles.contains(m_displayPropIndex - offset + Qt::UserRole + 1))
                emit dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DisplayRole << Qt::EditRole);
              if (roles.contains(m_decorationPropIndex - offset + Qt::UserRole + 1))
                emit dataChanged(topLeft, bottomRight, QVector<int>() << Qt::DecorationRole);
              if (roles.contains(m_tooltipPropIndex - offset + Qt::UserRole + 1))
                emit dataChanged(topLeft, bottomRight, QVector<int>() << Qt::ToolTipRole);
            });
  }

  /*!
  \brief Destructor.
 */
  GenericListModel::~GenericListModel()
  {
  }

  /*!
  \list
    \li \a parent Unused for lists.
  \endlist
  Returns the number of objects in the list if parent is an
  invalid QModelIndex, otherwise returns 0.
 */
  int GenericListModel::rowCount(const QModelIndex& parent) const
  {
    if (parent.isValid())
      return 0;
    else
      return m_objects.size();
  }

  /*!
  \brief For a given index in the model, returns the data read from that
  object's property and returns the data as a \c QVariant.

  If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property read
  from is the property with the name defined by the \e displayPropertyName
  field.

  If the role is \c Qt::UserRole, then the \c QVariant return is the pointer
  to the underlying QObject in the list.

  If the role is is any value greater than \c Qt::UserRole, then the role is
  mapped to the property in the object associated with that role. See
  \l roleNames to get a list of those properties.

  \list
    \li \a index The index of the object in the model to query
    \li \a role The role to query data for which maps to properties
  \endlist

  Returns the data as read from the property cast as a QVariant.

  \sa roleNames
 */
  QVariant GenericListModel::data(const QModelIndex& index, int role) const
  {
    if (!m_elementType)
      return QVariant();
    else if (!index.isValid())
      return QVariant();

    auto o = m_objects.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      const auto property = m_elementType->property(m_displayPropIndex);
      return property.read(o);
    }
    else if (role == Qt::DecorationRole)
    {
      const auto property = m_elementType->property(m_decorationPropIndex);
      auto p = property.read(o);
      return p;
    }
    else if (role == Qt::ToolTipRole)
    {
      const auto property = m_elementType->property(m_tooltipPropIndex);
      auto p = property.read(o);
      return p;
    }
    else if (role == Qt::UserRole)
    {
      return QVariant::fromValue(o);
    }
    else if (role >= Qt::UserRole)
    {
      const auto propIndex = role - (Qt::UserRole + 1) + m_elementType->propertyOffset();
      const auto property = m_elementType->property(propIndex);
      return property.read(o);
    }

    return QVariant();
  }

  /*!
  \brief For a given index in the model, attempts to set the data in the
  corresponding property to value and returns whether the write succeeded or
  not.

  If role is \c Qt::DisplayRole or \c Qt::EditRole, then the property written
  to is the property with the name defined by the displayPropertyName
  field.

  If the role is \c Qt::UserRole, then we attempt to cast the QVariant to
  the type defined by elementType, and replace the object in the model with
  this new object.

  If the role is is any value greater than \c Qt::UserRole, then the role is
  mapped to the property in the object associated with that role. See
  roleNames to get a list of those properties.

  \list
  \li \a index QObject in list to write to.
  \li \a role The role to map to a property in the QObject.
  \li \a value Value written to the property in the QObject.
  \endlist

  Returns \c true if the QMetaProperty::write call succeeded.
 */
  bool GenericListModel::setData(const QModelIndex& index, const QVariant& value, int role)
  {
    if (!m_elementType)
      return false;
    else if (!index.isValid())
      return false;

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      auto o = m_objects.at(index.row());
      const auto property = m_elementType->property(m_displayPropIndex);
      return property.write(o, value);
    }
    else if (role == Qt::UserRole)
    {
      auto newObject = qvariant_cast<QObject*>(value);
      if (newObject && newObject->metaObject()->inherits(m_elementType))
      {
        m_objects[index.row()] = newObject;
        emit dataChanged(index, index);
        return true;
      }
      else
      {
        return false;
      }
    }
    else if (role > Qt::UserRole)
    {
      auto o = m_objects.at(index.row());
      const auto propIndex = role - (Qt::UserRole + 1) + m_elementType->propertyOffset();
      const auto property = m_elementType->property(propIndex);
      return property.write(o, value);
    }

    return false;
  }

  /*!
  \brief A collection of role names and the corresponding user role enum.

  This will always return the hard-coded \e{(name, role)} combination
  \c{(listData, Qt::UserRole)}.

  For each subsequent property it will also expose:
  \c{(property_N, Qt::UserRole + N + 1)} where \tt{property_N} is the N\sup{th}
  property in the QMetaObject.

  I.E. for the \c Foo example given in the class documentation we would
  return:

  \table
   \header
     \li Name
     \li Value
   \row
     \li listData
     \li \c{Qt::UserRole}
   \row
     \li propertyA
     \li \c{ Qt::UserRole + 1 }
   \row
     \li propertyB
     \li \c{ Qt::UserRole + 2 }
     \li \c {QString}
   \row
     \li propertyC
     \li \c{ Qt::UserRole + 3}
  \endtable

  \sa elementType
 */
  QHash<int, QByteArray> GenericListModel::roleNames() const
  {
    if (!m_elementType)
      return {};

    QHash<int, QByteArray> output;
    output.insert(Qt::UserRole, "listData");

    const int offset = m_elementType->propertyOffset();
    for (int i = offset; i < m_elementType->propertyCount(); ++i)
    {
      // Each property has a name and an index which starts from an arbitrary
      // offset. So we have properties at indices: OFFSET, OFFSET+1, OFFSET+2....
      // We map OFFSET to, Qt::UserRole+1, OFFSET+1 to Qt::UserRole+2 and so on.
      auto property = m_elementType->property(i);
      output.insert(i - offset + Qt::UserRole + 1, property.name());
    }
    return output;
  }

  /*!
  \brief Default constructs count new objects and appends them to this list.

  For this function work, the type as defined by elementType must have the
  following constructor:

  \code
  Q_INVOKABLE MyClass(QObject* parent = nullptr);
  \endcode

  Otherwise behaviour is undefined.

  \list
  \li \a row Row to start insertion at.
  \li \a count Number of new objects to create and insert.
  \li \a parent Not used for lists.
  \endlist

  Returns true If creation was successful.
 */
  bool GenericListModel::insertRows(int row, int count, const QModelIndex& parent)
  {
    if (!m_elementType)
      return false;
    else if (m_elementType->constructorCount() < 1)
      return false;
    if (parent.isValid())
      return false;
    else if (count < 1)
      return false;
    else if (row < 0)
      return false;
    else if (row > m_objects.size())
      return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
      m_objects.insert(row, m_elementType->newInstance(Q_ARG(QObject*, this)));
    }
    endInsertRows();

    for (int i = row; i < count; ++i)
    {
      connectElement(index(row));
    }
    return true;
  }

  /*!
  \brief Removes and deletes the range of objects defined by row and count.

  \list
  \li \a row Start index for deletion.
  \li \a count Number of objects to delete.
  \li \a parent Not used for lists.
  \endlist

  Returns \c true if deletion was successful.
 */
  bool GenericListModel::removeRows(int row, int count, const QModelIndex& parent)
  {
    if (parent.isValid())
      return false;
    else if (count < 0)
      return false;
    else if (row < 0)
      return false;
    else if (row >= rowCount(parent))
      return false;
    else if (row + count > rowCount(parent))
      return false;
    else if (count == 0)
      return true; // A valid no-op.

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = count - 1; i >= row; --i)
    {
      auto o = m_objects.at(i);

      // Ensure additional removal signals are not triggered.
      if (o && o->parent() == this)
      {
        disconnect(o, &QObject::destroyed, this, nullptr);
        delete o;
      }

      m_objects.removeAt(i);
    }
    endRemoveRows();
    return true;
  }

  /*!
  \brief Set the QMetaObject that defines all the roles this model will
  expose.

  This function will reset the model and delete the current model
  contents if changed.

  \list
  \li \a metaObject A QMetaObject with properties to expose.
  \endlist
 */
  void GenericListModel::setElementType(const QMetaObject* metaObject)
  {
    beginResetModel();
    for (auto o : m_objects)
    {
      // Ensure additional removal signals are not triggered.
      if (o && o->parent() == this)
      {
        disconnect(o, &QObject::destroyed, this, nullptr);
        delete o;
      }
    }
    m_objects.clear();
    m_elementType = metaObject;
    m_displayPropIndex = -1;
    endResetModel();
  }

  /*!
   \brief Return the QMetaObject which dictates all the roles this model
   exposes.

   Returns a QMetaObject with the desired properties.
 */
  const QMetaObject* GenericListModel::elementType() const
  {
    return m_elementType;
  }

  /*!
  \brief The name of the property which is to be exposed as both the
  display role and the edit role.

  When data or setData is called with \c Qt::DisplayRole or \c Qt::EditRole
  as the given role, then the underlying property as defined by this setter
  will be read from or set to.

  This property is reset if \l setElementType is called.

  \list
   \li \a propertyName name of property to expose as DisplayRole and EditRole.
  \endlist
 */
  void GenericListModel::setDisplayPropertyName(const QString& propertyName)
  {
    m_displayPropIndex = m_elementType->indexOfProperty(propertyName.toLatin1());
  }

  void GenericListModel::setDecorationPropertyName(const QString& propertyName)
  {
    m_decorationPropIndex = m_elementType->indexOfProperty(propertyName.toLatin1());
  }

  void GenericListModel::setTooltipPropertyName(const QString& propertyName)
  {
    m_tooltipPropIndex = m_elementType->indexOfProperty(propertyName.toLatin1());
  }
  /*!
  \brief Returns the name of the property which has been elevated to be used
  as the \c Qt::DisplayRole and Qt::EditRole in this model.

  Returns name of property.
 */
  QString GenericListModel::displayPropertyName()
  {
    if (m_displayPropIndex < 0)
      return "";

    return m_elementType->property(m_displayPropIndex).name();
  }

  /*!
  \brief Returns the name of the property which has been elevated to be used
  as the \c Qt::DecorationRole in this model.

  Returns name of property.
 */
  QString GenericListModel::decorationPropertyName()
  {
    if (m_decorationPropIndex < 0)
      return "";

    return m_elementType->property(m_decorationPropIndex).name();
  }

  /*!
  \brief Returns the name of the property which has been elevated to be used
  as the \c Qt::ToolTipRole in this model.

  Returns name of property.
 */
  QString GenericListModel::tooltipPropertyName()
  {
    if (m_tooltipPropIndex < 0)
      return "";

    return m_elementType->property(m_tooltipPropIndex).name();
  }

  /*!
  \brief Helper function append an additional object to this list.

  The append will fail if \a object is null.
  The append will fail if the MetaType of \a object does not match elementType.

  \list
  \li \a object Object to append to this model.
  \endlist

  Returns \c true if object was appended.
 */
  bool GenericListModel::append(QObject* object)
  {
    if (!m_elementType)
      return false;

    if (!object)
      return false;

    if (!object->metaObject()->inherits(m_elementType))
      return false;

    auto i = rowCount();
    beginInsertRows(QModelIndex(), i, i);
    m_objects << object;
    endInsertRows();
    connectElement(index(i));
    return true;
  }

  /*!
  \brief Helper function append additional objects to this list.

  \list
  \li \a objects List of object to append to this model.
  \endlist

  The append will fail if any object is null.
  The append will fail if any objects' MetaType does not match elementType.

  Return \c true If objects were appended.
 */
  bool GenericListModel::append(QList<QObject*> objects)
  {
    const auto size = objects.size();

    if (!m_elementType)
      return false;
    if (size < 1)
      return true;

    for (auto o : objects)
    {
      if (!o)
        return false;
      else if (!o->metaObject()->inherits(m_elementType))
        return false;
    }

    auto i = rowCount();

    beginInsertRows(QModelIndex(), i, i + size - 1);
    m_objects << objects;
    endInsertRows();

    for (; i < size; ++i)
      connectElement(index(i));

    return true;
  }

  /*!
  \internal

  \brief Given a \c QModelIndex, connect up to the underlying \c QObject.

  We connect up to the destroyed signal on the \c QObject so that we can
  automatically remove the object from this list.

  We also connect up to each notify signal on each property, so we can
  emit a \l dataChanged signal each time a property informs us of an update.

  \list
  \li \a index Index of item in the model.
  \endlist
 */
  void GenericListModel::connectElement(QModelIndex index)
  {
    if (!m_elementType || !index.isValid())
      return;

    QObject* object = m_objects.at(index.row());

    if (!object || !object->metaObject()->inherits(m_elementType))
      return;

    // If object is deleted externally we remove from the model.
    connect(object, &QObject::destroyed, this,
            [this, pIndex = QPersistentModelIndex(index)]
            {
              if (!pIndex.isValid())
                return;

              const auto row = pIndex.row();

              if (row < 0 || row >= m_objects.size())
                return;

              m_objects[row] = nullptr; // Prevents double delete.
              removeRow(row, pIndex.parent());
            });

    // Connect to each property notifySignal and hook up to our dataChanged signal
    // using MetaElement objects as drop-in replacements for lambdas.
    const auto offset = m_elementType->propertyOffset();
    for (int i = offset; i < m_elementType->propertyCount(); ++i)
    {
      auto property = m_elementType->property(i);
      if (property.hasNotifySignal())
      {
        auto element = new MetaElement(
            index,
            i - offset + Qt::UserRole + 1,
            object,
            this);

        // Signal to signal connection.
        connect(object, property.notifySignal(),
                element, QMetaMethod::fromSignal(&MetaElement::propertyChanged));
      }
    }
  }

  /*!
  \internal
  \brief Returns the size of the list for the count property.
  \note Use rowCount in C++ code. This is a QML convenience property.
 */
  int GenericListModel::count() const
  {
    return m_objects.size();
  }

  /*!
  \brief Returns header data for the list.

  The horizontal header is the class-name of the elementType.

  Vertical header is the section numbering.

  \list
  \li \a section Section to query
  \li \a orientation Horizontal or vertical header.
  \li \a role Role to query.
  \endlist

  Returns the header data as a \c QVariant.
 */
  QVariant GenericListModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (!m_elementType)
      return QVariant();

    if (role != Qt::DisplayRole)
      return QVariant();

    if (Qt::Orientation::Vertical == orientation)
      return section + 1;
    else if (Qt::Orientation::Horizontal && section == 0)
      return QString(m_elementType->className());
    else
      return "";
  }

    /*!
  \brief Overridden \l QAbstractListModel function. Returns the \l Qt::ItemFlags for each item in the list.

  If \l setFlagsCallback is set, Qt::ItemsFlags are returned from the call-back. Otherwise, the default \l QAbstractListModel::flags will be called.
  Flags returned are used from the \l QListView to apply visual properties.

  \list
  \li \a index Index of item
  \endlist
  \sa Qt::ItemFlags QFlags
  */
  Qt::ItemFlags GenericListModel::flags(const QModelIndex& index) const
  {
    if (m_flagsCallback)
      return m_flagsCallback(index);
    // call default base class .flags()
    return QAbstractItemModel::flags(index);
  }

  QObject* GenericListModel::element(const QModelIndex& index)
  {
    return qvariant_cast<QObject*>(data(index, Qt::UserRole));
  }

  bool GenericListModel::clear()
  {
    return removeRows(0, count());
  }

  /*!
   \typedef Esri::ArcGISRuntime::Toolkit::GenericListModel::FlagsCallback
  This is a typedef for a pointer to a function with the following signature
  \code
    QFlags<Qt::ItemFlag> myFlagsCallback(const QModelIndex& index);
  \endcode

  */

  /*!
  \fn template<typename Func> void Esri::ArcGISRuntime::Toolkit::GenericListModel::setFlagsCallback(Func&& f)
  \brief Template member function used to set the callback function which calculates each item \c Qt::ItemFlags.

  \list
  \li \c Func Signature type that is accepted. Should be implicitly convertible to \l Esri::ArcGISRuntime::Toolkit::GenericListModel::FlagsCallback.
  \li \a f Function which handles the return of \c Qt::ItemFlags for each item in the collection \c QList<QObject*>
  \endlist
  */

  /*!
  \fn T* Esri::ArcGISRuntime::Toolkit::GenericListModel::element(const QModelIndex& index) const

  \brief Helper function to grab the object at index and cast to type T.

  \list
  \li \c T The type to cast to.
  \li \a index Index of element to grab.
  \endlist

  Returns the cast of object in list to \c T. Nullptr is returned if the cast
  fails.
 */

  /*!
  \fn void Esri::ArcGISRuntime::Toolkit::GenericListModel::countChanged()
  \brief Emitted when the size of the list changes.
 */

  /*!
  \property Esri::ArcGISRuntime::Toolkit::GenericListModel::count
  \brief The current number of elements in this list model.
 */

} // Esri::ArcGISRuntime::Toolkit
