// COPYRIGHT 2024 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com
/// \file ArcGISAuthenticationController.cpp

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

// ArcGISRuntime headers
#include <ArcGISAuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>
#include <OAuthUserConfiguration.h>
#include <OAuthUserCredential.h>
#include <TokenCredential.h>
#include <ArcGISAuthenticationChallenge.h>
#include <ErrorException.h>
#include <Error.h>

#include <QUrl>
#include <QFuture>
#include <QPointer>

using namespace Esri::ArcGISRuntime;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
 \class Toolkit::ArcGISAuthenticationController
 \inmodule EsriArcGISRuntimeToolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief In MVC architecture, this is the controller for the corresponding
        AuthenticationView.
 */

/*!
  \brief Constructor
  \list
    \li \a parent Owning QObject.
  \endlist
 */
ArcGISAuthenticationController::ArcGISAuthenticationController(QObject* parent) :
  ArcGISAuthenticationChallengeHandler(parent)
{
  if (!canBeUsed_())
    return;

  // Set the authentication manager to use this controller
  ArcGISRuntimeEnvironment::instance()->arcGISAuthenticationManager()->setArcGISAuthenticationChallengeHandler(this);
}

/*!
  \brief Destructor.
 */
ArcGISAuthenticationController::~ArcGISAuthenticationController() = default;

ArcGISAuthenticationController* ArcGISAuthenticationController::create(QQmlEngine*, QJSEngine*)
{
  static QPointer<ArcGISAuthenticationController> instance = new ArcGISAuthenticationController();

  if (!instance)
    instance = new ArcGISAuthenticationController();

  return instance;
}

ArcGISAuthenticationController* ArcGISAuthenticationController::instance()
{
  return create(nullptr, nullptr);
}

void ArcGISAuthenticationController::handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_currentChallenge = challenge;
  emit currentAuthenticatingHostChanged();

  // first see if we can handle this with OAuth
  const auto requestUrl = challenge->requestUrl();
  for (OAuthUserConfiguration* userConfiguration : std::as_const(m_userConfigurations))
  {
    if (userConfiguration->canBeUsedForUrl(requestUrl))
    {
      OAuthUserCredential::createAsync(userConfiguration, this).then([this](OAuthUserCredential* credential)
      {
        m_currentChallenge->continueWithCredential(credential);
      }).onFailed([this](const ErrorException& e)
      {
        m_currentChallenge->continueAndFailWithError(e.error());
      });

      return;
    }
  }

  // TODO need to show a dialog to get the username and password for token
  emit displayUsernamePasswordSignInView();
}

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
    m_currentChallenge->deleteLater();
    m_currentChallenge = nullptr;
  }).onFailed(this, [this](ErrorException e)
  {
    m_currentChallenge->continueAndFailWithError(e.error());
    m_currentChallenge->deleteLater();
    m_currentChallenge = nullptr;
  });
}

// this class cannot be used with the legacy authentication system
bool ArcGISAuthenticationController::canBeUsed_() const
{
  return ArcGISRuntimeEnvironment::useLegacyAuthentication() == false;
}

void ArcGISAuthenticationController::addOAuthUserConfiguration(OAuthUserConfiguration* userConfiguration)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  userConfiguration->setParent(this);
  m_userConfigurations.append(userConfiguration);
}

void ArcGISAuthenticationController::setOAuthUserConfigurations(QList<OAuthUserConfiguration*> userConfigurations)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_userConfigurations = std::move(userConfigurations);
  std::for_each(std::begin(m_userConfigurations), std::end(m_userConfigurations), [this](auto* userConfiguration)
  {
    userConfiguration->setParent(this);
  });
}

void ArcGISAuthenticationController::clearOAuthUserConfigurations()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  qDeleteAll(m_userConfigurations);
  m_userConfigurations.clear();
}

QList<OAuthUserConfiguration*> ArcGISAuthenticationController::oAuthUserConfigurations() const
{
  return m_userConfigurations;
}

QUrl ArcGISAuthenticationController::currentAuthenticatingHost_() const
{
  if (!m_currentChallenge)
    return {};

  const auto requestUrl = m_currentChallenge->requestUrl();
  const auto scheme = requestUrl.scheme();
  const auto host = requestUrl.host();
  return scheme + "://" + host;
}

} //  Esri::ArcGISRuntime::Toolkit
