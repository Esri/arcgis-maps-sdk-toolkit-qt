/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_USERCREDENTIALVIEW_H
#define ESRI_ARCGISRUNTIME_USERCREDENTIALVIEW_H

#include <QPointer>
#include <QWidget>

// Toolkit headers.
#include "AuthenticationController.h"

namespace Ui {
  class UserCredentialView;
}

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class UserCredentialView : public QWidget
  {
    Q_OBJECT
  public:
    explicit UserCredentialView(AuthenticationController* controller, QWidget* parent = nullptr);
    ~UserCredentialView() override;

  private slots:
    void acceptWithUsernamePassword();
    void enableOkButton();

  private:
    QPointer<AuthenticationController> m_controller = nullptr;
    Ui::UserCredentialView* m_ui = nullptr;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_USERCREDENTIALVIEW_H
