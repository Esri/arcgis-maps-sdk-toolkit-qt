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
#include "UserCredentialView.h"
#include "ui_UserCredentialView.h"

#include "AuthenticationController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \brief Constructor.
  \list
    \li \a parent Parent widget.
  \endlist
 */
UserCredentialView::UserCredentialView(AuthenticationController* controller, QWidget* parent) :
  QWidget(parent),
  m_controller(controller),
  m_ui(new Ui::UserCredentialView)
{
  m_ui->setupUi(this);
  if (m_controller && m_controller->currentChallengeFailureCount() < 2)
  {
    m_ui->incorrectMessage->setVisible(false);
  }

  connect(m_ui->usernameEdit, &QLineEdit::returnPressed, this, &UserCredentialView::acceptWithUsernamePassword);
  connect(m_ui->passwordEdit, &QLineEdit::returnPressed, this, &UserCredentialView::acceptWithUsernamePassword);
  connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &UserCredentialView::acceptWithUsernamePassword);
  connect(m_ui->buttonBox, &QDialogButtonBox::rejected,
          [this]
          {
            m_controller ? m_controller->cancel() : void();
          });
}

UserCredentialView::~UserCredentialView()
{
  delete m_ui;
}

void UserCredentialView::acceptWithUsernamePassword()
{
  if (!m_controller)
  {
    return;
  }
  m_controller->continueWithUsernamePassword(m_ui->usernameEdit->text(), m_ui->passwordEdit->text());
}

} // Toolkit
} // ArcGISRuntime
} // Esri
