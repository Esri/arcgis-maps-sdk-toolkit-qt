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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_COORDIANTEEDITDELEGATE_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_COORDIANTEEDITDELEGATE_H

// Qt headers
#include <QItemDelegate>
#include <QPointer>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionController;

class CoordinateEditDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  CoordinateEditDelegate(QObject* parent = nullptr);

  ~CoordinateEditDelegate() override;

  void setController(CoordinateConversionController* c);

  CoordinateConversionController* controller() const;

  void setModelData(QWidget* editor, QAbstractItemModel* model,
                    const QModelIndex& index) const override;

private:
  QPointer<CoordinateConversionController> m_controller;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_COORDIANTEEDITDELEGATE_H
