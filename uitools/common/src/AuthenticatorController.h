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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATORCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATORCONTROLLER_H

// Qt headers
#include <QHash>
#include <QObject>
#include <QQmlEngine>

// STL headers
#include <memory>
#include <mutex>

class QUrl;

Q_MOC_INCLUDE(<QUrl>)

namespace Esri::ArcGISRuntime::Authentication {
class OAuthUserConfiguration;
class OAuthUserLoginPrompt;
class ArcGISAuthenticationChallenge;
class NetworkAuthenticationChallenge;
}

namespace Esri::ArcGISRuntime::Toolkit {

class ArcGISAuthenticationChallengeRelay;
class NetworkAuthenticationChallengeRelay;

class AuthenticatorController : public QObject
{
  Q_OBJECT

  // general
  Q_PROPERTY(bool canBeUsed READ canBeUsed_ CONSTANT)

  // token authentication
  Q_PROPERTY(QString currentAuthenticatingHost READ currentAuthenticatingHost_ NOTIFY currentAuthenticatingHostChanged)

  // OAuth
  Q_PROPERTY(QUrl authorizeUrl READ authorizeUrl_ NOTIFY authorizeUrlChanged)
  Q_PROPERTY(QString redirectUri READ redirectUri_ NOTIFY redirectUriChanged)

  Q_PROPERTY(int previousFailureCount READ previousFailureCount_ NOTIFY previousFailureCountChanged)

public:
  // assumed to be owned by the QML engine
  static AuthenticatorController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);
  static AuthenticatorController* instance();

  ~AuthenticatorController() override;

  enum class CertificateResult
  {
    Accepted,
    PasswordRejected,
    Error
  };
  Q_ENUM(CertificateResult)

  // callbacks invoked by QML
  // token authentication
  Q_INVOKABLE void continueWithUsernamePassword(const QString& username, const QString& password);

  // OAuth
  Q_INVOKABLE void respond(const QUrl& url);
  Q_INVOKABLE void respondWithError(const QString& platformError);

  // ServerTrust
  Q_INVOKABLE void continueWithServerTrust(bool trust);

  // ClientCertificate
  Q_INVOKABLE CertificateResult respondWithClientCertificate(const QUrl& path, const QString& password);

  Q_INVOKABLE void cancel();

  // ArcGISAuthenticationChallengeHandler relay
  void handleArcGISAuthenticationChallenge(Authentication::ArcGISAuthenticationChallenge* challenge);

  // NetworkAuthenticationChallengeHandler relay
  void handleNetworkAuthenticationChallenge(Authentication::NetworkAuthenticationChallenge* challenge);

  // OAuth user challenge support
  void addOAuthUserConfiguration(Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration* userConfiguration);
  void clearOAuthUserConfigurations();
  QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> oAuthUserConfigurations() const;

signals:
  void displayClientCertificateView();
  void displayOAuthSignInView();
  void displayUsernamePasswordSignInView();
  void displayAuthenticatorServerTrustView();
  void currentAuthenticatingHostChanged();
  void authorizeUrlChanged();
  void redirectUriChanged();
  void previousFailureCountChanged();

private:
  explicit AuthenticatorController(QObject* parent = nullptr);
  bool canBeUsed_() const;
  QString currentAuthenticatingHost_() const;
  QUrl authorizeUrl_() const;
  QString redirectUri_() const;
  int previousFailureCount_() const;

  void processOAuthExternalBrowserLogin_();
  void finishOAuthExternalBrowserChallengeFlow_();

  void continueWithUsernamePasswordArcGIS_(const QString& username, const QString& password);
  void continueWithUsernamePasswordNetwork_(const QString& username, const QString& password);

  std::unique_ptr<ArcGISAuthenticationChallengeRelay> m_arcGISAuthenticationChallengeRelay;
  std::unique_ptr<NetworkAuthenticationChallengeRelay> m_networkAuthenticationChallengeRelay;

  std::unique_ptr<Authentication::ArcGISAuthenticationChallenge> m_currentArcGISChallenge;
  std::unique_ptr<Authentication::NetworkAuthenticationChallenge> m_currentNetworkChallenge;
  QList<Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration*> m_userConfigurations;
  Esri::ArcGISRuntime::Authentication::OAuthUserConfiguration* m_currentOAuthUserConfiguration = nullptr;
  std::unique_ptr<Authentication::OAuthUserLoginPrompt> m_currentOAuthUserLoginPrompt;

  std::mutex m_mutex;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATORCONTROLLER_H
