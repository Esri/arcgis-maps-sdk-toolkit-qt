
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
#include "OAuth2View.h"

#include "ui_OAuth2View.h"

namespace Esri::ArcGISRuntime::Toolkit {

  namespace {
    bool isSuccess(const QString& title)
    {
      return title.indexOf("SUCCESS code=") > -1;
    }

    bool isInvalidRequest(const QString& title)
    {
      return (title.indexOf("Denied error=invalid_request") > -1);
    }

    bool isError(const QString& title)
    {
      return (title.indexOf("Denied error=") > -1) || (title.indexOf("Error: ") > -1);
    }
  }

  /*!
  \internal
 */

  OAuth2View::OAuth2View(AuthenticationController* controller, QWidget* parent) :
    QWidget(parent),
    m_controller(controller),
    m_ui(new Ui::OAuth2View)
  {
    Q_ASSERT(m_controller);
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this,
            [this]
            {
              if (m_controller)
              {
                m_controller->cancel();
              }
            });

    connect(m_ui->webView, &QWebEngineView::titleChanged, this,
            [this](const QString& title)
            {
              setWindowTitle(title);

              if (!m_controller)
              {
                return;
              }

              if (isSuccess(title))
              {
                auto authCode = title;
                authCode.replace("SUCCESS code=", "");
                m_controller->continueWithOAuthAuthorizationCode(authCode);
              }
              else if (isInvalidRequest(title))
              {
                auto self = QPointer<OAuth2View>(this);
                m_ui->webView->page()->toHtml(
                    [self, title](const QString& html)
                    {
                      if (self && self->m_controller)
                        self->m_controller->cancelWithError(title, html);
                    });
              }
              else if (isError(title))
              {
                auto self = QPointer<OAuth2View>(this);
                m_ui->webView->page()->toHtml(
                    [self, title](const QString& html)
                    {
                      if (self && self->m_controller)
                        self->m_controller->cancelWithError(title, html);
                    });
              }
            });

    connect(m_ui->webView, &QWebEngineView::loadFinished, this,
            [this](bool ok)
            {
              if (!ok && m_controller)
              {
                m_controller->cancelWithError("Failed to load", "");
              }
            });
    m_ui->webView->load(m_controller->currentChallengeUrl());
  }

  OAuth2View::~OAuth2View()
  {
    delete m_ui;
  }

} // Esri::ArcGISRuntime::Toolkit
