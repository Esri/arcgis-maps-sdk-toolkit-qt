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
#include "ClientCertificateView.h"

#include "ClientCertificatePasswordDialog.h"
#include "ui_ClientCertificateView.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QStringListModel>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  /*!
  \brief Constructor.
  \list
    \li \a parent Parent widget.
  \endlist
 */
  ClientCertificateView::ClientCertificateView(AuthenticationController* controller, QWidget* parent) :
    QWidget(parent),
    m_controller(controller),
    m_ui(new Ui::ClientCertificateView)
  {
    Q_ASSERT(m_controller);
    m_ui->setupUi(this);

    // Show the appropriate certificate preable.
    m_ui->preamble->setText(tr("The service has requested a client certificate to authenticate you. "
                               "The app has identified the requesting server as "
                               "'%1', but you "
                               "should only give the app access to the certificate if you trust it.")
                                .arg(m_controller->currentAuthenticatingHost()));

    // Setup ListView to show live client certificate infos.
    auto model = new QStringListModel(m_controller->clientCertificateInfos(), this);
    connect(m_controller, &AuthenticationController::clientCertificateInfosChanged, model, [this, model]()
            {
              model->setStringList(m_controller->clientCertificateInfos());
            });
    m_ui->certificatesView->setModel(model);

    // Setup the Ok button to only enable when a certificate is selected.
    auto ok_button = m_ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
    Q_ASSERT(ok_button);
    ok_button->setEnabled(false);
    auto selection_model = m_ui->certificatesView->selectionModel();
    selection_model->clearSelection();
    connect(selection_model, &QItemSelectionModel::currentChanged, ok_button,
            [ok_button](const QModelIndex& current)
            {
              ok_button->setEnabled(current.isValid());
            });

    // Add a client certificate on request.
    // Note: uses queued connection to not block the pressed signal with a
    // modal dialog.
    connect(
        m_ui->addCertificateButton, &QPushButton::pressed, this,
        [this]
        {
          if (!m_controller)
            return;

          auto fileUrl = QFileDialog::getOpenFileUrl(
              this,
              tr("Add certificate"),
              QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation));

          if (fileUrl.isEmpty())
          {
            return;
          }
          m_controller->addClientCertificate(fileUrl);
        },
        Qt::QueuedConnection);

    // Prompt for a password when required.
    // Note: uses queued connection to not block the pressed signal with a
    // modal dialog.
    connect(
        m_controller, &AuthenticationController::clientCertificatePasswordRequired, this,
        [this](QUrl certificate)
        {
          auto dialog = new ClientCertificatePasswordDialog(std::move(certificate), m_controller, this);
          dialog->exec();
        },
        Qt::QueuedConnection);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this,
            [this, selection_model]
            {
              auto index = selection_model->currentIndex();
              if (m_controller && index.isValid())
              {
                m_controller->continueWithClientCertificate(index.row());
              }
            });

    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this,
            [this]
            {
              if (m_controller)
                m_controller->cancel();
            });
  }

  ClientCertificateView::~ClientCertificateView()
  {
    delete m_ui;
  }

} // Toolkit
} // ArcGISRuntime
} // Esri
