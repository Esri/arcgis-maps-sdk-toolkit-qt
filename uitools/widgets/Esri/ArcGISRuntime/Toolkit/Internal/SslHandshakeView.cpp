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
#include "SslHandshakeView.h"

#include "ui_SslHandshakeView.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
  \brief Constructor.
  \list
    \li \a parent Parent widget.
  \endlist
 */
  SslHandshakeView::SslHandshakeView(AuthenticationController* controller, QWidget* parent) :
    QWidget(parent),
    m_controller(controller),
    m_ui(new Ui::SslHandshakeView)
  {
    Q_ASSERT(controller);
    m_ui->setupUi(this);

    m_ui->authUrlLabel->setText(m_controller->currentAuthenticatingHost());

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this,
            [this]
            {
              if (m_controller)
                m_controller->continueWithSslHandshake(
                    true, m_ui->rememberBox->isChecked());
            });
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this,
            [this]
            {
              if (m_controller)
                m_controller->continueWithSslHandshake(
                    false, m_ui->rememberBox->isChecked());
            });
  }

  SslHandshakeView::~SslHandshakeView()
  {
    delete m_ui;
  }

} // Esri::ArcGISRuntime::Toolkit
