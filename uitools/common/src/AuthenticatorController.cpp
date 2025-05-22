
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
#include <QFuture>
#include <QPointer>
#include <QUrl>
#include <QSslError>
#include <QSslSocket>
#include <QStringLiteral>

// Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <Authentication/AuthenticationTypes.h>
#include <Authentication/CertificateCredential.h>
#include <Authentication/OAuthUserConfiguration.h>
#include <Authentication/OAuthUserCredential.h>
#include <Authentication/OAuthUserLoginPrompt.h>
#include <Authentication/TokenCredential.h>
#include <Authentication/ServerTrustCredential.h>
#include <Authentication/NetworkAuthenticationChallenge.h>
#include <Authentication/PasswordCredential.h>
#include <ArcGISRuntimeEnvironment.h>
#include <Error.h>
#include <ErrorException.h>

// Toolkit headers
#include "AuthenticatorChallengeRelay.h"
#include "NetworkAuthenticationChallengeRelay.h"

// STL headers
#include <optional>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;
using Esri::ArcGISRuntime::Authentication::AuthenticationManager;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

AuthenticatorController::AuthenticatorController(QObject* parent) :
    QObject(parent)
{
  if (!canBeUsed_())
  {
    return;
  }

  m_AuthenticatorChallengeRelay = std::make_unique<AuthenticatorChallengeRelay>(this);
  m_networkAuthenticationChallengeRelay = std::make_unique<NetworkAuthenticationChallengeRelay>(this);

  // listen for OAuth prompts
  connect(ArcGISRuntimeEnvironment::authenticationManager(), &AuthenticationManager::oAuthUserLoginPromptIssued, this,
          [this](OAuthUserLoginPrompt* currentOAuthUserLoginPrompt)
          {
            currentOAuthUserLoginPrompt->setParent(nullptr);
            m_currentOAuthUserLoginPrompt = std::unique_ptr<OAuthUserLoginPrompt>{currentOAuthUserLoginPrompt};
            emit authorizeUrlChanged();
            emit redirectUriChanged();
            emit displayOAuthSignInView();
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
      OAuthUserCredential::createAsync(userConfiguration, this).then(this, [this](OAuthUserCredential* credential)
                                                                     {
                                                                       if (!m_currentArcGISChallenge)
                                                                       {
                                                                         return;
                                                                       }

                                                                       m_currentArcGISChallenge->continueWithCredential(credential);
                                                                       m_currentArcGISChallenge.reset();
                                                                     }).onFailed(this, [this](const ErrorException& e)
                    {
                      if (!m_currentArcGISChallenge)
                      {
                        return;
                      }

                      m_currentArcGISChallenge->continueAndFailWithError(e.error());
                      m_currentArcGISChallenge.reset();
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

  switch(m_currentNetworkChallenge->networkChallengeType())
  {
    case NetworkChallengeType::ServerTrust:
    {
      emit displayServerTrustView();
      return;
    }
    case NetworkChallengeType::Basic: [[fallthrough]];
    case NetworkChallengeType::Digest: [[fallthrough]];
    case NetworkChallengeType::Ntlm: [[fallthrough]];
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
        const auto error = QStringLiteral("ClientCertificate authentication is not supported with the current SSL backend (%1). ")
        .arg(sslBackend) +
            QStringLiteral("See https://doc.qt.io/qt-6/qsslsocket.html#activeBackend for more details. ") +
            QStringLiteral("Only the openssl backend supports Client Certificates (PKI).");

        qWarning() << error;
        m_currentNetworkChallenge->continueAndFailWithError(Error{error, ""});
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
    m_currentNetworkChallenge->continueAndFailWithError(
        Error{"A ServerTrust challenge was issued, but was blocked by the user", ""});
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

  TokenCredential::createAsync(m_currentArcGISChallenge->requestUrl(),
                               username,
                               password,
                               std::nullopt).then(this, [this](TokenCredential* credential)
            {
              if (m_arcGISPreviousFailureCountsForUrl.contains(m_currentArcGISChallenge->requestUrl()))
              {
                m_arcGISPreviousFailureCountsForUrl.remove(m_currentArcGISChallenge->requestUrl());
              }

              m_currentArcGISChallenge->continueWithCredential(credential);
              m_currentArcGISChallenge.reset();
            }).onFailed(this, [this](const ErrorException& e)
                {
                  const auto requestUrl = m_currentArcGISChallenge->requestUrl();
                  if (m_arcGISPreviousFailureCountsForUrl.contains(requestUrl))
                  {
                    m_arcGISPreviousFailureCountsForUrl[requestUrl] = m_arcGISPreviousFailureCountsForUrl[requestUrl] + 1;
                  }
                  else
                  {
                    m_arcGISPreviousFailureCountsForUrl[requestUrl] = 1;
                  }

                  if (m_arcGISPreviousFailureCountsForUrl[requestUrl] >= s_maxArcGISPreviousFailureCount)
                  {
                    m_arcGISPreviousFailureCountsForUrl.remove(requestUrl);
                    m_currentArcGISChallenge->continueAndFailWithError(e.error());
                    m_currentArcGISChallenge.reset();
                    return;
                  }

                  emit previousFailureCountChanged();
                  emit displayUsernamePasswordSignInView();
                });
}

void AuthenticatorController::AuthenticatorController::continueWithUsernamePasswordNetwork_(const QString& username, const QString& password)
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

bool AuthenticatorController::canBeUsed_() const
{
  // this class cannot be used with the legacy authentication system
  return ArcGISRuntimeEnvironment::useLegacyAuthentication() == false;
}

void AuthenticatorController::addOAuthUserConfiguration(OAuthUserConfiguration* userConfiguration)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  userConfiguration->setParent(this);
  m_userConfigurations.append(userConfiguration);
}

void AuthenticatorController::setOAuthUserConfigurations(QList<OAuthUserConfiguration*> userConfigurations)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_userConfigurations = std::move(userConfigurations);
  std::for_each(std::begin(m_userConfigurations), std::end(m_userConfigurations), [this](auto* userConfiguration)
                {
                  userConfiguration->setParent(this);
                });
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
  return m_currentOAuthUserLoginPrompt ? m_currentOAuthUserLoginPrompt->redirectUri() : QString{};
}

int AuthenticatorController::previousFailureCount_() const
{
  if (m_currentNetworkChallenge)
  {
    return m_currentNetworkChallenge->previousFailureCount();
  }

  if (m_currentArcGISChallenge)
  {
    const auto requestUrl = m_currentArcGISChallenge->requestUrl();
    if (m_arcGISPreviousFailureCountsForUrl.contains(requestUrl))
    {
      return m_arcGISPreviousFailureCountsForUrl[requestUrl];
    }
  }

  return 0;
}

} //  Esri::ArcGISRuntime::Toolkit
