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
#include "AuthenticationView.h"

#include "ui_AuthenticationView.h"

// Toolkit widget headers
#include "Internal/ClientCertificateView.h"
#include "Internal/OAuth2View.h"
#include "Internal/SslHandshakeView.h"
#include "Internal/UserCredentialView.h"

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  namespace {
    QWidget* createWidgetView(AuthenticationController* controller, AuthenticationView* view)
    {
      Q_ASSERT(controller);
      const auto challengeType = static_cast<AuthenticationChallengeType>(controller->currentChallengeType());

      switch (challengeType)
      {
      case AuthenticationChallengeType::UsernamePassword:
        // ArcGIS token, HTTP Basic/Digest, IWA
        return new UserCredentialView(controller, view);
      case AuthenticationChallengeType::OAuth:
        // OAuth 2
        return new OAuth2View(controller, view);
      case AuthenticationChallengeType::ClientCertificate:
        // Client Certificate
        return new ClientCertificateView(controller, view);
      case AuthenticationChallengeType::SslHandshake:
        // SSL Handshake - Self-signed certificate
        return new SslHandshakeView(controller, view);
      default:
        // Load nothing, challenge not understood.
        return nullptr;
      }
    }
  }

/*!
  \class Esri::ArcGISRuntime::Toolkit::AuthenticationView
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppWidgetsViews
  \since Esri.ArcGISRutime 100.12
  \brief A view for handling authentication challenges and automatically 
  launching the appropriate UI for each type of authentication.
    
  Declare an AuthenticationView. The AuthenticationView 
  will then be connected to all authentication challenges, and will
  automatically launch the appropriate view for the type of
  challenge. Supported security formats include:

  \list
    \li ArcGIS Token (user credentials)
    \li HTTP Basic (user credentials)
    \li HTTP Digest (user credentials)
    \li IWA (user credentials)
    \li OAuth 2.0 (OAuth2)
    \li SAML (OAuth2)
    \li PKI (client certificate)
    \li SSL Handshake Warnings (ssl)
  \endlist
  
  \note OAuth 2.0 uses QWebEngineView. To use OAuth you must add \c{QT += webenginewidgets}
        to \c{qmake}.
        See \l{https://doc.qt.io/qt-5/qwebengineview.html}{QWebEngineView}.
 */

  /*!
    \brief Constructor.
    \list
      \li \a parent Parent widget.
    \endlist

    Internally the view will create and maintain its own \l AuthenticationController.
   */
  AuthenticationView::AuthenticationView(QWidget* parent) :
    AuthenticationView(new AuthenticationController{}, parent)
  {
    m_controller->setParent(this);
  }

  /*!
  \brief Constructor.
  \list
    \li \a controller AuthenticationController which will drive the view.
    \li \a parent Parent widget.
  \endlist
 */
  AuthenticationView::AuthenticationView(AuthenticationController* controller, QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::AuthenticationView)
  {
    m_ui->setupUi(this);
    setModal(true);
    connect(this, &QDialog::rejected, this, [this]
            {
              m_controller ? m_controller->cancel() : void();
            });
    setController(controller);
  }

  /*!
    \brief Destructor.
   */
  AuthenticationView::~AuthenticationView()
  {
    delete m_ui;
  }

  /*!
    \brief Returns the controller.
   */
  AuthenticationController* AuthenticationView::controller() const
  {
    return m_controller;
  }

  /*!
    \brief Sets the controller.
   */
  void AuthenticationView::setController(AuthenticationController* controller)
  {
    if (controller != m_controller)
    {
      // Disconnect from everything.
      if (m_controller)
      {
        disconnect(m_controller, nullptr, this, nullptr);
        m_controller->cancel();
      }
      m_controller = controller;

      // If controller is destroyed we stop showing a challenge.
      connect(
          m_controller, &AuthenticationController::destroyed, this, [this]
          {
            this->hide();
          });

      connect(m_controller, &AuthenticationController::currentChallengeChanged, this,
              [this]
              {
                auto view = createWidgetView(m_controller, this);
                if (!view)
                {
                  this->hide();
                  return;
                }

                // Delete view if the controller changes during its lifetime.
                connect(m_controller, &AuthenticationController::currentChallengeChanged, view, [view]()
                        {
                          view->deleteLater();
                        });

                // Delete view if the challenge changes during its lifetime.
                connect(m_controller, &AuthenticationController::destroyed, view, [view]()
                        {
                          view->deleteLater();
                        });

                m_ui->viewContent->addWidget(view);

                setWindowTitle(view->windowTitle());
                connect(view, &QWidget::windowTitleChanged, this, [this](const QString& title)
                        {
                          setWindowTitle(title);
                        });

                this->show();
              });
      emit authenticationControllerChanged();
    }
  }

  /*!
    \fn void Esri::ArcGISRuntime::Toolkit::AuthenticationView::authenticationControllerChanged()
    \brief Emitted when the controller used to drive the view changes.
   */

} // Toolkit
} // ArcGISRuntime
} // Esri
