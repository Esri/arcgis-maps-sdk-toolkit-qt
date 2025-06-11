
/*******************************************************************************
 *  Copyright 2012-2025 Esri
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
#include "OAuthUserConfigurationManager.h"

// Qt headers
#include <QList>

// Toolkit headers
#include "AuthenticatorController.h"

using namespace Esri::ArcGISRuntime::Authentication;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \inmodule Esri.ArcGISRuntime.Toolkit
  \ingroup GenericTools
  \class Esri::ArcGISRuntime::Toolkit::OAuthUserConfigurationManager
  \brief A class that manages the available OAuthUserConfiguration instances for the application.

  All added configurations will become owned by the OAuthUserConfigurationManager. Any parent
  applied will be removed.

  In order for your application to issue OAuth challenges, you must add at least one OAuthUserConfiguration
  to the OAuthUserConfigurationManager.

  For more information, see the \l
  {https://developers.arcgis.com/documentation/security-and-authentication/user-authentication/tutorials/create-oauth-credentials-user-auth/}
  {OAuth tutorial}.

  \since Esri.ArcGISRuntime::Toolkit 200.8
 */

/*!
  \brief Adds \a userConfiguration to the list of OAuthUserConfigurations.

  \note any QObject parent will be removed and reset. Ownership is managed by the OAuthUserConfigurationManager.
 */
void OAuthUserConfigurationManager::addConfiguration(OAuthUserConfiguration* userConfiguration)
{
  AuthenticatorController::instance()->addOAuthUserConfiguration(userConfiguration);
}

/*!
  \brief Clears all OAuthUserConfigurations.

  All configurations will be removed and deleted. Your app will no longer be able to handle any OAuth challenges.
  This will not impact any existing, successfully authenticated credentials/sessions.
 */
void OAuthUserConfigurationManager::clearConfigurations()
{
  AuthenticatorController::instance()->clearOAuthUserConfigurations();
}

/*!
  \brief Returns the list of OAuthUserConfigurations in use.

  The returned objects are owned by the OAuthUserConfigurationManager.
 */
QList<OAuthUserConfiguration*> OAuthUserConfigurationManager::configurations()
{
  return AuthenticatorController::instance()->oAuthUserConfigurations();
}

} //  Esri::ArcGISRuntime::Toolkit
