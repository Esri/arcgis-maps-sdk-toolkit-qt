
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
#include "ArcGISAuthenticationController.h"

// Qt headers
#include <QFuture>
#include <QPointer>
#include <QUrl>
#include <QSslError>

// Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <Authentication/AuthenticationTypes.h>
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
#include "ArcGISAuthenticationChallengeRelay.h"
#include "NetworkAuthenticationChallengeRelay.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;
using Esri::ArcGISRuntime::Authentication::AuthenticationManager;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
 \class Toolkit::ArcGISAuthenticationController
 \inmodule Esri.ArcGISRuntime.Toolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief In MVC architecture, this is the controller for the corresponding
        AuthenticationView.

 \internal
 */

/*!
  \internal
 */
ArcGISAuthenticationController::ArcGISAuthenticationController(QObject* parent) :
  QObject(parent)
{
  if (!canBeUsed_())
  {
    return;
  }

  m_arcGISAuthenticationChallengeRelay = std::make_unique<ArcGISAuthenticationChallengeRelay>(this);
  m_networkAuthenticationChallengeRelay = std::make_unique<NetworkAuthenticationChallengeRelay>(this);

  // listen for OAuth prompts
  connect(ArcGISRuntimeEnvironment::authenticationManager(), &AuthenticationManager::oAuthUserLoginPromptIssued, this,
          [this](OAuthUserLoginPrompt* currentOAuthUserLoginPrompt)
  {
    currentOAuthUserLoginPrompt->setParent(nullptr);
    m_currentOAuthUserLoginPrompt = std::unique_ptr<OAuthUserLoginPrompt>{currentOAuthUserLoginPrompt};
    emit authorizeUrlChanged();
    emit preferPrivateWebBrowserSessionChanged();
    emit redirectUriChanged();
    emit displayOAuthSignInView();
  });
}

/*!
  \internal
 */
ArcGISAuthenticationController::~ArcGISAuthenticationController() = default;

/*!
  \internal
 */
ArcGISAuthenticationController* ArcGISAuthenticationController::create(QQmlEngine* qmlEngine, QJSEngine* /*jsEngine*/)
{
  static QPointer<ArcGISAuthenticationController> instance = new ArcGISAuthenticationController(qmlEngine);

  if (!instance)
  {
    instance = new ArcGISAuthenticationController(qmlEngine);
  }

  return instance;
}

/*!
  \internal
 */
ArcGISAuthenticationController* ArcGISAuthenticationController::instance()
{
  return create(nullptr, nullptr);
}

/*!
  \internal
 */
void ArcGISAuthenticationController::handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge)
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

/*!
  \internal
 */
void ArcGISAuthenticationController::handleNetworkAuthenticationChallenge(NetworkAuthenticationChallenge* challenge)
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
      Q_UNIMPLEMENTED();
    }
  }

  m_currentNetworkChallenge.reset();
  qWarning() << "unimplemented network authentication challenge";
}

/*!
  \internal
 */
void ArcGISAuthenticationController::continueWithServerTrust(bool trust)
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

/*!
  \internal
 */
void ArcGISAuthenticationController::continueWithUsernamePassword(const QString& username, const QString& password)
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

/*!
  \internal
 */
void ArcGISAuthenticationController::continueWithUsernamePasswordArcGIS_(const QString& username, const QString& password)
{
  if (!m_currentArcGISChallenge)
  {
    return;
  }

  TokenCredential::createAsync(m_currentArcGISChallenge->requestUrl(),
                               username,
                               password,
                               0).then(this, [this](TokenCredential* credential)
  {
    m_currentArcGISChallenge->continueWithCredential(credential);
    m_currentArcGISChallenge.reset();
  }).onFailed(this, [this](const ErrorException& e)
  {
    if ((++m_arcGISPreviousFailureCount) >= s_maxArcGISPreviousFailureCount)
    {
      m_arcGISPreviousFailureCount = 0;
      m_currentArcGISChallenge->continueAndFailWithError(e.error());
      m_currentArcGISChallenge.reset();
      return;
    }

    emit previousFailureCountChanged();
    emit displayUsernamePasswordSignInView();
  });
}

/*!
  \internal
 */
void ArcGISAuthenticationController::ArcGISAuthenticationController::continueWithUsernamePasswordNetwork_(const QString& username, const QString& password)
{
  if (!m_currentNetworkChallenge)
  {
    return;
  }

  auto* passwordCredential = NetworkCredential::password(username, password, this);
  m_currentNetworkChallenge->continueWithCredential(passwordCredential);
  m_currentNetworkChallenge.reset();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::respond(const QUrl& url)
{
  if (!m_currentOAuthUserLoginPrompt)
  {
    return;
  }

  m_currentOAuthUserLoginPrompt->respond(url);
  m_currentOAuthUserLoginPrompt.reset();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::respondWithError(const QString& platformError)
{
  if (!m_currentOAuthUserLoginPrompt)
  {
    return;
  }

  m_currentOAuthUserLoginPrompt->respondWithError(platformError);
  m_currentOAuthUserLoginPrompt.reset();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::cancel()
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

/*!
  \internal
 */
bool ArcGISAuthenticationController::canBeUsed_() const
{
  // this class cannot be used with the legacy authentication system
  return ArcGISRuntimeEnvironment::useLegacyAuthentication() == false;
}

/*!
  \internal
 */
void ArcGISAuthenticationController::addOAuthUserConfiguration(OAuthUserConfiguration* userConfiguration)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  userConfiguration->setParent(this);
  m_userConfigurations.append(userConfiguration);
}

/*!
  \internal
 */
void ArcGISAuthenticationController::setOAuthUserConfigurations(QList<OAuthUserConfiguration*> userConfigurations)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_userConfigurations = std::move(userConfigurations);
  std::for_each(std::begin(m_userConfigurations), std::end(m_userConfigurations), [this](auto* userConfiguration)
  {
    userConfiguration->setParent(this);
  });
}

/*!
  \internal
 */
void ArcGISAuthenticationController::clearOAuthUserConfigurations()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  qDeleteAll(m_userConfigurations);
  m_userConfigurations.clear();
}

/*!
  \internal
 */
QList<OAuthUserConfiguration*> ArcGISAuthenticationController::oAuthUserConfigurations() const
{
  return m_userConfigurations;
}

/*!
  \internal
 */
QString ArcGISAuthenticationController::currentAuthenticatingHost_() const
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

/*!
  \internal
 */
QUrl ArcGISAuthenticationController::authorizeUrl_() const
{
  return m_currentOAuthUserLoginPrompt ? m_currentOAuthUserLoginPrompt->authorizeUrl() : QUrl{};
}

/*!
  \internal
 */
bool ArcGISAuthenticationController::preferPrivateWebBrowserSession_() const
{
  return m_currentOAuthUserLoginPrompt ? m_currentOAuthUserLoginPrompt->preferPrivateWebBrowserSession() : false;
}

/*!
  \internal
 */
QString ArcGISAuthenticationController::redirectUri_() const
{
  return m_currentOAuthUserLoginPrompt ? m_currentOAuthUserLoginPrompt->redirectUri() : QString{};
}

/*!
  \internal
 */
int ArcGISAuthenticationController::previousFailureCount_() const
{
  return m_currentNetworkChallenge ? m_currentNetworkChallenge->previousFailureCount() : m_arcGISPreviousFailureCount;
}

} //  Esri::ArcGISRuntime::Toolkit
