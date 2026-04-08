/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

#include "ArcGISRuntimeEnvironmentProxy.h"

#include "OAuthUserConfigurationManager.h"

#include <ArcGISRuntimeEnvironment.h>
#include <Authentication/ArcGISCredentialStore.h>
#include <Authentication/AuthenticationManager.h>
#include <AuthenticatorController.h>
#include <QFuture>

/*!
  \internal
  \class ArcGISRuntimeEnvironmentProxy
  \brief This class exposes the global `ArcGISRuntimeEnvironmentProxy.apiKey` property such
  that it is accessible to QML.
 */

ArcGISRuntimeEnvironmentProxy::ArcGISRuntimeEnvironmentProxy(QObject* parent) :
  QObject(parent),
  m_cachedApiKey(Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::apiKey())
{
}

ArcGISRuntimeEnvironmentProxy::~ArcGISRuntimeEnvironmentProxy()
{
}

QString ArcGISRuntimeEnvironmentProxy::apiKey() const
{
  return Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::apiKey();
}

void ArcGISRuntimeEnvironmentProxy::setApiKey(const QString& apiKey)
{
  using namespace Esri::ArcGISRuntime;
  const auto normalizedApiKey = apiKey.trimmed();
  const auto oldApiKey = ArcGISRuntimeEnvironment::apiKey();

  if (oldApiKey == normalizedApiKey)
  {
    return;
  }

  m_cachedApiKey = normalizedApiKey;

  ArcGISRuntimeEnvironment::setApiKey(normalizedApiKey);
  emit apiKeyChanged();
}

void ArcGISRuntimeEnvironmentProxy::cacheCurrentChallengeHandler()
{
  using namespace Esri::ArcGISRuntime;
  if (m_toolkitChallengeHandler == nullptr)
  {
    m_toolkitChallengeHandler = ArcGISRuntimeEnvironment::authenticationManager()->arcGISAuthenticationChallengeHandler();
  }
}

void ArcGISRuntimeEnvironmentProxy::prepareForDemoSwitch()
{
  setApiKey(m_cachedApiKey);
  resetAuthenticationState();
}

void ArcGISRuntimeEnvironmentProxy::resetAuthenticationState()
{
  using namespace Esri::ArcGISRuntime;

  ArcGISRuntimeEnvironment::authenticationManager()->arcGISCredentialStore()->removeAll();

  Esri::ArcGISRuntime::Toolkit::OAuthUserConfigurationManager::clearConfigurations();

  if (m_toolkitChallengeHandler != nullptr)
  {
    ArcGISRuntimeEnvironment::authenticationManager()->setArcGISAuthenticationChallengeHandler(m_toolkitChallengeHandler);
  }

  if (auto* authController = Toolkit::AuthenticatorController::instance(); authController)
  {
    authController->cancelOutstandingChallenges();
  }
}
