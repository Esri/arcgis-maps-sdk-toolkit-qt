
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

// Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>
#include <Error.h>
#include <ErrorException.h>
#include <Authentication/OAuthUserConfiguration.h>
#include <Authentication/OAuthUserCredential.h>
#include <Authentication/OAuthUserLoginPrompt.h>
#include <Authentication/TokenCredential.h>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;
using Esri::ArcGISRuntime::Authentication::AuthenticationManager;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
 \class Toolkit::ArcGISAuthenticationController
 \inmodule EsriArcGISRuntimeToolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief In MVC architecture, this is the controller for the corresponding
        AuthenticationView.

 \internal
 */

/*!
  \internal
 */
ArcGISAuthenticationController::ArcGISAuthenticationController(QObject* parent) :
  ArcGISAuthenticationChallengeHandler(parent)
{
  if (!canBeUsed_())
    return;

  // Set the authentication manager to use this controller
  ArcGISRuntimeEnvironment::instance()->authenticationManager()->setArcGISAuthenticationChallengeHandler(this);

  // listen for OAuth prompts
  connect(ArcGISRuntimeEnvironment::instance()->authenticationManager(), &AuthenticationManager::oAuthUserLoginPromptIssued, this,
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
    instance = new ArcGISAuthenticationController(qmlEngine);

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
  m_currentChallenge = std::unique_ptr<ArcGISAuthenticationChallenge>{challenge};
  emit currentAuthenticatingHostChanged();

  // first see if we can handle this with OAuth
  const auto requestUrl = challenge->requestUrl();
  for (OAuthUserConfiguration* userConfiguration : std::as_const(m_userConfigurations))
  {
    if (userConfiguration->canBeUsedForUrl(requestUrl))
    {
      OAuthUserCredential::createAsync(userConfiguration, this).then(this, [this](OAuthUserCredential* credential)
      {
        if (!m_currentChallenge)
          return;

        m_currentChallenge->continueWithCredential(credential);
        m_currentChallenge.reset();
      }).onFailed(this, [this](const ErrorException& e)
      {
        if (!m_currentChallenge)
          return;

        m_currentChallenge->continueAndFailWithError(e.error());
        m_currentChallenge.reset();
      });

      return;
    }
  }

  emit displayUsernamePasswordSignInView();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::continueWithUsernamePassword(const QString& username, const QString& password)
{
  if (!m_currentChallenge)
    return;

  TokenCredential::createAsync(m_currentChallenge->requestUrl(),
                               username,
                               password,
                               0).then(this, [this](TokenCredential* credential)
  {
    m_currentChallenge->continueWithCredential(credential);
    m_currentChallenge.reset();
  }).onFailed(this, [this](ErrorException e)
  {
    if ((++m_currentChallengeFailureCount) >= s_maxChallengeFailureCount)
    {
      m_currentChallengeFailureCount = 0;
      m_currentChallenge->continueAndFailWithError(e.error());
      m_currentChallenge.reset();
      return;
    }

    emit currentChallengeFailureCountChanged();
    emit displayUsernamePasswordSignInView();
  });
}

/*!
  \internal
 */
void ArcGISAuthenticationController::respond(const QUrl& url)
{
  if (!m_currentOAuthUserLoginPrompt)
    return;

  m_currentOAuthUserLoginPrompt->respond(url);
  m_currentOAuthUserLoginPrompt.reset();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::respondWithError(const QString& platformError)
{
  if (!m_currentOAuthUserLoginPrompt)
    return;

  m_currentOAuthUserLoginPrompt->respondWithError(platformError);
  m_currentOAuthUserLoginPrompt.reset();
}

/*!
  \internal
 */
void ArcGISAuthenticationController::cancel()
{
  if (m_currentChallenge)
  {
    m_currentChallenge->cancel();
    m_currentChallenge.reset();
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
QUrl ArcGISAuthenticationController::currentAuthenticatingHost_() const
{
  if (!m_currentChallenge)
    return {};

  const auto requestUrl = m_currentChallenge->requestUrl();
  const auto scheme = requestUrl.scheme();
  const auto host = requestUrl.host();
  return scheme + "://" + host;
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
int ArcGISAuthenticationController::currentChallengeFailureCount_() const
{
  return m_currentChallengeFailureCount;
}

} //  Esri::ArcGISRuntime::Toolkit
