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

namespace Esri::ArcGISRuntime::Toolkit {

class MetaElement : public QObject
{
  Q_OBJECT
public:
  MetaElement(QModelIndex index, int customRole, QObject* trackedObject, QAbstractItemModel* parent);

signals:
  void propertyChanged();

private slots:
  void emitDataChanged();

private:
  QPersistentModelIndex m_index;
  int m_customRole = -1;
  QPointer<QObject> m_trackedObject;
  QAbstractItemModel* m_parentModel = nullptr;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_METAELEMENT_H
