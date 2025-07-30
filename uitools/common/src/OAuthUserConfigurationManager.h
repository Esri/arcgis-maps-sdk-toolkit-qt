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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_OAUTHUSERCONFIGURATIONMANAGER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_OAUTHUSERCONFIGURATIONMANAGER_H

// Qt headers
#include <QtContainerFwd>

namespace Esri::ArcGISRuntime::Authentication {
  class OAuthUserConfiguration;
}

namespace Esri::ArcGISRuntime::Toolkit {

class OAuthUserConfigurationManager
{
public:
  OAuthUserConfigurationManager() = delete;

  static void addConfiguration(Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration* userConfiguration);

  static void clearConfigurations();

  static QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> configurations();
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_OAUTHUSERCONFIGURATIONMANAGER_H
