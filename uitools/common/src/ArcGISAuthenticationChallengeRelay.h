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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCHALLENGERELAY_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCHALLENGERELAY_H

// ArcGIS Maps SDK headers
#include <Authentication/ArcGISAuthenticationChallengeHandler.h>

namespace Esri::ArcGISRuntime::Toolkit {

class AuthenticatorController;
class ArcGISAuthenticationChallengeRelay : public Authentication::ArcGISAuthenticationChallengeHandler
{
  Q_OBJECT

public:
  explicit ArcGISAuthenticationChallengeRelay(AuthenticatorController* controller);
  ~ArcGISAuthenticationChallengeRelay();

protected:
  void handleArcGISAuthenticationChallenge(Authentication::ArcGISAuthenticationChallenge* challenge) override;

private:
  AuthenticatorController* m_controller = nullptr;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCHALLENGERELAY_H
