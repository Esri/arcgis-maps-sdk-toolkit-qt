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
#include <QQmlEngine>

// STL headers
#include <Authentication/ArcGISAuthenticationChallengeHandler.h>
#include <memory>
#include <mutex>

namespace Esri::ArcGISRuntime::Authentication {
  class OAuthUserConfiguration;
  class OAuthUserLoginPrompt;
}

Q_MOC_INCLUDE(<QUrl>)

namespace Esri::ArcGISRuntime::Toolkit {

class ArcGISAuthenticationController : public Authentication::ArcGISAuthenticationChallengeHandler
{
  Q_OBJECT

  // general
  Q_PROPERTY(bool canBeUsed READ canBeUsed_ CONSTANT)

  // token authentication
  Q_PROPERTY(QUrl currentAuthenticatingHost READ currentAuthenticatingHost_ NOTIFY currentAuthenticatingHostChanged)

  // OAuth
  Q_PROPERTY(QUrl authorizeUrl READ authorizeUrl_ NOTIFY authorizeUrlChanged)
  Q_PROPERTY(bool preferPrivateWebBrowserSession READ preferPrivateWebBrowserSession_ NOTIFY preferPrivateWebBrowserSessionChanged)
  Q_PROPERTY(QString redirectUri READ redirectUri_ NOTIFY redirectUriChanged)

  Q_PROPERTY(int currentChallengeFailureCount READ currentChallengeFailureCount_ NOTIFY currentChallengeFailureCountChanged)

public:
  // assumed to be owned by the QML engine
  static ArcGISAuthenticationController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);
  static ArcGISAuthenticationController* instance();

  ~ArcGISAuthenticationController() override;

  // callbacks invoked by QML
  // token authentication
  Q_INVOKABLE void continueWithUsernamePassword(const QString& username, const QString& password);

  // OAuth
  Q_INVOKABLE void respond(const QUrl& url);
  Q_INVOKABLE void respondWithError(const QString& platformError);

  Q_INVOKABLE void cancel();

  // ArcGISAuthenticationChallengeHandler interface
  void handleArcGISAuthenticationChallenge(Authentication::ArcGISAuthenticationChallenge* challenge) override;

  // OAuth user challenge support
  void addOAuthUserConfiguration(Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration* userConfiguration);
  void setOAuthUserConfigurations(QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> userConfigurations);
  void clearOAuthUserConfigurations();
  QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> oAuthUserConfigurations() const;

signals:
  void displayOAuthSignInView();
  void displayUsernamePasswordSignInView();
  void currentAuthenticatingHostChanged();
  void authorizeUrlChanged();
  void preferPrivateWebBrowserSessionChanged();
  void redirectUriChanged();
  void currentChallengeFailureCountChanged();

private:
  explicit ArcGISAuthenticationController(QObject* parent = nullptr);
  bool canBeUsed_() const;
  QUrl currentAuthenticatingHost_() const;
  QUrl authorizeUrl_() const;
  bool preferPrivateWebBrowserSession_() const;
  QString redirectUri_() const;
  int currentChallengeFailureCount_() const;

  std::unique_ptr<Authentication::ArcGISAuthenticationChallenge> m_currentChallenge;
  QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> m_userConfigurations;
  std::unique_ptr<Authentication::OAuthUserLoginPrompt> m_currentOAuthUserLoginPrompt;
  int m_currentChallengeFailureCount = 0;
  static inline constexpr int s_maxChallengeFailureCount = 5;
  std::mutex m_mutex;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_ARCGISAUTHENTICATIONCONTROLLER_H