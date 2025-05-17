
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
#include "CustomOAuth2AuthorizationCodeFlow.h"

#include <utility>

using namespace Esri::ArcGISRuntime;

namespace Esri::ArcGISRuntime::Toolkit {

CustomOAuth2AuthorizationCodeFlow::CustomOAuth2AuthorizationCodeFlow(QUrl authorizeUrl, QObject* parent) :
  QAbstractOAuth2(parent),
  m_authorizeUrl(std::move(authorizeUrl))
{
}

CustomOAuth2AuthorizationCodeFlow::~CustomOAuth2AuthorizationCodeFlow() = default;

void CustomOAuth2AuthorizationCodeFlow::grant()
{
  emit authorizeWithBrowser(m_authorizeUrl);
}

QString CustomOAuth2AuthorizationCodeFlow::authorizationCode() const
{
  return m_authorizationCode;
}

void CustomOAuth2AuthorizationCodeFlow::setAuthorizationCode(const QString& code)
{
  m_authorizationCode = code;
}

} //  Esri::ArcGISRuntime::Toolkit
