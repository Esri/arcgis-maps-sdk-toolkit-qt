
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
#include "ArcGISAuthenticationChallengeRelay.h"
#include "AuthenticatorController.h"

// Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISAuthenticationChallengeRelay
  \inmodule EsriArcGISRuntimeToolkit
  \ingroup ArcGISQtToolkitUiCppControllers
  \brief Simple implementation of the ArcGISAuthenticationChallengeHandler interface.

  This class is used to relay authentication challenges to the AuthenticatorController. Since multiple
  inheritance of QObject is not allowed, and both ChallengeHandler classes inherit QObject, we use a
  composition pattern instead.

  \internal
 */

ArcGISAuthenticationChallengeRelay::ArcGISAuthenticationChallengeRelay(AuthenticatorController* controller) :
  ArcGISAuthenticationChallengeHandler(controller),
  m_controller(controller)
{
  ArcGISRuntimeEnvironment::authenticationManager()->setArcGISAuthenticationChallengeHandler(this);
}

ArcGISAuthenticationChallengeRelay::~ArcGISAuthenticationChallengeRelay() = default;

void ArcGISAuthenticationChallengeRelay::handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge)
{
  m_controller->handleArcGISAuthenticationChallenge(challenge);
}

} //  Esri::ArcGISRuntime::Toolkit
