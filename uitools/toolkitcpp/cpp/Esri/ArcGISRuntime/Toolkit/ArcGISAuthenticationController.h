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
/// \file ArcGISAuthenticationController.h

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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCONTROLLER_H

// Qt headers
#include <QObject>

// STL headers
#include <ArcGISAuthenticationChallengeHandler.h>
#include <mutex>

namespace Esri::ArcGISRuntime {
  class OAuthUserConfiguration;
}

Q_MOC_INCLUDE(<QUrl>)

namespace Esri::ArcGISRuntime::Toolkit {

class ArcGISAuthenticationController : public ArcGISAuthenticationChallengeHandler
{
  Q_OBJECT

  Q_PROPERTY(QUrl currentAuthenticatingHost READ currentAuthenticatingHost_ NOTIFY currentAuthenticatingHostChanged)
  Q_PROPERTY(bool canBeUsed READ canBeUsed_ CONSTANT)

public:
  explicit Q_INVOKABLE ArcGISAuthenticationController(QObject* parent = nullptr);

  ~ArcGISAuthenticationController() override;

  Q_INVOKABLE void continueWithUsernamePassword(const QString& username, const QString& password);

  void handleArcGISAuthenticationChallenge(ArcGISAuthenticationChallenge* challenge) override;

  void addOAuthUserConfiguration(Esri::ArcGISRuntime::OAuthUserConfiguration* userConfiguration);
  void setOAuthUserConfigurations(QList<Esri::ArcGISRuntime::OAuthUserConfiguration*> userConfigurations);
  void clearOAuthUserConfigurations();
  QList<Esri::ArcGISRuntime::OAuthUserConfiguration*> oAuthUserConfigurations() const;

signals:
  void displayOAuthSignInView();
  void displayUsernamePasswordSignInView();
  void currentAuthenticatingHostChanged();

private:
  bool canBeUsed_() const;
  QUrl currentAuthenticatingHost_() const;

  ArcGISAuthenticationChallenge* m_currentChallenge = nullptr;
  QList<Esri::ArcGISRuntime::OAuthUserConfiguration*> m_userConfigurations;
  std::mutex m_mutex;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCONTROLLER_H
