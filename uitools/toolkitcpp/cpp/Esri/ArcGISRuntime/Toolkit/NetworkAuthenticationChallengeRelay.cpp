
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
#include "NetworkAuthenticationChallengeRelay.h"
#include "ArcGISAuthenticationController.h"

// Maps SDK headers
#include <Authentication/NetworkAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
 \class Toolkit::NetworkAuthenticationChallengeRelay
 \inmodule EsriArcGISRuntimeToolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief Simple implementation of NetworkAuthenticationChallengeHandler.

 \internal
 */

/*!
  \internal
 */
NetworkAuthenticationChallengeRelay::NetworkAuthenticationChallengeRelay(ArcGISAuthenticationController* controller) :
  NetworkAuthenticationChallengeHandler(controller),
  m_controller(controller)
{
  ArcGISRuntimeEnvironment::instance()->authenticationManager()->setNetworkAuthenticationChallengeHandler(this);
}

/*!
  \internal
 */
NetworkAuthenticationChallengeRelay::~NetworkAuthenticationChallengeRelay() = default;

/*!
  \internal
 */
void NetworkAuthenticationChallengeRelay::handleNetworkAuthenticationChallenge(NetworkAuthenticationChallenge* challenge)
{
  m_controller->handleNetworkAuthenticationChallenge(challenge);
}

} //  Esri::ArcGISRuntime::Toolkit
