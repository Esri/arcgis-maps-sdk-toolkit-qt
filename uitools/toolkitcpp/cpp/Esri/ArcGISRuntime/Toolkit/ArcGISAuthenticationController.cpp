
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
#include <ArcGISAuthenticationChallenge.h>
#include <ArcGISAuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>
#include <Error.h>
#include <ErrorException.h>
#include <OAuthUserConfiguration.h>
#include <OAuthUserCredential.h>
#include <OAuthUserLoginPrompt.h>
#include <TokenCredential.h>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthUriSchemeReplyHandler>
#include <QDesktopServices>

#include "CustomOAuth2AuthorizationCodeFlow.h"

using namespace Esri::ArcGISRuntime;

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
  ArcGISRuntimeEnvironment::instance()->arcGISAuthenticationManager()->setArcGISAuthenticationChallengeHandler(this);

  // listen for OAuth prompts
  connect(ArcGISRuntimeEnvironment::instance()->arcGISAuthenticationManager(), &ArcGISAuthenticationManager::oAuthUserLoginPromptIssued, this,
          [this](OAuthUserLoginPrompt* currentOAuthUserLoginPrompt)
  {
    m_currentOAuthUserLoginPrompt = currentOAuthUserLoginPrompt;
    m_currentOAuthUserLoginPrompt->setParent(this);

    if (const auto redirectUrl = m_currentOAuthUserLoginPrompt->redirectUri();
          redirectUrl == QUrl{"urn:ietf:wg:oauth:2.0:oob"} || // this is the default value for "oob"
          redirectUrl.toString().contains("oob")) // this is what the Qt docs indicate to check for
    { // use embedded web view session for "oob" redirect URI
      emit authorizeUrlChanged();
      emit preferPrivateWebBrowserSessionChanged();
      emit redirectUriChanged();
      emit displayOAuthSignInView();
    }
    else
    {
      processOAuthExternalBrowserLogin_();
    }
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
  else if (qmlEngine && !instance->parent())
  { // it's possible c++ can call 'instance' first and we don't want this to have no parent
    instance->setParent(qmlEngine);
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
  m_currentChallenge = std::unique_ptr<ArcGISAuthenticationChallenge>{challenge};
  emit currentAuthenticatingHostChanged();

  // first see if we can handle this with OAuth
  const auto requestUrl = challenge->requestUrl();
  for (OAuthUserConfiguration* userConfiguration : std::as_const(m_userConfigurations))
  {
    if (userConfiguration->canBeUsedForUrl(requestUrl))
    {
      m_currentOAuthUserConfiguration = userConfiguration;
      OAuthUserCredential::createAsync(userConfiguration, this).then(this, [this](OAuthUserCredential* credential)
      {
        if (!m_currentChallenge)
          return;

        m_currentChallenge->continueWithCredential(credential);
        finishChallengeFlow_();
      }).onFailed(this, [this](const ErrorException& e)
      {
        if (!m_currentChallenge)
          return;

        m_currentChallenge->continueAndFailWithError(e.error());
        finishChallengeFlow_();
      });

      return;
    }
  }

  emit displayUsernamePasswordSignInView();
}

void ArcGISAuthenticationController::processOAuthExternalBrowserLogin_()
{
  CustomOAuth2AuthorizationCodeFlow* oauthFlow = new CustomOAuth2AuthorizationCodeFlow(m_currentOAuthUserLoginPrompt->authorizeUrl(),
                                                                                       m_currentOAuthUserLoginPrompt);

  QOAuthUriSchemeReplyHandler* callbackReplyHandler = new QOAuthUriSchemeReplyHandler(m_currentOAuthUserLoginPrompt);
  connect(callbackReplyHandler, &QOAuthUriSchemeReplyHandler::callbackReceived, this, [this, oauthFlow](const QVariantMap& values)
  {
    if (!values.contains("code"))
    {
      m_currentOAuthUserLoginPrompt->respondWithError("There was an error obtaining the authorization code");
      finishChallengeFlow_();
      return;
    }

    const auto code = values.value("code").toString();
    oauthFlow->setAuthorizationCode(code);
    emit oauthFlow->granted();
  });

  oauthFlow->setAuthorizationUrl(m_currentOAuthUserLoginPrompt->authorizeUrl());
  oauthFlow->setClientIdentifier(m_currentOAuthUserConfiguration->clientId());

  connect(oauthFlow, &QAbstractOAuth::authorizeWithBrowser, this, &QDesktopServices::openUrl);
  connect(oauthFlow, &QAbstractOAuth::granted, this, [callbackReplyHandler, oauthFlow, this]()
  {
    callbackReplyHandler->close();
    m_currentOAuthUserLoginPrompt->respondWithAuthorizationCode(oauthFlow->authorizationCode());
  });

  callbackReplyHandler->setRedirectUrl(m_currentOAuthUserLoginPrompt->redirectUri());
  oauthFlow->setReplyHandler(callbackReplyHandler);

  if (callbackReplyHandler->listen())
  {
    oauthFlow->grant();
  }
}

void ArcGISAuthenticationController::finishChallengeFlow_()
{
  m_currentChallenge = {};
  m_currentOAuthUserConfiguration = nullptr;

  if (m_currentOAuthUserLoginPrompt)
  {
    m_currentOAuthUserLoginPrompt->deleteLater();
    m_currentOAuthUserLoginPrompt = nullptr;
  }

  m_currentChallengeFailureCount = 0;
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
    finishChallengeFlow_();
  }).onFailed(this, [this](ErrorException e)
  {
    if ((++m_currentChallengeFailureCount) >= s_maxChallengeFailureCount)
    {
      m_currentChallengeFailureCount = 0;
      m_currentChallenge->continueAndFailWithError(e.error());
      finishChallengeFlow_();
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
}

void ArcGISAuthenticationController::respondWithAuthorizationCode(const QString& authorizationCode)
{
  if (!m_currentOAuthUserLoginPrompt)
    return;

  m_currentOAuthUserLoginPrompt->respondWithAuthorizationCode(authorizationCode);
}

/*!
  \internal
 */
void ArcGISAuthenticationController::respondWithError(const QString& platformError)
{
  if (!m_currentOAuthUserLoginPrompt)
    return;

  m_currentOAuthUserLoginPrompt->respondWithError(platformError);
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
    finishChallengeFlow_();
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
QUrl ArcGISAuthenticationController::redirectUri_() const
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
