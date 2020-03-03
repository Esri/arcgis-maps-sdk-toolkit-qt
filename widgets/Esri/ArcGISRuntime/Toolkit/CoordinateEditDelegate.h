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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEEDITDELEGATE_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEEDITDELEGATE_H

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

/*!
 * \internal
 * \brief This is an implementation detail of table-view editing for the
 * CoordinateConversion tool.
 * 
 * When a coordinate is edited by the user, this tool triggers an update of the
 * current point on the controller, as opposed to just editing the value in
 * the cell as would normally happen.
 */
class CoordinateEditDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  /*!
   * \brief Constructor
   * \param parent Owning parent object.
   */
  CoordinateEditDelegate(QObject* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~CoordinateEditDelegate() override;

  /*!
   * \brief Set the Controller object that will be updated when a user commits
   * an edit.
   * \param c Controller to update on an edit completing. 
   */
  void setController(CoordinateConversionController* c);

  /*!
   * \brief Returns the controller object that will be updated when the user
   * commits an edit.
   */
  CoordinateConversionController* controller() const;

  /*!
   * \brief Called when the user commits data to the model.
   * \param editor Editing widget
   * \param model Model to update.
   * \param index Index in model to update.
   */
  void setModelData(QWidget* editor, QAbstractItemModel* model,
                    const QModelIndex& index) const override;

private:
  QPointer<CoordinateConversionController> m_controller;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEEDITDELEGATE_H
