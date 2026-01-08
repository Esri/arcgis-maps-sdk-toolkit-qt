
/*******************************************************************************
 *  Copyright 2012-2024 Esri
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
#include "AuthenticatorController.h"

// Qt headers
#include <QDesktopServices>
#include <QFuture>
#include <QOAuthUriSchemeReplyHandler>
#include <QPointer>
#include <QSslError>
#include <QSslSocket>
#include <QStringLiteral>
#include <QUrl>
#include <QtGlobal>
#include <QGuiApplication>

// Maps SDK headers
#include <ArcGISRuntimeEnvironment.h>
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <Authentication/AuthenticationTypes.h>
#include <Authentication/CertificateCredential.h>
#include <Authentication/NetworkAuthenticationChallenge.h>
#include <Authentication/OAuthUserConfiguration.h>
#include <Authentication/OAuthUserCredential.h>
#include <Authentication/OAuthUserLoginPrompt.h>
#include <Authentication/OAuthUserLogoutPrompt.h>
#include <Authentication/PasswordCredential.h>
#include <Authentication/ServerTrustCredential.h>
#include <Authentication/TokenCredential.h>
#include <Error.h>
#include <ErrorException.h>

// Toolkit headers
#include "ArcGISAuthenticationChallengeRelay.h"
#include "CustomOAuth2AuthorizationCodeFlow.h"
#include "NetworkAuthenticationChallengeRelay.h"

#ifdef Q_OS_IOS
#include "IOS/IOSWebAuthenticationSession.h"
#endif

// STL headers
#include <optional>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;
using Esri::ArcGISRuntime::Authentication::AuthenticationManager;

namespace Esri::ArcGISRuntime::Toolkit
{

  namespace
  {
    bool isInProcessOAuthRedirect(const QString& redirectUri)
    {
      return redirectUri == QStringLiteral("urn:ietf:wg:oauth:2.0:oob") || // this is the default value for "oob"
             redirectUri.contains(QStringLiteral("oob")); // this is what the Qt docs indicate to check for
    }
  } // namespace

  /*!
    \internal
    This class is an internal implementation detail and is subject to change.
   */

  AuthenticatorController::AuthenticatorController(QObject* parent) :
    QObject(parent)
  {
    m_arcGISAuthenticationChallengeRelay = std::make_unique<ArcGISAuthenticationChallengeRelay>(this);
    m_networkAuthenticationChallengeRelay = std::make_unique<NetworkAuthenticationChallengeRelay>(this);

    // listen for OAuth login prompts
    connect(ArcGISRuntimeEnvironment::authenticationManager(), &AuthenticationManager::oAuthUserLoginPromptIssued, this,
            [this](OAuthUserLoginPrompt* oAuthUserLoginPrompt)
    {
      oAuthUserLoginPrompt->setParent(nullptr);
      m_currentOAuthUserLoginPrompt = std::unique_ptr<OAuthUserLoginPrompt>{oAuthUserLoginPrompt};

      if (isInProcessOAuthRedirect(m_currentOAuthUserLoginPrompt->redirectUri()))
      { // use embedded web view session for "oob" redirect URI
        emit authorizeUrlChanged();
        emit redirectUriChanged();
        emit displayOAuthSignInView();
      }
      else
      {
        processOAuthExternalBrowserLogin_();
      }
    });

    // process logout requests for IAP workflows
    connect(ArcGISRuntimeEnvironment::authenticationManager(), &AuthenticationManager::oAuthUserLogoutPromptIssued, this,
            [this](OAuthUserLogoutPrompt* oAuthUserLogoutPrompt)
    {
      oAuthUserLogoutPrompt->setParent(nullptr);
      m_currentOAuthUserLogoutPrompt = std::unique_ptr<OAuthUserLogoutPrompt>{oAuthUserLogoutPrompt};

      if (isInProcessOAuthRedirect(m_currentOAuthUserLogoutPrompt->redirectUri()))
      {
        // No support currently for in-process WebView IAP logouts in the toolkit
        m_currentOAuthUserLogoutPrompt->respondWithError("In-process WebView logouts are not supported.");
        m_currentOAuthUserLogoutPrompt.reset();
      }
      else
      {
        processOAuthExternalBrowserLogout_();
      }
    });
  }

  AuthenticatorController::~AuthenticatorController() = default;

  AuthenticatorController* AuthenticatorController::create(QQmlEngine* qmlEngine, QJSEngine* /*jsEngine*/)
  {
    static QPointer<AuthenticatorController> instance = new AuthenticatorController(qmlEngine);

    if (!instance)
    {
      instance = new AuthenticatorController(qmlEngine);
    }

    return instance;
  }

  AuthenticatorController* AuthenticatorController::instance()
  {
    return create(nullptr, nullptr);
  }

  void AuthenticatorController::handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    challenge->setParent(nullptr);
    m_currentArcGISChallenge = std::unique_ptr<ArcGISAuthenticationChallenge>{challenge};
    emit currentAuthenticatingHostChanged();

    // first see if we can handle this with OAuth
    const auto requestUrl = challenge->requestUrl();
    for (OAuthUserConfiguration* userConfiguration : std::as_const(m_userConfigurations))
    {
      if (userConfiguration->canBeUsedForUrl(requestUrl))
      {
        m_currentOAuthUserConfiguration = userConfiguration;
        OAuthUserCredential::createAsync(userConfiguration, this)
          .then(this,
                [this](OAuthUserCredential* credential)
        {
          if (!m_currentArcGISChallenge)
          {
            return;
          }

          m_currentArcGISChallenge->continueWithCredential(credential);
          m_currentArcGISChallenge.reset();
        })
          .onFailed(this, [this](const ErrorException& e)
        {
          if (!m_currentArcGISChallenge)
          {
            return;
          }

          emit previousFailureCountChanged();
          auto* arcgisChallenge = m_currentArcGISChallenge.release();
          arcgisChallenge->setParent(this);
          arcgisChallenge->deleteLater();
          arcgisChallenge->continueWithError(e.error());
        });

        return;
      }
    }

    emit displayUsernamePasswordSignInView();
  }

  void AuthenticatorController::handleNetworkAuthenticationChallenge(NetworkAuthenticationChallenge* challenge)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    challenge->setParent(nullptr);
    m_currentNetworkChallenge = std::unique_ptr<NetworkAuthenticationChallenge>{challenge};

    if (!m_currentNetworkChallenge)
    {
      return;
    }

    switch (m_currentNetworkChallenge->networkChallengeType())
    {
      case NetworkChallengeType::ServerTrust:
      {
        emit displayAuthenticatorServerTrustView();
        return;
      }
      case NetworkChallengeType::Basic:
        [[fallthrough]];
      case NetworkChallengeType::Digest:
        [[fallthrough]];
      case NetworkChallengeType::Ntlm:
        [[fallthrough]];
      case NetworkChallengeType::Negotiate:
      {
        emit displayUsernamePasswordSignInView();
        return;
      }
      case NetworkChallengeType::ClientCertificate:
      {
        const auto sslBackend = QSslSocket::activeBackend();
        if (QSslSocket::activeBackend() != QStringLiteral("openssl"))
        {
          const auto error = QStringLiteral("ClientCertificate authentication is not supported with the current SSL backend (%1). ").arg(sslBackend) +
                             QStringLiteral("See https://doc.qt.io/qt-6/qsslsocket.html#activeBackend for more details. ") +
                             QStringLiteral("Only the openssl backend supports Client Certificates (PKI).");

          qWarning() << error;
          m_currentNetworkChallenge->continueWithError(Error{error, ""});
          m_currentNetworkChallenge.reset();
          return;
        }

        emit displayClientCertificateView();
        return;
      }
    }

    m_currentNetworkChallenge.reset();
    qWarning() << "unimplemented network authentication challenge";
  }

  void AuthenticatorController::continueWithServerTrust(bool trust)
  {
    if (trust)
    {
      auto* credential = ServerTrustCredential::createWithChallenge(m_currentNetworkChallenge.get(), this);
      if (credential)
      {
        m_currentNetworkChallenge->continueWithCredential(credential);
      }
      else
      {
        m_currentNetworkChallenge->continueAndFail();
      }
    }
    else
    {
      m_currentNetworkChallenge->continueWithError(Error{"A ServerTrust challenge was issued, but was blocked by the user", ""});
    }

    m_currentNetworkChallenge.reset();
  }

  AuthenticatorController::CertificateResult AuthenticatorController::respondWithClientCertificate(const QUrl& path, const QString& password)
  {
    if (!m_currentNetworkChallenge)
    {
      return CertificateResult::Error;
    }

    if (auto* clientCredential = NetworkCredential::certificate(path, password, this); clientCredential)
    {
      m_currentNetworkChallenge->continueWithCredential(clientCredential);
      m_currentNetworkChallenge.reset();
      return CertificateResult::Accepted;
    }

    return CertificateResult::PasswordRejected;
  }

  void AuthenticatorController::continueWithUsernamePassword(const QString& username, const QString& password)
  {
    if (m_currentArcGISChallenge)
    {
      continueWithUsernamePasswordArcGIS_(username, password);
    }
    else if (m_currentNetworkChallenge)
    {
      continueWithUsernamePasswordNetwork_(username, password);
    }
  }

  void AuthenticatorController::continueWithUsernamePasswordArcGIS_(const QString& username, const QString& password)
  {
    if (!m_currentArcGISChallenge)
    {
      return;
    }

    TokenCredential::createAsync(m_currentArcGISChallenge->requestUrl(), username, password, std::nullopt, this)
      .then(this,
            [this](TokenCredential* credential)
    {
      m_currentArcGISChallenge->continueWithCredential(credential);
      m_currentArcGISChallenge.reset();
    })
      .onFailed(this, [this](const ErrorException& e)
    {
      if (!m_currentArcGISChallenge)
      {
        return;
      }
      emit previousFailureCountChanged();
      auto* arcgisChallenge = m_currentArcGISChallenge.release();
      arcgisChallenge->setParent(this);
      arcgisChallenge->deleteLater();
      arcgisChallenge->continueWithError(e.error());
    });
  }

  void AuthenticatorController::continueWithUsernamePasswordNetwork_(const QString& username, const QString& password)
  {
    if (!m_currentNetworkChallenge)
    {
      return;
    }

    auto* passwordCredential = NetworkCredential::password(username, password, this);
    m_currentNetworkChallenge->continueWithCredential(passwordCredential);
    m_currentNetworkChallenge.reset();
  }

  void AuthenticatorController::respond(const QUrl& url)
  {
    if (!m_currentOAuthUserLoginPrompt)
    {
      return;
    }

    m_currentOAuthUserLoginPrompt->respond(url);
    m_currentOAuthUserLoginPrompt.reset();
  }

  void AuthenticatorController::respondWithError(const QString& platformError)
  {
    if (!m_currentOAuthUserLoginPrompt)
    {
      return;
    }

    m_currentOAuthUserLoginPrompt->respondWithError(platformError);
    m_currentOAuthUserLoginPrompt.reset();
  }

  void AuthenticatorController::cancel()
  {
#ifdef Q_OS_IOS
    if (m_iosWebAuthenticationSession)
    {
      m_iosWebAuthenticationSession->cancel();
      m_iosWebAuthenticationSession.reset();
    }
#endif

    if (m_currentNetworkChallenge)
    {
      m_currentNetworkChallenge->cancel();
      m_currentNetworkChallenge.reset();
    }

    if (m_currentArcGISChallenge)
    {
      m_currentArcGISChallenge->cancel();
      m_currentArcGISChallenge.reset();
    }

    if (m_currentOAuthUserLoginPrompt)
    {
      m_currentOAuthUserLoginPrompt->respondWithError("User canceled");
      m_currentOAuthUserLoginPrompt.reset();
    }
  }

  void AuthenticatorController::addOAuthUserConfiguration(OAuthUserConfiguration* userConfiguration)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    userConfiguration->setParent(this);
    m_userConfigurations.append(userConfiguration);
  }

  void AuthenticatorController::clearOAuthUserConfigurations()
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    qDeleteAll(m_userConfigurations);
    m_userConfigurations.clear();
  }

  QList<OAuthUserConfiguration*> AuthenticatorController::oAuthUserConfigurations() const
  {
    return m_userConfigurations;
  }

  QString AuthenticatorController::currentAuthenticatingHost_() const
  {
    if (m_currentNetworkChallenge)
    {
      return m_currentNetworkChallenge->host();
    }

    if (m_currentArcGISChallenge)
    {
      return m_currentArcGISChallenge->requestUrl().host();
    }

    return {};
  }

  QUrl AuthenticatorController::authorizeUrl_() const
  {
    return m_currentOAuthUserLoginPrompt ? m_currentOAuthUserLoginPrompt->authorizeUrl() : QUrl{};
  }

  QString AuthenticatorController::redirectUri_() const
  {
    if (m_currentOAuthUserLoginPrompt)
    {
      return m_currentOAuthUserLoginPrompt->redirectUri();
    }

    if (m_currentOAuthUserLogoutPrompt)
    {
      return m_currentOAuthUserLogoutPrompt->redirectUri();
    }

    return {};
  }

  int AuthenticatorController::previousFailureCount_() const
  {
    if (m_currentNetworkChallenge)
    {
      return m_currentNetworkChallenge->previousFailureCount();
    }

    if (m_currentArcGISChallenge)
    {
      return m_currentArcGISChallenge->previousFailureCount();
    }

    return 0;
  }

  void AuthenticatorController::processOAuthExternalBrowserLogin_()
  {
#ifdef Q_OS_IOS
    if (!m_currentOAuthUserLoginPrompt)
    {
      return;
    }

    const QUrl redirectUri{m_currentOAuthUserLoginPrompt->redirectUri()};
    const QString callbackScheme = redirectUri.scheme();

    m_iosWebAuthenticationSession = std::make_unique<IOSWebAuthenticationSession>(this);

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::completed, this, [this](const QUrl& callbackUrl)
    {
      if (!m_currentOAuthUserLoginPrompt)
      {
        finishOAuthExternalBrowserFlow_();
        return;
      }

      m_currentOAuthUserLoginPrompt->respond(callbackUrl);
      finishOAuthExternalBrowserFlow_();
    });

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::canceled, this, [this]()
    {
      if (m_currentOAuthUserLoginPrompt)
      {
        m_currentOAuthUserLoginPrompt->respondWithError(QStringLiteral("User canceled"));
      }
      finishOAuthExternalBrowserFlow_();
    });

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::failed, this, [this](const QString& error)
    {
      if (m_currentOAuthUserLoginPrompt)
      {
        m_currentOAuthUserLoginPrompt->respondWithError(error);
      }
      finishOAuthExternalBrowserFlow_();
    });

    m_iosWebAuthenticationSession->start(m_currentOAuthUserLoginPrompt->authorizeUrl(), callbackScheme);
    return;
#endif

    auto* oauthFlow = new CustomOAuth2AuthorizationCodeFlow(m_currentOAuthUserLoginPrompt->authorizeUrl(), m_currentOAuthUserLoginPrompt.get());

    auto* callbackReplyHandler = new QOAuthUriSchemeReplyHandler(m_currentOAuthUserLoginPrompt.get());

    connect(callbackReplyHandler, &QOAuthUriSchemeReplyHandler::callbackReceived, this, [this, oauthFlow](const QVariantMap& values)
    {
      if (!values.contains("code"))
      {
        m_currentOAuthUserLoginPrompt->respondWithError("There was an error obtaining the authorization code");
        finishOAuthExternalBrowserFlow_();
        return;
      }

      const auto code = values.value("code").toString();
      oauthFlow->setAuthorizationCode(code);
      emit oauthFlow->granted();
    });

    oauthFlow->setAuthorizationUrl(m_currentOAuthUserLoginPrompt->authorizeUrl());

    if (m_currentOAuthUserConfiguration)
    { // identity-aware proxy workflows will not have any OAuthUserConfiguration
      oauthFlow->setClientIdentifier(m_currentOAuthUserConfiguration->clientId());
    }

    connect(oauthFlow, &QAbstractOAuth::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    connect(oauthFlow, &QAbstractOAuth::granted, this, [callbackReplyHandler, oauthFlow, this]()
    {
      callbackReplyHandler->close();

      // this needs to be in the form of redirectUri?code=authCode
      const auto formattedResponseUrl = QUrl{QString("%1?code=%2").arg(m_currentOAuthUserLoginPrompt->redirectUri(), oauthFlow->authorizationCode())};
      m_currentOAuthUserLoginPrompt->respond(formattedResponseUrl);
      finishOAuthExternalBrowserFlow_();
    });

    callbackReplyHandler->setRedirectUrl(m_currentOAuthUserLoginPrompt->redirectUri());
    oauthFlow->setReplyHandler(callbackReplyHandler);

    if (callbackReplyHandler->listen())
    {
      oauthFlow->grant();
    }
    else
    {
      m_currentOAuthUserLoginPrompt->respondWithError("There was an error establishing the redirect URL listener");
      finishOAuthExternalBrowserFlow_();
    }
  }

  void AuthenticatorController::processOAuthExternalBrowserLogout_()
  {
#ifdef Q_OS_IOS
    // IAP logout provides no way to get back to the app, so m_iosWebAuthenticationSession is a custom implementation
    // that pops Safari (or whatever system default browser) up within the same app window, and it contains a cancel
    // button that allows users to more gracefully return to the app, and this code to more gracefully know when it
    // happens.
    if (!m_currentOAuthUserLogoutPrompt)
    {
      return;
    }

    const QUrl redirectUri{m_currentOAuthUserLogoutPrompt->redirectUri()};
    const QString callbackScheme = redirectUri.scheme();

    m_iosWebAuthenticationSession = std::make_unique<IOSWebAuthenticationSession>(this);

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::completed, this, [this](const QUrl& /*callbackUrl*/)
    {
      if (m_currentOAuthUserLogoutPrompt)
      {
        constexpr auto loggedOut = true;
        m_currentOAuthUserLogoutPrompt->respond(loggedOut);
      }
      finishOAuthExternalBrowserFlow_();
    });

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::canceled, this, [this]()
    {
      if (m_currentOAuthUserLogoutPrompt)
      {
        // currently with IAP, there is no callback mechanism to return to the app once the user logs out,
        // so we advise to simply cancel the Safari session to return to the app, which ends up here and
        // we assume it was successful.
        constexpr auto loggedOut = true;
        m_currentOAuthUserLogoutPrompt->respond(loggedOut);
      }
      finishOAuthExternalBrowserFlow_();
    });

    connect(m_iosWebAuthenticationSession.get(), &IOSWebAuthenticationSession::failed, this, [this](const QString& error)
    {
      if (m_currentOAuthUserLogoutPrompt)
      {
        m_currentOAuthUserLogoutPrompt->respondWithError(error);
      }
      finishOAuthExternalBrowserFlow_();
    });

    m_iosWebAuthenticationSession->start(m_currentOAuthUserLogoutPrompt->logoutUrl(), callbackScheme);
#else
    // for IAP logout workflows, there is no way to get back to the app. The callback URI does not fire back to the app after logging out.
    // As such, we just open the browser, allow the user to do this, and then assume whenever control returns to the app, it works successfully.

    m_appState = QGuiApplication::applicationState();
    m_logoutStateChangeConnection = connect(qGuiApp, &QGuiApplication::applicationStateChanged, this, [this](Qt::ApplicationState state)
    {
      const auto previousState = m_appState;
      m_appState = state;
      if (previousState != Qt::ApplicationActive && m_appState == Qt::ApplicationActive)
      {
        constexpr auto loggedOut = true;
        m_currentOAuthUserLogoutPrompt->respond(loggedOut);
        finishOAuthExternalBrowserFlow_();
        disconnect(m_logoutStateChangeConnection);
      }
    });

    QDesktopServices::openUrl(m_currentOAuthUserLogoutPrompt->logoutUrl());
#endif // Q_OS_IOS
  }

  void AuthenticatorController::finishOAuthExternalBrowserFlow_()
  {
    m_currentOAuthUserConfiguration = nullptr;
    m_currentOAuthUserLoginPrompt.reset();

    m_currentOAuthUserLogoutPrompt.reset();

#ifdef Q_OS_IOS
    if (m_iosWebAuthenticationSession)
    {
      m_iosWebAuthenticationSession->cancel();
      m_iosWebAuthenticationSession.reset();
    }
#endif
  }

} // namespace Esri::ArcGISRuntime::Toolkit
