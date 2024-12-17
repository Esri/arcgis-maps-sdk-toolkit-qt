/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H

// Qt headers
#include <QObject>
#include <QPointer>

// STL headers
#include <CoreTypes.h>

namespace Esri::ArcGISRuntime {
  class AuthenticationChallenge;
}

namespace Esri::ArcGISRuntime::Toolkit {

class AuthenticationController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QUrl currentChallengeUrl READ currentChallengeUrl NOTIFY currentChallengeChanged)
  Q_PROPERTY(QString currentAuthenticatingHost READ currentAuthenticatingHost NOTIFY currentChallengeChanged)
  Q_PROPERTY(int currentChallengeType READ currentChallengeType NOTIFY currentChallengeChanged)
  Q_PROPERTY(int currentChallengeFailureCount READ currentChallengeFailureCount NOTIFY currentChallengeChanged)
  Q_PROPERTY(QStringList clientCertificateInfos READ clientCertificateInfos NOTIFY clientCertificateInfosChanged)

public:

  explicit Q_INVOKABLE AuthenticationController(QObject* parent = nullptr);

  ~AuthenticationController() override;

  // General methods

  Q_INVOKABLE void addClientCertificate(const QUrl& clientCertificate, const QString& password = QString{});

  QStringList clientCertificateInfos() const;

  void setDeleteChallengeOnProcessed(bool deleteFlag);

  bool deleteChallengeOnProcessed() const;

  // Current challenge methods

  int currentChallengeType() const;

  QUrl currentChallengeUrl() const;

  QString currentAuthenticatingHost() const;

  int currentChallengeFailureCount() const;

  Q_INVOKABLE void continueWithUsernamePassword(const QString& username, const QString& password);

  Q_INVOKABLE void continueWithOAuthAuthorizationCode(const QString& oAuthAuthorizationCode);

  Q_INVOKABLE void continueWithClientCertificate(int clientCertificateIndex);

  Q_INVOKABLE void continueWithSslHandshake(bool trust, bool remember);

  Q_INVOKABLE void cancel();

  Q_INVOKABLE void cancelWithError(const QString& title, const QString& html);

signals:
    void currentChallengeChanged();

    void clientCertificateInfosChanged();

    void clientCertificatePasswordRequired(QUrl certificate);

private:
  void cleanup();
private:
    QPointer<AuthenticationChallenge> m_currentChallenge;
    bool m_deleteChallengeOnProcessed = true;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_AUTHENTICATIONCONTROLLER_H
