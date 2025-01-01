
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
#include "ArcGISAuthenticationChallengeRelay.h"
#include "ArcGISAuthenticationController.h"

// Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallenge.h>
#include <Authentication/AuthenticationManager.h>
#include <ArcGISRuntimeEnvironment.h>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Authentication;

namespace Esri::ArcGISRuntime::Toolkit {

/*!
 \class Toolkit::ArcGISAuthenticationChallengeRelay
 \inmodule EsriArcGISRuntimeToolkit
 \ingroup ArcGISQtToolkitUiCppControllers
 \brief Simple implementation of ArcGISAuthenticationChallengeHandler.

 \internal
 */

/*!
  \internal
 */
ArcGISAuthenticationChallengeRelay::ArcGISAuthenticationChallengeRelay(ArcGISAuthenticationController* controller) :
  ArcGISAuthenticationChallengeHandler(controller),
  m_controller(controller)
{
  ArcGISRuntimeEnvironment::instance()->authenticationManager()->setArcGISAuthenticationChallengeHandler(this);
}

/*!
  \internal
 */
ArcGISAuthenticationChallengeRelay::~ArcGISAuthenticationChallengeRelay() = default;

/*!
  \internal
 */
void ArcGISAuthenticationChallengeRelay::handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge)
{
  m_controller->handleArcGISAuthenticationChallenge(challenge);
}

} //  Esri::ArcGISRuntime::Toolkit
