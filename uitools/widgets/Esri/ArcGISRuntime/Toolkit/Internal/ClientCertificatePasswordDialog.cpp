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
#include "ClientCertificatePasswordDialog.h"

#include "ui_ClientCertificatePasswordDialog.h"

namespace Esri::ArcGISRuntime::Toolkit {

  /*!
  \brief Constructor.
  \list
    \li \a parent Parent widget.
  \endlist
 */
  ClientCertificatePasswordDialog::ClientCertificatePasswordDialog(QUrl certificateFile, AuthenticationController* controller, QWidget* parent) :
    QDialog(parent),
    m_certificateFile(std::move(certificateFile)),
    m_controller(controller),
    m_ui(new Ui::ClientCertificatePasswordDialog)
  {
    Q_ASSERT(m_controller);
    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->preamble->setText(
        tr("The client certificate file '%1' requires a password to open.")
            .arg(m_certificateFile.fileName()));

    connect(m_controller, &QObject::destroyed, this,
            [this]
            {
              close();
            });

    connect(this, &QDialog::accepted, this,
            [this]
            {
              if (m_controller)
                m_controller->addClientCertificate(m_certificateFile, m_ui->passwordEdit->text());
            });
  }

  ClientCertificatePasswordDialog::~ClientCertificatePasswordDialog()
  {
    delete m_ui;
  }

} // Esri::ArcGISRuntime::Toolkit
