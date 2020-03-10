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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_METAELEMENT_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_METAELEMENT_H

// Qt headers
#include <QObject>
#include <QPersistentModelIndex>
#include <QPointer>

class QAbstractItemModel;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \internal
 * \brief The MetaElement is an implementation detail of GenericListModel that
 * deals with helping the model emit dataChanged signals when a property
 * updates.
 * 
 * A MetaElement tracks an object in the GenericListModel, and updates for the
 * given stored index/role fields when the `propertyChanged` signal is called.
 * 
 * Qt connection functions do not allow for connecting a lambda to a
 * QMetaMethod. This object takes the role of a lambda to track state
 * information that needs triggered on a property changed signal.
 * \sa GenericListModel
 */
class MetaElement : public QObject
{
  Q_OBJECT
public:
  /*!
   * \brief Construct a new MetaElement.
   * 
   * \param index When emitDataChanged is called, this is the index used to
   * trigger a dataChanged signal on the parent GenericListModel. This is stored
   * internally as a QPersistentModelIndex so the index is correctly tracked.
   * 
   * \param customRole When emitDataChanged is called, this is the role enum
   * used to trigger a dataChanged signal on the parent GenericListModel.
   * Qt::UserRole is always emitted along with this role.
   *      
   * \param trackedObject This is the associated element in the GenericListModel
   * which we are tracking.
   * 
   * \param parent The parent ListModel.
   */
  MetaElement(QModelIndex index, int customRole, QObject* trackedObject, QAbstractItemModel* parent);

signals:
  /*!
   * \brief Signal that when called will trigger the emitDataChanged slot.
   * 
   * In reality this signal takes on the role of a slot as it is invoked by the
   * tracked object's property's notify signal.
   * 
   * This is due to the fact that there is QMetaMethod::fromSignal method but
   * no slot equivalent, so this signal takes on the role of a slot for
   * QMetaMethod connection purposes.
   */
  void propertyChanged();

private slots:
  /*!
   * \brief When triggered will emit a dataChangedSignal on the parent
   * GenericListModel using the stored index and role as cues.
   */
  void emitDataChanged();

private:
  QPersistentModelIndex m_index;
  int m_customRole = -1;
  QPointer<QObject> m_trackedObject;
  QAbstractItemModel* m_parentModel = nullptr;
};

} // Esri
} // ArcGISRuntime
} // Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_METAELEMENT_H
