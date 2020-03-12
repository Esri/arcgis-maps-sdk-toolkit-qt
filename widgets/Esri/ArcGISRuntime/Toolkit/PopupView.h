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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEW_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEW_H

#include <QWidget>

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

class PopupViewController;

class PopupView : public QFrame
{
  Q_OBJECT
public:
  /*!
   * \brief Constructor
   * \param parent Parent widget.
   */
  explicit PopupView(QWidget* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~PopupView() override;

  /*!
   * \brief Returns the controller object driving this widget.
   */
  PopupViewController* controller() const;

private:
  PopupViewController* m_controller = nullptr;
  Ui::PopupView* m_ui = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEW_H
