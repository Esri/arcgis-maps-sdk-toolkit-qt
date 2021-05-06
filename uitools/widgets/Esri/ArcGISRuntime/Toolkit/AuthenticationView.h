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
#ifndef ESRI_ARCGISRUNTIME_AUTHENTICATIONVIEW_H
#define ESRI_ARCGISRUNTIME_AUTHENTICATIONVIEW_H

#include <QDialog>

namespace Ui
{
class AuthenticationView;
}

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

class AuthenticationController;

class AuthenticationView : public QDialog 
{
  Q_OBJECT
public:
  explicit AuthenticationView(AuthenticationController* controller, QWidget* parent = nullptr);
  explicit AuthenticationView(QWidget* parent = nullptr);
  ~AuthenticationView() override;

  AuthenticationController* controller() const;
  void setController(AuthenticationController* controller);

signals:
  void authenticationControllerChanged();

private:
  AuthenticationController* m_controller = nullptr;
  Ui::AuthenticationView* m_ui = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_AUTHENTICATIONVIEW_H
